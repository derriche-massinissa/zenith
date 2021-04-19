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

	double volume = 1.;

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

	void loop (Entity entity, bool loop);

	void removeAll ();

	void pauseAll ();

	void resumeAll ();

	void stopAll ();

	void onWindowBlur ();

	void onWindowFocus ();

	void update (Uint32 time, Uint32 delta);

	void forEachActiveSound ();

	void setPosition (Entity entity, float x, float y, float z);

	void setVelocity (Entity entity, float x, float y, float z);

	void setVolume (double volume);

	void mute ();

	void unmute ();

	/// TODO unload buffer
	void unloadBufferDataPlsImplementThis();
};

}	// namespace Zen

#endif
