/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_AUDIO_AUDIO_HPP
#define ZEN_AUDIO_AUDIO_HPP

#include <string>

namespace Zen {

// FIXME
struct AudioConfig {};
struct AudioMarker {};

class Audio
{
public:
	Audio (std::string key, AudioConfig config);

	std::string key;

	bool isPlaying = false;

	bool isPaused = false;

	Mix_Chunk* source = nullptr;

	Mix_Chunk* loopSource = nullptr;

	bool mute = false;

	double volume = 1.;

	int playTime = 0;

	int startTime = 0;

	int loopTime = 0;

	void* rateUpdates[10]; // ???

	bool hasEnded = false;

	bool hasLooped = false;

	unsigned int duration = 0;

	bool play (AudioConfig config = {});

	bool play (AudioMarker marker = {});

	bool pause ();

	bool resume ();

	bool stop ();

private:
	void createAndStartBufferSource ();

	void createAndStartLoopBufferSource ();
};

}

#endif
