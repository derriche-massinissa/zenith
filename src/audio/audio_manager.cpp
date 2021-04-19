/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "audio_manager.hpp"

#include "tools/al_utility.hpp"
#include "tools/alc_utility.hpp"
#include "tools/ogg.hpp"
#include "entt/entt.hpp"
#include "../components/audio.hpp"
#include "../components/audio_stream.hpp"
#include "../utils/assert.hpp"

namespace Zen {

extern entt::registry g_registry;

AudioManager::AudioManager ()
{}

AudioManager::~AudioManager ()
{
	// Delete buffers
	/*
	for (auto it_ : buffers)
	{
		ZEN_AL_CALL(alDeleteBuffers, 1, &it_.second.buffer);
	}
	*/

	// Clear streams
	for (auto& sd_ : streams)
	{
		close_stream_ogg(&sd_);
	}

	ALCboolean flag = false;
	if ( device != nullptr && !ZEN_ALC_CALL(alcMakeContextCurrent, flag, device, nullptr) )
	{}

	if ( context != nullptr && !ZEN_ALC_CALL(alcDestroyContext, device, context) )
	{}

	if ( device != nullptr && !ZEN_ALC_CALL(alcCloseDevice, flag, device, device) )
	{}
}

void AudioManager::boot ()
{
	device = alcOpenDevice(nullptr);
	if (device == nullptr)
	{
		MessageError("No audio device could be opened");
		return;
	}

	if ( !ZEN_ALC_CALL(alcCreateContext, context, device, device, nullptr) || context == nullptr )
	{
		MessageError("Could not create audio context");
		alcCloseDevice(device);
		return;
	}

	ALCboolean flag = false;
	if ( !ZEN_ALC_CALL(alcMakeContextCurrent, flag, device, context) || flag != ALC_TRUE )
	{
		MessageError("Could not make audio context current");
		alcDestroyContext(context);
		alcCloseDevice(device);
	}
}

void AudioManager::addAudioShort (std::string key_, std::string filename_)
{
	// Check if key is used
	auto itSh_ = buffers.find(key_);
	if (itSh_ != buffers.end()) {
		MessageError("The key \"", key_, "\" is already in use for a short");
		return;
	}

	auto itSt_ = streamSources.find(key_);
	if (itSt_ != streamSources.end()) {
		MessageError("The key \"", key_, "\" is already in use for a stream");
		return;
	}

	buffers[key_] = {};
	load_ogg(filename_, &buffers[key_]);
}

void AudioManager::addAudioStream (std::string key_, std::string filename_)
{
	// Check if key is used	for a stream
	auto itSt_ = streamSources.find(key_);
	if (itSt_ != streamSources.end()) {
		MessageError("The key \"", key_, "\" is already in use for a stream");
		return;
	}

	auto itSh_ = buffers.find(key_);
	if (itSh_ != buffers.end()) {
		MessageError("The key \"", key_, "\" is already in use for a short");
		return;
	}

	// Check if the file exists
	std::ifstream file_ (filename_);
	if (!file_.is_open())
	{
		MessageError("Could not open file \"", filename_, "\"");
		return;
	}

	file_.close();
	streamSources[key_] = filename_;
}

Entity AudioManager::add (std::string key_)
{
	// Figure out if the key is for a stream or a short
	if (buffers.find(key_) != buffers.end())
	{
		// Short
		Entity audio_ = g_registry.create();
		auto& audioComp_ = g_registry.emplace<Components::Audio>(audio_);

		ZEN_AL_CALL(alGenSources, 1, &audioComp_.source);
		ZEN_AL_CALL(alSourcef, audioComp_.source, AL_PITCH, 1.f);
		ZEN_AL_CALL(alSourcef, audioComp_.source, AL_GAIN, 1.f);
		ZEN_AL_CALL(alSource3f, audioComp_.source, AL_POSITION, 0.f, 0.f, 0.f);
		ZEN_AL_CALL(alSource3f, audioComp_.source, AL_VELOCITY, 0.f, 0.f, 0.f);
		ZEN_AL_CALL(alSourcei, audioComp_.source, AL_LOOPING, AL_FALSE);

		ZEN_AL_CALL(alSourcei, audioComp_.source, AL_BUFFER, buffers[key_].buffer);

		return audio_;
	}
	else if (streamSources.find(key_) != streamSources.end())
	{
		// Stream
		Entity stream_ = g_registry.create();
		auto& streamComp_ = g_registry.emplace<Components::AudioStream>(stream_);

		streamComp_.index = streams.size();
		streams.emplace_back();
		auto& data_ = streams.back();

		ZEN_AL_CALL(alGenSources, 1, &data_.source);
		ZEN_AL_CALL(alSourcef, data_.source, AL_PITCH, 1.f);
		ZEN_AL_CALL(alSourcef, data_.source, AL_GAIN, 1.f);
		ZEN_AL_CALL(alSource3f, data_.source, AL_POSITION, 0.f, 0.f, 0.f);
		ZEN_AL_CALL(alSource3f, data_.source, AL_VELOCITY, 0.f, 0.f, 0.f);
		ZEN_AL_CALL(alSourcei, data_.source, AL_LOOPING, AL_FALSE);

		setup_stream_ogg(streamSources[key_], &data_);

		return stream_;
	}
	else
	{
		// Key isn't used
		MessageError("The key \"", key_, "\" is not used for any loaded audio");
		return entt::null;
	}
}

void AudioManager::play (Entity entity_)
{
	ZEN_ASSERT((entity_ != entt::null), "Can't play a null audio");
	auto [audio_,  stream_] = g_registry.try_get<Components::Audio, Components::AudioStream>(entity_);
	ZEN_ASSERT((audio_ || stream_), "The entity has no 'Audio' nor 'AudioStream' component.");

	if (audio_ != nullptr)
	{
		ZEN_AL_CALL(alSourceRewind, audio_->source);
		ZEN_AL_CALL(alSourcePlay, audio_->source);
	}
	else if (stream_ != nullptr)
	{
		rewind_stream_ogg(&streams[stream_->index]);
		ZEN_AL_CALL(alSourcePlay, streams[stream_->index].source);
	}
}

void AudioManager::play (Entity entity_, AudioConfig config_)
{
	auto audio_ = g_registry.try_get<Components::Audio>(entity_);
	ZEN_ASSERT(audio_, "The entity has no 'Audio' component.");

	ZEN_AL_CALL(alSourceRewind, audio_->source);
	ZEN_AL_CALL(alSourcePlay, audio_->source);
}

void AudioManager::play (Entity entity_, AudioMarker marker_)
{
	auto audio_ = g_registry.try_get<Components::Audio>(entity_);
	ZEN_ASSERT(audio_, "The entity has no 'Audio' component.");

	ZEN_AL_CALL(alSourceRewind, audio_->source);
	ZEN_AL_CALL(alSourcePlay, audio_->source);
}

void AudioManager::pause (Entity entity_)
{
	auto audio_ = g_registry.try_get<Components::Audio>(entity_);
	ZEN_ASSERT(audio_, "The entity has no 'Audio' component.");

	ZEN_AL_CALL(alSourcePause, audio_->source);
}

void AudioManager::resume (Entity entity_)
{
	auto audio_ = g_registry.try_get<Components::Audio>(entity_);
	ZEN_ASSERT(audio_, "The entity has no 'Audio' component.");

	ZEN_AL_CALL(alSourcePlay, audio_->source);
}

void AudioManager::stop (Entity entity_)
{
	auto audio_ = g_registry.try_get<Components::Audio>(entity_);
	ZEN_ASSERT(audio_, "The entity has no 'Audio' component.");

	ZEN_AL_CALL(alSourceStop, audio_->source);
}

void AudioManager::remove (Entity entity_)
{
	g_registry.destroy(entity_);
}

void AudioManager::loop (Entity entity_, bool loop_)
{
	auto audio_ = g_registry.try_get<Components::Audio>(entity_);
	ZEN_ASSERT(audio_, "The entity has no 'Audio' component.");

	if (loop_)
		ZEN_AL_CALL(alSourcei, audio_->source, AL_LOOPING, AL_TRUE);
	else
		ZEN_AL_CALL(alSourcei, audio_->source, AL_LOOPING, AL_FALSE);
}

void AudioManager::removeAll ()
{
	auto view_ = g_registry.view<Components::Audio>();
	for (auto& audio_ : view_)
	{
		g_registry.destroy(audio_);
	}
}

void AudioManager::pauseAll ()
{
	auto view_ = g_registry.view<Components::Audio>();
	for (auto& ent_ : view_)
	{
		auto& audio_ = g_registry.get<Components::Audio>(ent_);
		ZEN_AL_CALL(alSourcePause, audio_.source);
	}
}

void AudioManager::resumeAll ()
{
	auto view_ = g_registry.view<Components::Audio>();
	for (auto& ent_ : view_)
	{
		auto& audio_ = g_registry.get<Components::Audio>(ent_);
		ZEN_AL_CALL(alSourcePlay, audio_.source);
	}
}

void AudioManager::stopAll ()
{
	auto view_ = g_registry.view<Components::Audio>();
	for (auto& ent_ : view_)
	{
		auto& audio_ = g_registry.get<Components::Audio>(ent_);
		ZEN_AL_CALL(alSourceStop, audio_.source);
	}
}

void AudioManager::onWindowBlur ()
{
}

void AudioManager::onWindowFocus ()
{
}

void AudioManager::update (Uint32 time_, Uint32 delta_)
{
	auto streamsView_ = g_registry.view<Components::AudioStream>();

	for (auto& stream_ : streamsView_)
	{
		auto& strCmp_ = g_registry.get<Components::AudioStream>(stream_);

		update_stream_ogg(&streams[strCmp_.index]);
	}

	// Emit events?
}

void AudioManager::forEachActiveSound ()
{
}

void AudioManager::setPosition (Entity entity_, float x_, float y_, float z_)
{
	auto audio_ = g_registry.try_get<Components::Audio>(entity_);
	ZEN_ASSERT(audio_, "The entity has no 'Audio' component.");

	ZEN_AL_CALL(alSource3f, audio_->source, AL_POSITION, x_, y_, z_);
}

void AudioManager::setVelocity (Entity entity_, float x_, float y_, float z_)
{
	auto audio_ = g_registry.try_get<Components::Audio>(entity_);
	ZEN_ASSERT(audio_, "The entity has no 'Audio' component.");

	ZEN_AL_CALL(alSource3f, audio_->source, AL_VELOCITY, x_, y_, z_);
}

void AudioManager::setVolume (double volume_)
{
}

void AudioManager::mute ()
{
}

void AudioManager::unmute ()
{
}

AudioManager g_audio;

}	// namespace Zen
