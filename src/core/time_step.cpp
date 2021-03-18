/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "time_step.h"

namespace Zen {
namespace Core {

TimeStep::TimeStep ()
{}

TimeStep::~TimeStep ()
{}

void TimeStep::start (std::function<void(Uint32, Uint32)> gameStep_)
{
	if (started)
		return;

	started = true;

	for (int i_ = 0; i_ < fpsSmoothingMax; i_++)
	{
		fpsHistory[i_] = 0;
	}

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

	//if (before < 0)
	//	before = 0;

	callback(now, delta);

	// Shift time value over
	lastTime = now;

	//frame++;
}

void TimeStep::shutdown ()
{
	quit = true;
}

}	// namespace Core
}	// namespace Zen
