/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_AUDIO_MANAGER_HPP
#define ZEN_AUDIO_MANAGER_HPP

#include <SDL2/SDL_stdinc.h>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <deque>
#include "../event/event_emitter.hpp"
#include "../ecs/entity.hpp"
#include "tools/al_utility.hpp"
#include "tools/alc_utility.hpp"
#include "types/audio_buffer.hpp"
#include "types/audio_stream_data.hpp"

namespace Zen {

// FIXME
struct AudioConfig {};
struct AudioMarker {};

class AudioManager : public EventEmitter
{
	ALCdevice* device = nullptr;
	ALCcontext* context = nullptr;

	std::map<std::string, AudioBuffer> buffers;

	std::map<std::string, std::string> streamSources;

	std::deque<AudioStreamData> streams;

	bool muted = false;

	ALfloat volume = 1.f;

	bool pauseOnBlur = false;

public:
	AudioManager ();

	~AudioManager ();

	void boot ();

	void addAudioShort (std::string key, std::string filename);

	void addAudioStream (std::string key, std::string filename);

	Entity add (std::string buffer);

	void play (Entity entity);

	void play (Entity entity, AudioConfig config);

	void play (Entity entity, AudioMarker marker);

	void pause (Entity entity);

	void resume (Entity entity);

	void stop (Entity entity);

	void remove (Entity entity);

	void setLoop (Entity entity, bool loop);

	void removeAll ();

	void pauseAll ();

	void resumeAll ();

	void stopAll ();

	void onWindowBlur ();

	void onWindowFocus ();

	void update (Uint32 time, Uint32 delta);

	void forEachActiveSound ();

	/**
	 * TODO
	 *
	 * The audio file must be in Mono and not Stereo for OpenAL to be able to
	 * pan
	 *
	 * teros channels are already computed whereas Mono isn't, so panning is calculated by OpenAL.
	 *
	 * OpenAL cannot play Stereo audio samples spacially.
	 *
	 * @since 0.0.0
	 */
	void setPosition (ALfloat x, ALfloat y, ALfloat z);

	void setPosition (Entity entity, ALfloat x, ALfloat y, ALfloat z);

	void setVelocity (ALfloat x, ALfloat y, ALfloat z);

	void setVelocity (Entity entity, ALfloat x, ALfloat y, ALfloat z);

	void setVolume (ALfloat volume);

	void setVolume (Entity entity, ALfloat volume);

	void setMute (bool mute);

	void setMute (Entity entity, bool mute);

	/// TODO unload buffer
	void unloadBufferDataPlsImplementThis();
};

}	// namespace Zen

#endif
