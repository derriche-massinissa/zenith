/**
 * @file		time_step.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef TIME_STEP_H
#define TIME_STEP_H

#include <SDL2/SDL.h>
#include <functional>
#include "../messages.h"

namespace Zen
{
	/**
	 * Responsible for the core game loop.
	 *
	 * Invoked automatically by the main Game instance.
	 *
	 * @since 0.0.0
	 */
	class TimeStep
	{
	public:
		TimeStep();
		~TimeStep();

		/**
		 * Starts the Time Step running, if it is not already doing so.
		 *
		 * Called automatically by the Game Boot process.
		 *
		 * @since 0.0.0
		 * @param callback - The callback to be invoked each time the Time
		 * Step steps.
		 */
		void start (std::function<void(Uint32, Uint32)> gameStep);

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
		 * Stops the main loop, which will in turn shutdown the whole game.
		 *
		 * @since 0.0.0
		 */
		void shutdown ();

	private:
		/**
		 * The quit flag, the main game loop stops if this is set to true.
		 *
		 * @since 0.0.0
		 */
		bool quit;

		/**
		 * The callback to call for in each game loop, which is the step
		 * method of the Game instance.
		 *
		 * @since 0.0.0
		 */
		std::function<void(Uint32, Uint32)> callback;

		/**
		 * The amount of time since SDL started, in milliseconds.
		 *
		 * @since 0.0.0
		 */
		Uint32 now;

		/**
		 * The time the last step happened.
		 *
		 * The amount of time since SDL started, during the last step, in
		 * milliseconds.
		 *
		 * @since 0.0.0
		 */
		Uint32 lastTime;

		Uint32 delta;

		//Uint32 frame;

		//bool running;

		bool started;

		//float fps;
	};
}

#endif
