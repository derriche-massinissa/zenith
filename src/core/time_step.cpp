/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "time_step.h"

namespace Zen {
namespace Core {

void TimeStep::start (std::function<void(Uint32, Uint32)> gameStep_)
{
	if (started)
		return;

	started = true;

	callback = gameStep_;

	loop();
}

void TimeStep::loop ()
{
	// Main game loop
	while (!quit) {
		// Game step
		step();
	}
	// Clean up and close the program
}

void TimeStep::step ()
{
	now = SDL_GetTicks();

	delta = now - lastTime;

	// Frame rate
	if (now > nextFpsUpdate)
	{
		// Compute the new exponential moving average with an alpha of 0.25
		actualFps = 0.25 * framesThisSecond + 0.75 * actualFps;
		nextFpsUpdate = now + 1000;
		framesThisSecond = 0;
	}

	framesThisSecond++;

	callback(now, delta);

	// Shift time value over
	lastTime = now;

	frame++;
}

void TimeStep::tick ()
{
	step();
}

Uint32 TimeStep::getDuration ()
{
	return now / 1000;
}

Uint32 TimeStep::getDurationMS ()
{
	return now;
}

void TimeStep::shutdown ()
{
	quit = true;
}

}	// namespace Core
}	// namespace Zen
