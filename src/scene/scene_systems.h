/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SCENES_SCENE_SYSTEMS_H
#define ZEN_SCENES_SCENE_SYSTEMS_H

#include <SDL2/SDL.h>
#include "../data.h"
#include "scene_settings.h"
#include "scene_config.h"
#include "../event/event_emitter.h"

// Forward declarations
#include "scene.fwd.h"
#include "../core/game.fwd.h"

namespace Zen {
namespace Scenes {

class SceneSystems
{
public:
	/**
	 * @since 0.0.0
	 */
	SceneSystems (Scene& scene_);

	/**
	 * @since 0.0.0
	 */
	~SceneSystems ();

	/**
	 * A reference to the scene these Systems belong to.
	 *
	 * @since 0.0.0
	 */
	Scene& scene;

	/**
	 * The Scene Settings.
	 *
	 * @since 0.0.0
	 */
	SceneSettings settings;

	/**
	 * The SceneSystems specific event manager.
	 *
	 * @since 0.0.0
	 */
	Events::EventEmitter events;

	/**
	 * This method is called only once by the Scene Manager when the Scene
	 * is instantiated.
	 * It is responsible for setting up all of the references and should
	 * never be called directly.
	 *
	 * @since 0.0.0
	 */
	void init ();

	/**
	 * A single game step. Called automatically by the Scene Manager as a result of a game loop call.
	 *
	 * @since 0.0.0
	 *
	 * @param time_ The time since the game started.
	 * @param delta_ The delta value since the last fram.
	 */
	void step (Uint32 time_, Uint32 delta_);

	/**
	 * Called automatically by the Scene Manager.
	 * Instructs the Scene to render itself via its Camera Manager.
	 *
	 * @since 0.0.0
	 *
	 */
	void render ();

	/**
	 * Force a sort of the display list on the next render.
	 *
	 * @since 0.0.0
	 *
	 */
	void queueDepthSort ();

	/**
	 * Immediately sorts the display list if the flag is set.
	 *
	 * @since 0.0.0
	 *
	 */
	void depthSort ();

	/**
	 * Pause this Scene.
	 * A paused scene still renders, it just doesn't run ANY of its update
	 * handlers or systems.
	 *
	 * @since 0.0.0
	 *
	 * @param data_ An event data that will be passed in the "pause" event.
	 */
	SceneSystems& pause (Data data_ = {});

	/**
	 * Resume this Scene from a paused state.
	 *
	 * @since 0.0.0
	 *
	 * @param data_ An event data that will be passed in the "resume" event.
	 */
	SceneSystems& resume (Data data_ = {});

	/**
	 * Send this Scene to sleep.
	 *
	 * A sleeping Scene doesn't run its update step or render anything, but
	 * it also isn't shut down or has any of its systems or children
	 * removed, meaning it can be re-activated at any point and will carry
	 * on from where it left off. It also keeps everything in memory and
	 * events and callbacks from other Scenes may still invoke changes
	 * within it, so be careful what is left active.
	 *
	 * @since 0.0.0
	 *
	 * @param data_ An event data that will be passed in the "sleep" event.
	 */
	SceneSystems& sleep (Data data_ = {});

	/**
	 * Wake up this Scene if it was previously asleep.
	 *
	 * @since 0.0.0
	 *
	 * @param data_ An event data that will be passed in the "wake" event.
	 */
	SceneSystems& wake (Data data_ = {});

    /**
     * Returns any data that was sent to this Scene by another Scene.
     *
     * The data is also passed to `Scene.init` and in various Scene events, but
     * you can access it at any point via this method.
     *
     * @since 0.0.0
     *
     * @return Data
     */
    Data getData ();

	/**
	 * Is this Scene sleeping?
	 *
	 * @since 0.0.0
	 *
	 * @return `true` if this Scene is asleep, otherwise `false`.
	 */
	bool isSleeping ();

	/**
	 * Is this Scene running?
	 *
	 * @since 0.0.0
	 *
	 * @return `true` if this Scene is running, otherwise `false`.
	 */
	bool isActive ();

	/**
	 * Is this Scene paused?
	 *
	 * @since 0.0.0
	 *
	 * @return `true` if this Scene is paused, otherwise `false`.
	 */
	bool isPaused ();

	/**
	 * Is this Scene currently transitioning out to, or in from another scene?
	 *
	 * @since 0.0.0
	 *
	 * @return `true` if this Scene is currently transitioning,
	 * otherwise `false`.
	 */
	bool isTransitioning ();

	/**
	 * Is this Scene currently transitioning out from itself to another Scene?
	 *
	 * @since 0.0.0
	 *
	 * @return `true` if this Scene is in transition to another Scene,
	 * otherwise `false`.
	 */
	bool isTransitionOut ();

	/**
	 * Is this Scene currently transitioning in from another Scene?
	 *
	 * @since 0.0.0
	 *
	 * @return `true` if this Scene is in transition from another Scene,
	 * otherwise `false`.
	 */
	bool isTransitionIn ();

	/**
	 * Is this Scene visible and rendering?
	 *
	 * @since 0.0.0
	 *
	 * @return `true` if this Scene is visible, otherwise `false`.
	 */
	bool isVisible ();

	/**
	 * Sets the visible state of this Scene.
	 *
	 * An invisible Scene will not render, but will still process
	 * updates.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ `true` to render this Scene, otherwise `false`.
	 *
	 * @return A reference to this Systems object.
	 */
	SceneSystems& setVisible (bool value_);

	/**
	 * Sets the active state of this Scene.
	 *
	 * An active Scene will run its core update loop.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ If `true` the Scene will be resumed, if previously paused. If 
	 * `false` it will be paused.
	 * @param data_ A data object that will be passed in the "resume" or
	 * "pause" events.
	 *
	 * @return A reference to this Systems object.
	 */
	SceneSystems& setActive (bool value_, Data data_);

	/**
	 * Start this Scene running and rendering.
	 * Called automatically by the Scene Manager.
	 *
	 * @since 0.0.0
	 *
	 */
	void start (Data data_ = {});

	/**
	 * Shutdown this Scene and send a shutdown event to all of its systems.
	 * A Scene that has been shutdown will not run its update loop or
	 * render, but it does not destroy its references. It is put into
	 * hibernation for later use.
	 * If you don't ever plan to use this Scene again, then it should be
	 * destroyed instead to free-up resources.
	 *
	 * @since 0.0.0
	 *
	 */
	void shutdown (Data data_ = {});
};

}	// namespace Scenes
}	// namespace Zen

// Declarations of the forward declared elements
//#include "scene.h"

#endif
