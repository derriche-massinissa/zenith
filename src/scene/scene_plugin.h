/**
 * @file		scene_plugin.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef SCENE_PLUGIN_H
#define SCENE_PLUGIN_H

#include <SDL2/SDL.h>
#include <functional>
#include <string>
#include <memory>

// Forward declarations
#include "scene.fwd.h"
#include "scene_manager.fwd.h"
#include "scene_transition_config.h"

#include "../data.h"
#include "../math.h"
#include "../event/event_listener.h"

namespace Zen
{
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
	 * @since 0.0.0
	 */
	class ScenePlugin
	{
	public:
		ScenePlugin (Scene& s);
		~ScenePlugin ();

		/**
		 * A reference to the Scene that this Scene Plugin belongs to.
		 *
		 * @since 0.0.0
		 */
		Scene& scene;

		/**
		 * A reference to the Game's Scene Manager.
		 *
		 * @since 0.0.0
		 */
		SceneManager& manager;

		/**
		 * If this Scene is currently transitioning to another, this holds
		 * the current percentage of the transition progress,
		 * between 0 and 1.
		 *
		 * @since 0.0.0
		 */
		double transitionProgress;

		/**
		 * Transition elapsed timer.
		 *
		 * @since 0.0.0
		 */
		Uint32 _elapsed;

		/**
		 * The Scene this Scene is transitioning to.
		 *
		 * @since 0.0.0
		 */
		Scene *_target;

		/**
		 * Transition duration.
		 *
		 * @since 0.0.0
		 */
		Uint32 _duration;

		/**
		 * The update event listener, we store it to later remove it from
		 * the event.
		 *
		 * @since 0.0.0
		 */
		Listener *_updateListener;

		/**
		 * Transition callback.
		 *
		 * @since 0.0.0
		 */
		std::function<void(Data)> _onUpdate;

		/**
		 * Will this Scene sleep (true) after the transition, or stop
		 * (false).
		 *
		 * @since 0.0.0
		 */
		bool _willSleep;

		/**
		 * Will this Scene be removed from the Scene Manager after the
		 * transition completes?
		 *
		 * @since 0.0.0
		 */
		bool _willRemove;

		/**
		 * Shutdown this Scene and run the given one.
		 *
		 * This will happen at the next Scene Manager update, not
		 * immediately.
		 *
		 * @since 0.0.0
		 * @param key The key of the Scene to start.
		 * @param data The Scene data.
		 * @return A reference to this Scene Plugin instance.
		 */
		ScenePlugin& start (std::string key = "", Data data = {});

		/**
		 * Restarts this Scene.
		 *
		 * This will happen at the next Scene Manager update, not
		 * immediately.
		 *
		 * @since 0.0.0
		 * @return A reference to this Scene Plugin instance.
		 */
		ScenePlugin& restart (Data data = {});

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
		 * @param config The scene transition object.
		 * @return `true` if the transition started, otherwise `false`.
		 */
		bool transition (SceneTransitionConfig config);

		/**
		 * Checks to see if this Scene can transition to the target Scene
		 * or not.
		 *
		 * @since 0.0.0
		 * @param target A pointer to the scene to check.
		 * @return `true` if this Scene can transition, otherwise `false`.
		 */
		bool checkValidTransition (Scene* target);

		/**
		 * A single game step. This is only called if the parent Scene is
		 * transitioning out to another Scene.
		 *
		 * @since 0.0.0
		 * @param data A data object containing two integers:
		 * * time The time since SDL started, in ms.
		 * * delta The delta time since the last frame, in ms.
		 */
		void step (Data data);

		/**
		 * Called by `step` when the transition out of this Scene to another
		 * is over.
		 *
		 * @since 0.0.0
		 */
		void transitionComplete ();

		/**
		 * Add the Scene into the Scene Manager and start it if
		 * `autoStart` is `true` or the Scene config `active` property is set.
		 *
		 * @since 0.0.0
		 * @param key A unique key used to reference the Scene.
		 * @param sceneToAdd A unique pointer to a new Scene instance.
		 * @param autoStart If `true` the Scene will be started
		 * immediately after being added.
		 * @param data Optional data object.
		 * @return A reference to the added Scene.
		 */
		Scene& add (std::string key, std::unique_ptr<Scene> sceneToAdd, bool autoStart = false, Data data = {});

		/**
		 * Launch the given Scene and run it in parallel with this one.
		 *
		 * This will happen at the next Scene Manager update, not
		 * immediately.
		 *
		 * @since 0.0.0
		 * @param key The Scene to launch.
		 * @param data The Scene data.
		 * @return A reference to this Scene Plugin instance.
		 */
		ScenePlugin& launch (std::string key, Data data = {});

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
		 * @param key The Scene to run.
		 * @param data The Scene data.
		 * @return A reference to this Scene Plugin instance.
		 */
		ScenePlugin& run (std::string key, Data data = {});

		/**
		 * Pause the Scene, this stops the update step from happening but it
		 * still renders.
		 *
		 * This will happen at the next Scene Manager update, not
		 * immediately.
		 *
		 * @since 0.0.0
		 * @param key The Scene to pause.
		 * @param data The Scene data.
		 * @return A reference to this Scene Plugin instance.
		 */
		ScenePlugin& pause (std::string key = "", Data data = {});

		/**
		 * Resume the Scene, starting the update loop again.
		 *
		 * This will happen at the next Scene Manager update, not
		 * immediately.
		 *
		 * @since 0.0.0
		 * @param key The Scene to resume.
		 * @param data The Scene data.
		 * @return A reference to this Scene Plugin instance.
		 */
		ScenePlugin& resume (std::string key = "", Data data = {});

		/**
		 * Make the Scene sleep (no update, no render) but doesn't
		 * shutdown.
		 *
		 * This will happen at the next Scene Manager update, not
		 * immediately.
		 *
		 * @since 0.0.0
		 * @param key The Scene to put to sleep.
		 * @param data The Scene data.
		 * @return A reference to this Scene Plugin instance.
		 */
		ScenePlugin& sleep (std::string key = "", Data data = {});

		/**
		 * Make the Scene wake-up (starts update and render).
		 * shutdown.
		 *
		 * This will happen at the next Scene Manager update, not
		 * immediately.
		 *
		 * @since 0.0.0
		 * @param key The Scene to wake up.
		 * @param data The Scene data.
		 * @return A reference to this Scene Plugin instance.
		 */
		ScenePlugin& wake (std::string key = "", Data data = {});

		/**
		 * Make the Scene sleep then start the Scene given.
		 *
		 * This will happen at the next Scene Manager update, not
		 * immediately.
		 *
		 * @since 0.0.0
		 * @param key The Scene to start.
		 * @return A reference to this Scene Plugin instance.
		 */
		ScenePlugin& swap (std::string key);

		/**
		 * Shutdown the Scene, clearing display list, timers, etc.
		 *
		 * This will happen at the next Scene Manager update, not
		 * immediately.
		 *
		 * @since 0.0.0
		 * @param key The Scene to stop.
		 * @param data The Scene data.
		 * @return A reference to this Scene Plugin instance.
		 */
		ScenePlugin& stop (std::string key = "", Data data = {});

		/**
		 * Sets the visible state of this Scene.
		 *
		 * An invisible Scene will not render, but will still process
		 * updates.
		 *
		 * @since 0.0.0
		 * @param value `true` to render this Scene, otherwise `false`.
		 * @param key The Scene to set the active state of.
		 * @return A reference to this Scene Plugin object.
		 */
		ScenePlugin& setVisible (bool value, std::string key = "");

		/**
		 * Sets the active state of this Scene.
		 *
		 * An active Scene will run its core update loop.
		 *
		 * @since 0.0.0
		 * @param value If `true` the Scene will be resumed, if
		 * previously paused. If `false` it will be paused.
		 * @param key The Scene to set the active state of.
		 * @param data A data object that will be passed in the "resume" or
		 * "pause" events.
		 * @return A reference to this Scene Plugin object.
		 */
		ScenePlugin& setActive (bool value, std::string key = "", Data data = {});

		/**
		 * Is the given Scene sleeping?
		 *
		 * @since 0.0.0
		 * @param key The Scene to check.
		 * @return `true` if this Scene is asleep, otherwise `false`.
		 */
		bool isSleeping (std::string key = "");

		/**
		 * Is the given Scene running?
		 *
		 * @since 0.0.0
		 * @param key The Scene to check.
		 * @return `true` if this Scene is running, otherwise `false`.
		 */
		bool isActive (std::string key = "");

		/**
		 * Is the given Scene paused?
		 *
		 * @since 0.0.0
		 * @param key The Scene to check.
		 * @return `true` if this Scene is paused, otherwise `false`.
		 */
		bool isPaused (std::string key = "");

		/**
		 * Is the given Scene visible and rendering?
		 *
		 * @since 0.0.0
		 * @param key The Scene to check.
		 * @return `true` if this Scene is visible, otherwise `false`.
		 */
		bool isVisible (std::string key = "");

		/**
		 * Swap the position of two scenes in the Scenes list.
		 *
		 * This controls the order in which they are rendered and
		 * updated.
		 *
		 * @since 0.0.0
		 * @param keyA The first Scene to swap.
		 * @param keyB The second Scene to swap. If none is given, it
		 * defaults to this Scene.
		 * @return A reference to this Scene Plugin object.
		 */
		ScenePlugin& swapPosition (std::string keyA, std::string keyB = "");

		/**
		 * Swaps the position of two scenes in the Scenes list, so that Scene B
		 * is directly above Scene A.
		 *
		 * This controls the order in which they are rendered and updated.
		 *
		 * @since 0.0.0
		 * @param keyA The Scene that Scene B will be moved to be above.
		 * @param keyB The Scene to be moved. If none is given, it
		 * defaults to this Scene.
		 * @return A reference to this Scene Plugin object.
		 */
		ScenePlugin& moveAbove (std::string keyA, std::string keyB = "");

		/**
		 * Swaps the position of two scenes in the Scenes list, so that Scene B
		 * is directly below Scene A.
		 *
		 * This controls the order in which they are rendered and updated.
		 *
		 * @since 0.0.0
		 * @param keyA The Scene that Scene B will be moved to be below.
		 * @param keyB The Scene to be moved. If none is given, it
		 * defaults to this Scene.
		 * @return A reference to this Scene Plugin object.
		 */
		ScenePlugin& moveBelow (std::string keyA, std::string keyB = "");

		/**
		 * Remove a Scene from the Scene Manager.
		 *
		 * If the Scene Manager is processing the Scenes when this is called,
		 * it will queue the operation for the next update sequence.
		 *
		 * @since 0.0.0
		 * @param key The Scene to remove.
		 * @return A reference to this Scene Plugin object.
		 */
		ScenePlugin& remove (std::string key = "");

		/**
		 * Move a Scene up one position in the Scene list.
		 *
		 * @since 0.0.0
		 * @param key The Scene to move.
		 * @return A reference to this Scene Plugin object.
		 */
		ScenePlugin& moveUp (std::string key = "");

		/**
		 * Move a Scene down one position in the Scene list.
		 *
		 * @since 0.0.0
		 * @param key The Scene to move.
		 * @return A reference to this Scene Plugin object.
		 */
		ScenePlugin& moveDown (std::string key = "");

		/**
		 * Brings a Scene to the top of the Scenes list.
		 *
		 * This means it will render above all other Scenes.
		 *
		 * @since 0.0.0
		 * @param key The Scene to move.
		 * @return A reference to this Scene Plugin object.
		 */
		ScenePlugin& bringToTop (std::string key = "");

		/**
		 * Sends a Scene to the back of the Scenes list.
		 *
		 * This means it will render below all other Scenes.
		 *
		 * @since 0.0.0
		 * @param key The Scene to move.
		 * @return A reference to this Scene Plugin object.
		 */
		ScenePlugin& sendToBack (std::string key = "");

		/**
		 * Retrieve a Scene.
		 *
		 * @since 0.0.0
		 * @param key The Scene to retrieve.
		 * @return The Scene.
		 */
		Scene& get (std::string key);

		/**
		 * Retrieves the numeric index of a Scene in the Scenes list.
		 *
		 * @since 0.0.0
		 * @param key The Scene to get the index of.
		 * @return The index of the Scene.
		 */
		int getIndex (std::string key = "");

		/**
		 * The Scene that owns this plugin is shutting down.
		 *
		 * We need to kill and reset all internal properties as well as stop
		 * listening to Scene events.
		 *
		 * @since 0.0.0
		 */
		void shutdown ();

	private:
	};
}

// Declarations of the forward declared elements
#include "scene.h"
#include "scene_manager.h"

#endif
