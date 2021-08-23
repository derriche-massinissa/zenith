/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "audio_manager.hpp"

#include <algorithm>
#include "tools/al_utility.hpp"
#include "tools/alc_utility.hpp"
#include "tools/ogg.hpp"
#include "events/events.hpp"
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
		audio_->stopped = false;
		audio_->completed = false;

		emit(entity_, ZEN_AUDIO_EVENTS_PLAY);
	}
	else if (stream_ != nullptr)
	{
		rewind_stream_ogg(&streams[stream_->index]);
		ZEN_AL_CALL(alSourcePlay, streams[stream_->index].source);
		streams[stream_->index].playing = true;

		emit(entity_, ZEN_AUDIO_EVENTS_PLAY);
	}
}

void AudioManager::play (Entity entity_, AudioConfig config_)
{
}

void AudioManager::play (Entity entity_, AudioMarker marker_)
{
}

void AudioManager::pause (Entity entity_)
{
	ZEN_ASSERT((entity_ != entt::null), "Can't pause a null audio");

	auto [audio_,  stream_] = g_registry.try_get<Components::Audio, Components::AudioStream>(entity_);
	ZEN_ASSERT((audio_ || stream_), "The entity has no 'Audio' nor 'AudioStream' component.");

	if (audio_ != nullptr)
	{
		ZEN_AL_CALL(alSourcePause, audio_->source);

		emit(entity_, ZEN_AUDIO_EVENTS_PAUSE);
	}
	else if (stream_ != nullptr)
	{
		ZEN_AL_CALL(alSourcePause, streams[stream_->index].source);
		streams[stream_->index].playing = false;

		emit(entity_, ZEN_AUDIO_EVENTS_PAUSE);
	}
}

void AudioManager::resume (Entity entity_)
{
	ZEN_ASSERT((entity_ != entt::null), "Can't resume a null audio");

	auto [audio_,  stream_] = g_registry.try_get<Components::Audio, Components::AudioStream>(entity_);
	ZEN_ASSERT((audio_ || stream_), "The entity has no 'Audio' nor 'AudioStream' component.");

	if (audio_ != nullptr && !audio_->stopped)
	{
		ZEN_AL_CALL(alSourcePlay, audio_->source);
		emit(entity_, ZEN_AUDIO_EVENTS_RESUME);
	}
	else if (stream_ != nullptr)
	{
		ZEN_AL_CALL(alSourcePlay, streams[stream_->index].source);
		streams[stream_->index].playing = true;
		emit(entity_, ZEN_AUDIO_EVENTS_RESUME);
	}
}

void AudioManager::stop (Entity entity_)
{
	ZEN_ASSERT((entity_ != entt::null), "Can't stop a null audio");

	auto [audio_,  stream_] = g_registry.try_get<Components::Audio, Components::AudioStream>(entity_);
	ZEN_ASSERT((audio_ || stream_), "The entity has no 'Audio' nor 'AudioStream' component.");

	if (audio_ != nullptr)
	{
		ZEN_AL_CALL(alSourceStop, audio_->source);
		audio_->stopped = true;
	}
	else if (stream_ != nullptr)
	{
		ZEN_AL_CALL(alSourceStop, streams[stream_->index].source);
		streams[stream_->index].playing = false;
	}

	emit(entity_, ZEN_AUDIO_EVENTS_STOP);
}

void AudioManager::remove (Entity entity_)
{
	ZEN_ASSERT((entity_ != entt::null), "Can't remove a null audio");

	auto [audio_,  stream_] = g_registry.try_get<Components::Audio, Components::AudioStream>(entity_);
	ZEN_ASSERT((audio_ || stream_), "The entity has no 'Audio' nor 'AudioStream' component.");

	if (audio_ != nullptr)
	{
		emit(entity_, ZEN_AUDIO_EVENTS_REMOVE);

		ZEN_AL_CALL(alSourceStop, audio_->source);
		ZEN_AL_CALL(alDeleteSources, 1, &audio_->source);
	}
	else if (stream_ != nullptr)
	{
		emit(entity_, ZEN_AUDIO_EVENTS_REMOVE);

		ZEN_AL_CALL(alSourceStop, streams[stream_->index].source);
		close_stream_ogg(&streams[stream_->index]);
	}

	g_registry.destroy(entity_);
}

void AudioManager::setLoop (Entity entity_, bool loop_)
{
	ZEN_ASSERT((entity_ != entt::null), "Can't loop a null audio");

	auto [audio_,  stream_] = g_registry.try_get<Components::Audio, Components::AudioStream>(entity_);
	ZEN_ASSERT((audio_ || stream_), "The entity has no 'Audio' nor 'AudioStream' component.");

	if (audio_ != nullptr)
	{
		audio_->loop = loop_;

		emit(entity_, ZEN_AUDIO_EVENTS_LOOP, loop_);
	}
	else if (stream_ != nullptr)
	{
		streams[stream_->index].loop = loop_;

		emit(entity_, ZEN_AUDIO_EVENTS_LOOP, loop_);
	}
}

void AudioManager::removeAll ()
{
	emit(ZEN_AUDIO_EVENTS_REMOVE_ALL);

	auto viewSh_ = g_registry.view<Components::Audio>();
	for (auto& audio_ : viewSh_)
		remove(audio_);

	auto viewSt_ = g_registry.view<Components::AudioStream>();
	for (auto& audio_ : viewSt_)
		remove(audio_);
}

void AudioManager::pauseAll ()
{
	auto viewSh_ = g_registry.view<Components::Audio>();
	for (auto& audio_ : viewSh_)
		pause(audio_);

	auto viewSt_ = g_registry.view<Components::AudioStream>();
	for (auto& audio_ : viewSt_)
		pause(audio_);

	emit(ZEN_AUDIO_EVENTS_PAUSE_ALL);
}

void AudioManager::resumeAll ()
{
	auto viewSh_ = g_registry.view<Components::Audio>();
	for (auto& audio_ : viewSh_)
		resume(audio_);

	auto viewSt_ = g_registry.view<Components::AudioStream>();
	for (auto& audio_ : viewSt_)
		resume(audio_);

	emit(ZEN_AUDIO_EVENTS_RESUME_ALL);
}

void AudioManager::stopAll ()
{
	auto viewSh_ = g_registry.view<Components::Audio>();
	for (auto& audio_ : viewSh_)
		stop(audio_);

	auto viewSt_ = g_registry.view<Components::AudioStream>();
	for (auto& audio_ : viewSt_)
		stop(audio_);

	emit(ZEN_AUDIO_EVENTS_STOP_ALL);
}

void AudioManager::onWindowBlur ()
{
}

void AudioManager::onWindowFocus ()
{
}

void AudioManager::update ([[maybe_unused]] Uint32 time_, [[maybe_unused]] Uint32 delta_)
{
	auto streamsView_ = g_registry.view<Components::AudioStream>();

	for (auto& stream_ : streamsView_) {
		auto& strCmp_ = g_registry.get<Components::AudioStream>(stream_);
		switch ( update_stream_ogg(&streams[strCmp_.index]) )
		{
			case 1:
				emit(stream_, ZEN_AUDIO_EVENTS_COMPLETE);
				break;
			case 2:
				emit(stream_, ZEN_AUDIO_EVENTS_LOOPED);
				break;
			default:
				break;
		}
	}

	auto shortView_ = g_registry.view<Components::Audio>();

	for (auto& short_ : shortView_) {
		auto& shCmp_ = g_registry.get<Components::Audio>(short_);

		ALint state;
		ZEN_AL_CALL(alGetSourcei, shCmp_.source, AL_SOURCE_STATE, &state);

		if (state == AL_PLAYING)
			continue;

		if (shCmp_.stopped)
			continue;

		if (shCmp_.loop) {
			ZEN_AL_CALL(alSourcePlay, shCmp_.source);
			emit(short_, ZEN_AUDIO_EVENTS_LOOPED);
		}
		else if (!shCmp_.loop && !shCmp_.completed) {
			shCmp_.completed = true;
			emit(short_, ZEN_AUDIO_EVENTS_COMPLETE);
		}
	}
}

void AudioManager::forEachActiveSound ()
{
}

void AudioManager::setPosition (ALfloat x_, ALfloat y_, ALfloat z_)
{
	ZEN_AL_CALL(alListener3f, AL_POSITION, x_, y_, z_);

	emit(ZEN_AUDIO_EVENTS_GLOBAL_POSITION, x_, y_, z_);
}

void AudioManager::setPosition (Entity entity_, ALfloat x_, ALfloat y_, ALfloat z_)
{
	ZEN_ASSERT((entity_ != entt::null), "Can't set the position of a null audio");

	auto [audio_,  stream_] = g_registry.try_get<Components::Audio, Components::AudioStream>(entity_);
	ZEN_ASSERT((audio_ || stream_), "The entity has no 'Audio' nor 'AudioStream' component.");

	if (audio_ != nullptr)
	{
		ZEN_AL_CALL(alSource3f, audio_->source, AL_POSITION, x_, y_, z_);

		emit(entity_, ZEN_AUDIO_EVENTS_POSITION, x_, y_, z_);
	}
	else if (stream_ != nullptr)
	{
		ZEN_AL_CALL(alSource3f, streams[stream_->index].source, AL_POSITION, x_, y_, z_);

		emit(entity_, ZEN_AUDIO_EVENTS_POSITION, x_, y_, z_);
	}
}

void AudioManager::setVelocity (ALfloat x_, ALfloat y_, ALfloat z_)
{
	ZEN_AL_CALL(alListener3f, AL_VELOCITY, x_, y_, z_);

	emit(ZEN_AUDIO_EVENTS_GLOBAL_VELOCITY, x_, y_, z_);
}

void AudioManager::setVelocity (Entity entity_, ALfloat x_, ALfloat y_, ALfloat z_)
{
	ZEN_ASSERT((entity_ != entt::null), "Can't set the position of a null audio");

	auto [audio_,  stream_] = g_registry.try_get<Components::Audio, Components::AudioStream>(entity_);
	ZEN_ASSERT((audio_ || stream_), "The entity has no 'Audio' nor 'AudioStream' component.");

	if (audio_ != nullptr)
	{
		ZEN_AL_CALL(alSource3f, audio_->source, AL_VELOCITY, x_, y_, z_);

		emit(entity_, ZEN_AUDIO_EVENTS_VELOCITY, x_, y_, z_);
	}
	else if (stream_ != nullptr)
	{
		ZEN_AL_CALL(alSource3f, streams[stream_->index].source, AL_VELOCITY, x_, y_, z_);

		emit(entity_, ZEN_AUDIO_EVENTS_VELOCITY, x_, y_, z_);
	}
}

void AudioManager::setVolume (ALfloat volume_)
{
	volume = std::clamp(volume_, 0.f, 1.f);

	ZEN_AL_CALL(alListenerf, AL_GAIN, volume);

	emit(ZEN_AUDIO_EVENTS_GLOBAL_VOLUME, volume_);
}

void AudioManager::setVolume (Entity entity_, ALfloat volume_)
{
	ZEN_ASSERT((entity_ != entt::null), "Can't set the volume of a null audio");

	auto [audio_,  stream_] = g_registry.try_get<Components::Audio, Components::AudioStream>(entity_);
	ZEN_ASSERT((audio_ || stream_), "The entity has no 'Audio' nor 'AudioStream' component.");

	if (audio_ != nullptr)
	{
		ZEN_AL_CALL(alSourcef, audio_->source, AL_GAIN, volume_);

		emit(entity_, ZEN_AUDIO_EVENTS_VOLUME, volume_);
	}
	else if (stream_ != nullptr)
	{
		ZEN_AL_CALL(alSourcef, streams[stream_->index].source, AL_GAIN, volume_);

		emit(entity_, ZEN_AUDIO_EVENTS_VOLUME, volume_);
	}
}

void AudioManager::setMute (bool mute_)
{
	if (mute_)
	{
		muted = true;
		ZEN_AL_CALL(alListenerf, AL_GAIN, 0.f);
	}
	else
	{
		muted = false;
		ZEN_AL_CALL(alListenerf, AL_GAIN, volume);
	}

	emit(ZEN_AUDIO_EVENTS_GLOBAL_MUTE, mute_);
}

void AudioManager::setMute (Entity entity_, bool mute_)
{
	ZEN_ASSERT((entity_ != entt::null), "Can't mute a null audio");

	auto [audio_,  stream_] = g_registry.try_get<Components::Audio, Components::AudioStream>(entity_);
	ZEN_ASSERT((audio_ || stream_), "The entity has no 'Audio' nor 'AudioStream' component.");

	if (audio_ != nullptr) {
		if (mute_) {
			audio_->muted = true;
			ZEN_AL_CALL(alSourcef, audio_->source, AL_GAIN, 0.f);
		}
		else {
			audio_->muted = false;
			ZEN_AL_CALL(alSourcef, audio_->source, AL_GAIN, audio_->volume);
		}

		emit(entity_, ZEN_AUDIO_EVENTS_MUTE, mute_);
	}
	else if (stream_ != nullptr) {
		if (mute_) {
			streams[stream_->index].muted = true;
			ZEN_AL_CALL(alSourcef, streams[stream_->index].source, AL_GAIN, 0.f);
		}
		else {
			streams[stream_->index].muted = false;
			ZEN_AL_CALL(
					alSourcef,
					streams[stream_->index].source,
					AL_GAIN,
					streams[stream_->index].volume);
		}

		emit(entity_, ZEN_AUDIO_EVENTS_MUTE, mute_);
	}
}

}	// namespace Zen
