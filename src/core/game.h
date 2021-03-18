/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAME_H
#define ZEN_GAME_H

#include <SDL2/SDL.h>
#include <functional>

#include "../defs.h"
#include "../const.h"
#include "config.h"
#include "../messages.h"
#include "../window/window.h"
//#include "debug_header.h"
#include "../event/event_emitter.h"
#include "time_step.h"
//#include "visibility_handler.h"
//#include "../device.h"

//#include "../animations/animation_manager.h"
//#include "../data/data_manager.h"
//#include "../input/input_manager.h"
#include "../scale/scale_manager.h"
#include "../scene/scene_manager.h"
#include "../textures/texture_manager.h"
//#include "../audio/audio_manager.h"

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
	 * The renderer is responsible for displaying contents on the screen.
	 *
	 * @since 0.0.0
	 */
	Renderer renderer;

	/**
	 * An Event Emitter which is used to broadcast game-level events from
	 * the global system.
	 *
	 * @since 0.0.0
	 */
	EventEmitter events;

	/**
	 * An instance of the Animation Manager.
	 *
	 * The Animation Manager is a system responsible for managing all
	 * animations used within the game.
	 *
	 * @since 0.0.0
	 */
	//AnimationManager anims(this);

	/**
	 * An instance of the Texture Manager.
	 *
	 * The Texture Manager is a system responsible for managing all textures
	 * used within the game.
	 *
	 * @since 0.0.0
	 */
	TextureManager textures;

	/**
	 * An instance of the Data Manager.
	 *
	 * The Data Manager is a system responsible for storing data in multiple
	 * objects for later use.
	 *
	 * @since 0.0.0
	 */
	//DataManager registry(this);

	/**
	 * An instance of the Input Manager.
	 *
	 * The Input Manager is a system responsible for user input events.
	 *
	 * @since 0.0.0
	 */
	//InputManager input(this, config);

	/**
	 * An instance of the Scene Manager.
	 *
	 * The Scene Manager is a system responsible for creating, modifying and
	 * updating the scenes of the game.
	 *
	 * @since 0.0.0
	 */
	SceneManager scene;

	/**
	 * A reference to the Device Inspector.
	 *
	 * Contains information about the device running this game, such as
	 * OS, browser vendor and feature support.
	 * Used by various systems to determine capabilities and
	 * code paths.
	 *
	 * @since 0.0.0
	 */
	//Device device;

	/**
	 * An instance of the Scale Manager.
	 *
	 * The Scale Manager is a system responsible for handling scaling of
	 * the game.
	 *
	 * @since 0.0.0
	 */
	ScaleManager scale;

	/**
	 * An instance of the base Audio Manager.
	 *
	 * The Audio Manager is a system responsible for the playback and
	 * updating of all audio in the game.
	 *
	 * @since 0.0.0
	 */
	//AudioManager audio(this);

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
	 * Marks the game for shutdown.
	 *
	 * Note that the Game will only stop in the next game step.
	 *
	 * @since 0.0.0
	 * @param data A data object.
	 */
	void shutdown (Data data_);

	/**
	 * The Game Configuration structure.
	 *
	 * @since 0.0.0
	 */
	GameConfig& config;

	/**
	 * A reference to the window object.
	 *
	 * @since 0.0.0
	 */
	Window window;

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
