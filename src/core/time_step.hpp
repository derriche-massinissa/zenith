/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_CORE_TIME_STEP_HPP
#define ZEN_CORE_TIME_STEP_HPP

#include <SDL2/SDL.h>
#include <functional>

#include "../utils/messages.hpp"

namespace Zen {

/**
 * Responsible for the core game loop.
 *
 * Invoked automatically by the main Game instance.
 *
 * @class TimeStep
 * @since 0.0.0
 */
class TimeStep
{
public:
	/**
	 * The quit flag, the main game loop stops if this is set to true.
	 *
	 * @since 0.0.0
	 */
	bool quit = false;

	/**
	 * An exponential moving average of the frames per second.
	 *
	 * @since 0.0.0
	 */
	double actualFps = 60.0;

	/**
	 * The time at which the next fps rate update will take place.
	 * When an fps update happens, the `framesThisSecond` value is reset.
	 * 
	 * @since 0.0.0
	 */
	Uint32 nextFpsUpdate = 0;

	/**
	 * The number of frames processed this second.
	 *
	 * @since 0.0.0
	 */
	Uint32 framesThisSecond = 0;

	/**
	 * The callback to call for in each game loop, which is the step
	 * method of the Game instance.
	 *
	 * @since 0.0.0
	 */
	std::function<void(Uint32, Uint32)> callback = nullptr;

	/**
	 * The amount of time since SDL started, in milliseconds.
	 *
	 * @since 0.0.0
	 */
	Uint32 now = 0;

	/**
	 * The time the previous step happened.
	 *
	 * The amount of time since SDL started, during the last step, in
	 * milliseconds.
	 *
	 * @since 0.0.0
	 */
	Uint32 lastTime = 0;

	/**
	 * The current frame the game is on. This counter is incremented once every 
	 * game step, regardless of how much time has passed.
	 *
	 * @since 0.0.0
	 */
	int frame = 0;

	/**
	 * The delta time, in ms, since the last game step.
	 *
	 * @since 0.0.0
	 */
	Uint32 delta = 0;

	/**
	 * The number of frames that the fps is set to after the program panics over
	 * FPS rate, usually at the start of the program.
	 *
	 * @since 0.0.0
	 */
	double panicMax = 120.0;

	/**
	 * Has this game loop started?
	 *
	 * This is here to avoid any infinite game loop recursion.
	 *
	 * @since 0.0.0
	 */
	bool started = false;

	/**
	 * Starts the Time Step running, if it is not already doing so.
	 *
	 * Called automatically by the Game Boot process.
	 *
	 * @since 0.0.0
	 * @param callback - The callback to be invoked each time the Time
	 * Step steps.
	 */
	void start (std::function<void(Uint32, Uint32)> gameStep_);

	/**
	 * The main game loop.
	 *
	 * Once the program gets here, it will keep looping until the user
	 * requests to quit.
	 * Note that this function (as well as this whole class) is only
	 * responsible for the game step and game loop. Everything else, like
	 * generating graphics, managing textures, and handling events is done
	 * elsewhere in different specialized classes.
	 *
	 * @since 0.0.0
	 */
	void loop ();

	/**
	 * A single Time Step or tick, where the game step is called.
	 *
	 * @since 0.0.0
	 */
	void step ();

	/**
	 * Manually calls TimeStep::step.
	 *
	 * @since 0.0.0
	 */
	void tick ();

	/**
	 * Gets the duration which the game has been running, in seconds.
	 *
	 * @since 0.0.0
	 *
	 * @return The duration in seconds.
	 */
	Uint32 getDuration ();

	/**
	 * Gets the duration which the game has been running, in milliseconds.
	 *
	 * @since 0.0.0
	 *
	 * @return The duration in ms.
	 */
	Uint32 getDurationMS ();

	/**
	 * Stops the main loop, which will in turn shutdown the whole game.
	 *
	 * @since 0.0.0
	 */
	void shutdown ();
};

}	// namespace Zen

#endif
