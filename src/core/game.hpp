/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAME_HPP
#define ZEN_GAME_HPP

#include "config.fwd.hpp"
#include "time_step.hpp"

/**
 * @namespace Zen
 *
 * The Zen namespace is the namespace of the whole `Zenith` framework.
 *
 * @since 0.0.0
 */
namespace Zen {

/**
 * The Zen::Game instance is the core part of the entire game.
 *
 * It is responsible for handling the boot process, configuration, creating a 
 * window and a renderer through SDL2, and setting up all the global subsystems.
 * Once that is complete, it will start the Scene Manager and then begin
 * the main game loop.
 *
 * @class Game
 * @since 0.0.0
 */
class Game
{
public:
	/**
	 * @since 0.0.0
	 *
	 * @param config_ A reference to the game configuration object.
	 */
	Game (GameConfig& config_);

	/**
	 * Destroys this Game instance, all systems, all sub-systems, all
	 * Scenes, and stops SDL, ending the program entirely.
	 *
	 * If you need to react to the game's eventual destruction, listen
	 * for the `SYS_DESTROY` event.
	 *
	 * @since 0.0.0
	 */
	~Game ();

	/**
	 * The Game Configuration structure.
	 *
	 * @since 0.0.0
	 */
	GameConfig& config;

	/**
	 * An instance of the Time Step.
	 *
	 * The Time Step is a system responsible for setting-up and
	 * responding to the main SDL2 loop and calculating delta values. It
	 * then automatically calls the game step.
	 *
	 * @since 0.0.0
	 */
	TimeStep loop;

	/**
	 * This method starts the game.
	 *
	 * This method is responsible for creating a window and its renderer,
	 * displaying the Debug Header, and emitting the "boot" event. It
	 * listens for a "ready" event from the base systems and once
	 * received it will call the "start" method.
	 *
	 * @since 0.0.0
	 */
	void boot ();

	/**
	 * The game is now ready to start the main loop running.
	 * It will also enable the Visibility Handler.
	 *
	 * @since 0.0.0
	 */
	void start ();

	/**
	 * The main Game Step, called automatically by the Time Step, once per SDL loop.
	 *
	 * The step will update the managers first, then proceeds to update each Scene 
	 * in turn, via the Scene Manager. It will then render each Scene in turn, via 
	 * the Renderer. This process emits "prerender" and "postrender" events.
	 *
	 * @since 0.0.0
	 * @param time The total time since SDL was initialized in
	 * milliseconds (SDL_GetTicks).
	 * @param delta The delta time in ms since the last frame.
	 */
	void step (Uint32 time_, Uint32 delta_);

	/**
	 * A special version of the Game Step for the Headless renderer
	 * only.
	 *
	 * The main Game Step, called automatically by the Time Step, once per SDL loop.
	 *
	 * The step will update the managers first, then proceeds to update each Scene in turn, via the Scene Manager. It will then render each Scene in turn, via the Renderer. This process emits "Prerender" and "Postrender" events.
	 *
	 * @since 0.0.0
	 * @param time The total time since SDL was initialized in
	 * milliseconds (SDL_GetTicks).
	 * @param delta The delta time in ms since the last frame.
	 */
	void headlessStep (Uint32 time_, Uint32 delta_);

	/**
	 * Poll all queued SDL events and handle each of them accordingly.
	 *
	 * @since 0.0.0
	 */
	void handleSDLEvents ();

	/**
	 * Returns the current game frame.
	 *
	 * When the game starts running, the frame is incremented every
	 * SDL loop.
	 *
	 * @since 0.0.0
	 * @return The current game frame.
	 */
	Uint32 getFrame ();

	/**
	 * Returns the time that the current game step started at.
	 *
	 * @since 0.0.0
	 * @return The current game timestamp.
	 */
	Uint32 getTime ();

	/**
	 * Runs when the game window is minimized.
	 *
	 * @since 0.0.0
	 */
	void onMinimize ();

	/**
	 * Runs when the game window is restored from being minimized.
	 *
	 * @since 0.0.0
	 */
	void onRestore ();

	/**
	 * Marks the game for shutdown.
	 *
	 * Note that the Game will only stop in the next game step.
	 *
	 * @since 0.0.0
	 * @param data A data object.
	 */
	void shutdown ();

	/**
	 * A flag indicating if the Game instance is marked for shutdown in the next
	 * frame. This will stop the game loop.
	 *
	 * @since 0.0.0
	 */
	bool pendingShutdown = false;

	/**
	 * A flag indicating when this Game instance has finished its boot
	 * process.
	 *
	 * @since 0.0.0
	 */
	bool isBooted = false;

	/**
	 * A flag indicating if this Game instance is currently running its game
	 * step or not.
	 *
	 * @since 0.0.0
	 */
	bool isRunning = false;

	/**
	 * Is the game window visible (Not minimized)?
	 */
	bool isVisible = true;

	/**
	 * The delta time between each game step when the window isn't visible, as it
	 * isn't rendering and so not vsynced.
	 * It is recommended to never set it to zero.
	 *
	 * @since 0.0.0
	 */
	unsigned int hiddenDelta = 100;

	/**
	 * Shuts down the Game by stopping the main game loop.
	 *
	 * Called from the Game Game::step when Game::pendingShutdown
	 * is true, after calling Game::shutdown.
	 *
	 * @since 0.0.0
	 */
	void runShutdown ();
};

}	// namespace Zen

#endif
