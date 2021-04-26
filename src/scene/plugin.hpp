/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SCENES_SCENE_PLUGIN_HPP
#define ZEN_SCENES_SCENE_PLUGIN_HPP

#include <SDL2/SDL.h>
#include <functional>
#include <string>
#include <memory>

#include "../data.h"
#include "../event/event_listener.hpp"
#include "transition_config.hpp"

#include "scene.fwd.hpp"
#include "systems.fwd.hpp"

namespace Zen {

/**
 * The Scene Plugin is the main interface to the Scene Manager and allows
 * you to control any Scene running in your game. You should always use
 * this plugin. By default, it is mapped to the Scene property `scene`.
 * Meaning from within a Scene, you can call methods such as
 * `scene.start()`.
 *
 * Note that nearly all methods in this class are run on a queue-basis and
 * not immediately. For example, calling `scene.launch("SceneB")`
 * will try to launch SceneB when the Scene Manager next updates, which
 * is at the start of the game step. All operations are queued and run in
 * the order in which they are invoked here.
 *
 * @class ScenePlugin
 * @since 0.0.0
 */
class ScenePlugin
{
public:
	/**
	 * @since 0.0.0
	 */
	ScenePlugin (Scene* scene_);

	/**
	 * A reference to the Scene that this ScenePlugin belongs to.
	 *
	 * @since 0.0.0
	 */
	Scene* scene;

	/**
	 * The SceneSystems instance of the Scene that this ScenePlugin belongs to.
	 *
	 * @since 0.0.0
	 */
	SceneSystems* systems;

	/**
	 * The unique key of the Scene that ownd this ScenePlugin.
	 *
	 * @since 0.0.0
	 */
	std::string key = "";

	/**
	 * If this Scene is currently transitioning to another, this holds
	 * the current percentage of the transition progress, between 0 and 1.
	 *
	 * @since 0.0.0
	 */
	double transitionProgress = 0.0;

	/**
	 * Transition elapsed timer.
	 *
	 * @since 0.0.0
	 */
	Uint32 transitionElapsed = 0;

	/**
	 * The Scene this Scene is transitioning to.
	 *
	 * @since 0.0.0
	 */
	Scene *transitionTarget = nullptr;

	/**
	 * Transition duration.
	 *
	 * @since 0.0.0
	 */
	Uint32 transitionDuration = 0;

	/**
	 * Transition callback.
	 *
	 * @since 0.0.0
	 */
	std::function<void(double)> transitionCallback = nullptr;

	/**
	 * Will this Scene sleep (true) after the transition, or stop (false).
	 *
	 * @since 0.0.0
	 */
	bool transitionWillSleep = false;

	/**
	 * Will this Scene be removed from the Scene Manager after the
	 * transition completes?
	 *
	 * @since 0.0.0
	 */
	bool transitionWillRemove = false;

	ListenerBase* lUpdate = nullptr;

    /**
     * This method is called automatically by the Scene when it is starting up.
     * It is responsible for creating local systems, properties and listening for 
	 * Scene events.
     * Do not invoke it directly.
     *
     * @since 0.0.0
     */
	void pluginStart ();

	/**
	 * Shutdown this Scene and run the given one.
	 *
	 * This will happen at the next Scene Manager update, not immediately.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The key of the Scene to start.
	 * @param data_ The Scene data.
	 *
	 * @return A reference to this Scene Plugin instance.
	 */
	ScenePlugin& start (std::string key_ = "", Data data_ = {});

	/**
	 * Restarts this Scene.
	 *
	 * This will happen at the next Scene Manager update, not
	 * immediately.
	 *
	 * @since 0.0.0
	 *
	 *
	 * @return A reference to this Scene Plugin instance.
	 */
	ScenePlugin& restart (Data data_ = {});

	/**
	 * This will start a transition from the current Scene to the target
	 * Scene given.
	 *
	 * The transition will last for the duration specified in
	 * milliseconds.
	 *
	 * You can have the target Scene moved above or below this one in
	 * the display list.
	 *
	 * You can specify an update callback. This callback will be
	 * invoked EVERY FRAME for the duration of the transition.
	 *
	 * This Scene can either be sent to sleep at the end of the
	 * transition, or stopped. The default is to stop.
	 *
	 * There are also 5 transition related events: This Scene will emit
	 * `SYS_TRANSITION_OUT` when the transition begins, which is
	 * typically the frame after calling this method.
	 *
	 * The target Scene will emit the event `SYS_TRANSITION_INIT` when
	 * that Scene's `init` method is called.
	 * It will the emit the event `SYS_TRANSITION_START` when its
	 * `create` method is called.
	 * If the Scene was sleeping and has been woken up, it will emit the
	 * event `SYS_TRANSITION_WAKE` instead of the two, as the 
	 * Scene's `init` and `create` methods are not invoked when woken 
	 * up.
	 *
	 * When the duration of the transition has elapsed it will emit the
	 * event `SYS_TRANSITION_COMPLETE`.
	 * These events are cleared of all listeners when the Scene shuts
	 * down, but not if it is sent to sleep.
	 *
	 * It's important to understand that the duration of the
	 * transition begins the moment you call this method.
	 * If the Scene you are transitioning to includes delayed
	 * processes, such as waiting for files to load, the time still counts
	 * down even while that is happening. If the game itself pauses, or
	 * something else causes this Scene's update loop to stop, then the
	 * transition will also pause for that duration.
	 * There are checks in place to prevent you accidentally stopping
	 * a transitioning Scene but if you've got code to override this
	 * understand that until the target Scene completes it might never
	 * be unlocked for input events.
	 *
	 * @since 0.0.0
	 *
	 * @param config_ The scene transition object.
	 *
	 * @return `true` if the transition started, otherwise `false`.
	 */
	bool transition (SceneTransitionConfig config_);

	/**
	 * Checks to see if this Scene can transition to the target Scene
	 * or not.
	 *
	 * @since 0.0.0
	 *
	 * @param target_ A pointer to the scene to check.
	 *
	 * @return `true` if this Scene can transition, otherwise `false`.
	 */
	bool checkValidTransition (Scene* target_);

	/**
	 * A single game step. This is only called if the parent Scene is
	 * transitioning out to another Scene.
	 *
	 * @since 0.0.0
	 *
	 * @param time_ The current time since SDL started.
	 * @param delta_ The delta time in ms since the last step.
	 */
	void step (Uint32 time_, Uint32 delta_);

	/**
	 * Called by `step` when the transition out of this Scene to another is over.
	 *
	 * @since 0.0.0
	 */
	void transitionComplete ();

	/**
	 * Add the Scene into the Scene Manager and start it if
	 * `autoStart` is `true` or the Scene config `active` property is set.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ A unique key used to reference the Scene.
	 * @param sceneToAdd_ A unique pointer to a new Scene instance.
	 * @param autoStart_ If `true` the Scene will be started
	 * immediately after being added.
	 * @param data_ Optional data object.
	 *
	 * @return A pointer to the added Scene if it was added immediately,
	 * otherwise `nullptr`.
	 */
	Scene& add (
			std::string key_,
			std::unique_ptr<Scene> sceneToAdd_,
			bool autoStart_ = false,
			Data data_ = {});

	/**
	 * Launch the given Scene and run it in parallel with this one.
	 *
	 * This will happen at the next Scene Manager update, not
	 * immediately.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to launch.
	 * @param data_ The Scene data.
	 *
	 * @return A reference to this Scene Plugin instance.
	 */
	ScenePlugin& launch (std::string key_, Data data_ = {});

	/**
	 * Runs the given Scene, but does not change the state of this Scene.
	 *
	 * This will happen at the next Scene Manager update, not
	 * immediately.
	 *
	 * If the given Scene is paused, it will resume it. If sleeping, it
	 * will wake it.
	 * If not running at all, it will be started.
	 *
	 * Use this if you wish to open a modal Scene by calling `pause` on the
	 * current Scene, then `run` on the modal Scene.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to run.
	 * @param data_ The Scene data.
	 *
	 * @return A reference to this Scene Plugin instance.
	 */
	ScenePlugin& run (std::string key_, Data data_ = {});

	/**
	 * Pause the Scene, this stops the update step from happening but it
	 * still renders.
	 *
	 * This will happen at the next Scene Manager update, not
	 * immediately.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to pause.
	 * @param data_ The Scene data.
	 *
	 * @return A reference to this Scene Plugin instance.
	 */
	ScenePlugin& pause (std::string key_ = "", Data data_ = {});

	/**
	 * Resume the Scene, starting the update loop again.
	 *
	 * This will happen at the next Scene Manager update, not
	 * immediately.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to resume.
	 * @param data_ The Scene data.
	 *
	 * @return A reference to this Scene Plugin instance.
	 */
	ScenePlugin& resume (std::string key_ = "", Data data_ = {});

	/**
	 * Make the Scene sleep (no update, no render) but doesn't
	 * shutdown.
	 *
	 * This will happen at the next Scene Manager update, not
	 * immediately.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to put to sleep.
	 * @param data_ The Scene data.
	 *
	 * @return A reference to this Scene Plugin instance.
	 */
	ScenePlugin& sleep (std::string key_ = "", Data data_ = {});

	/**
	 * Make the Scene wake-up (starts update and render).
	 * shutdown.
	 *
	 * This will happen at the next Scene Manager update, not
	 * immediately.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to wake up.
	 * @param data_ The Scene data.
	 *
	 * @return A reference to this Scene Plugin instance.
	 */
	ScenePlugin& wake (std::string key_ = "", Data data_ = {});

	/**
	 * Make the Scene sleep then start the Scene given.
	 *
	 * This will happen at the next Scene Manager update, not
	 * immediately.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to start.
	 *
	 * @return A reference to this Scene Plugin instance.
	 */
	ScenePlugin& swap (std::string key_);

	/**
	 * Shutdown the Scene, clearing display list, timers, etc.
	 *
	 * This will happen at the next Scene Manager update, not
	 * immediately.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to stop.
	 * @param data_ The Scene data.
	 *
	 * @return A reference to this Scene Plugin instance.
	 */
	ScenePlugin& stop (std::string key_ = "", Data data_ = {});

	/**
	 * Sets the active state of this Scene.
	 *
	 * An active Scene will run its core update loop.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ If `true` the Scene will be resumed, if
	 * previously paused. If `false` it will be paused.
	 * @param key_ The Scene to set the active state of.
	 * @param data_ A data object that will be passed in the "resume" or
	 * "pause" events.
	 *
	 * @return A reference to this Scene Plugin object.
	 */
	ScenePlugin& setActive (bool value_, std::string key_ = "", Data data_ = {});

	/**
	 * Sets the visible state of this Scene.
	 *
	 * An invisible Scene will not render, but will still process
	 * updates.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ `true` to render this Scene, otherwise `false`.
	 * @param key_ The Scene to set the active state of.
	 *
	 * @return A reference to this Scene Plugin object.
	 */
	ScenePlugin& setVisible (bool value_, std::string key_ = "");

	/**
	 * Is the given Scene sleeping?
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to check.
	 *
	 * @return `true` if this Scene is asleep, otherwise `false`.
	 */
	bool isSleeping (std::string key_ = "");

	/**
	 * Is the given Scene running?
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to check.
	 *
	 * @return `true` if this Scene is running, otherwise `false`.
	 */
	bool isActive (std::string key_ = "");

	/**
	 * Is the given Scene paused?
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to check.
	 *
	 * @return `true` if this Scene is paused, otherwise `false`.
	 */
	bool isPaused (std::string key_ = "");

	/**
	 * Is the given Scene visible and rendering?
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to check.
	 *
	 * @return `true` if this Scene is visible, otherwise `false`.
	 */
	bool isVisible (std::string key_ = "");

	/**
	 * Swap the position of two scenes in the Scenes list.
	 *
	 * This controls the order in which they are rendered and
	 * updated.
	 *
	 * @since 0.0.0
	 *
	 * @param keyA_ The first Scene to swap.
	 * @param keyB_ The second Scene to swap. If none is given, it
	 * defaults to this Scene.
	 *
	 * @return A reference to this Scene Plugin object.
	 */
	ScenePlugin& swapPosition (std::string keyA_, std::string keyB_ = "");

	/**
	 * Swaps the position of two scenes in the Scenes list, so that Scene B
	 * is directly above Scene A.
	 *
	 * This controls the order in which they are rendered and updated.
	 *
	 * @since 0.0.0
	 *
	 * @param keyA_ The Scene that Scene B will be moved to be above.
	 * @param keyB_ The Scene to be moved. If none is given, it
	 * defaults to this Scene.
	 *
	 * @return A reference to this Scene Plugin object.
	 */
	ScenePlugin& moveAbove (std::string keyA_, std::string keyB_ = "");

	/**
	 * Swaps the position of two scenes in the Scenes list, so that Scene B
	 * is directly below Scene A.
	 *
	 * This controls the order in which they are rendered and updated.
	 *
	 * @since 0.0.0
	 *
	 * @param keyA_ The Scene that Scene B will be moved to be below.
	 * @param keyB_ The Scene to be moved. If none is given, it
	 * defaults to this Scene.
	 *
	 * @return A reference to this Scene Plugin object.
	 */
	ScenePlugin& moveBelow (std::string keyA_, std::string keyB_ = "");

	/**
	 * Remove a Scene from the Scene Manager.
	 *
	 * If the Scene Manager is processing the Scenes when this is called,
	 * it will queue the operation for the next update sequence.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to remove.
	 *
	 * @return A reference to this Scene Plugin object.
	 */
	ScenePlugin& remove (std::string key_ = "");

	/**
	 * Move a Scene up one position in the Scene list.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to move.
	 *
	 * @return A reference to this Scene Plugin object.
	 */
	ScenePlugin& moveUp (std::string key_ = "");

	/**
	 * Move a Scene down one position in the Scene list.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to move.
	 *
	 * @return A reference to this Scene Plugin object.
	 */
	ScenePlugin& moveDown (std::string key_ = "");

	/**
	 * Brings a Scene to the top of the Scenes list.
	 *
	 * This means it will render above all other Scenes.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to move.
	 *
	 * @return A reference to this Scene Plugin object.
	 */
	ScenePlugin& bringToTop (std::string key_ = "");

	/**
	 * Sends a Scene to the back of the Scenes list.
	 *
	 * This means it will render below all other Scenes.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to move.
	 *
	 * @return A reference to this Scene Plugin object.
	 */
	ScenePlugin& sendToBack (std::string key_ = "");

	/**
	 * Retrieve a Scene.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to retrieve.
	 *
	 * @return The Scene.
	 */
	Scene& get (std::string key_);

	/**
	 * Retrieves the numeric index of a Scene in the Scenes list.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The Scene to get the index of.
	 *
	 * @return The index of the Scene.
	 */
	int getIndex (std::string key_ = "");
};


}	// namespace Zen

#endif
