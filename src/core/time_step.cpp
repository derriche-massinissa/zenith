/**
 * @file		time_step.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "time_step.h"

Zen::TimeStep::TimeStep ()
{
	quit = false;
	now = 0;
	lastTime = 0;
	delta = 0;
	//frame = 0;
	//fps = 0;
	//running = false;
	//started = false;

	messageNote("TimeStep constructed.");
}

Zen::TimeStep::~TimeStep ()
{
	messageNote("TimeStep destructed.");
}

void Zen::TimeStep::start (std::function<void(Uint32, Uint32)> gameStep)
{
	if (started)
		return;

	started = true;
	//running = true;

	callback = gameStep;

	loop();
}

void Zen::TimeStep::loop ()
{
	// Main game loop
	while (!quit) {
		// Game step
		step();
	}

	// Clean up and close the program
}

void Zen::TimeStep::step ()
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

void Zen::TimeStep::shutdown ()
{
	quit = true;
}
