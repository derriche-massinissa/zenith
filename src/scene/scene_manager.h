/**
 * @file		scene_manager.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>

// Forward declarations
#include "../core/game.fwd.h"

#include "../messages.h"
#include "../const.h"
#include "../data.h"
#include "scene.h"
#include "scene_config.h"
#include "scene_settings_config.h"

namespace Zen
{
	/**
	 * The operations on Scenes to be queued if the Manager is busy when the
	 * operation comes in.
	 *
	 * @since 0.0.0
	 */
	struct SceneOperation
	{
		std::string operation;
		std::string keyA;
		std::string keyB;
		Data data;

		SceneOperation (std::string o = "", std::string a = "", std::string b = "", Data d = {})
			: operation(o), keyA(a), keyB(b), data(d)
		{}
	};

	/**
	 * @class SceneManager
	 * The SceneManager.
	 *
	 * The SceneManager is a Game level system, responsible for creating,
	 * processing and updating all of the Scenes in a Game instance.
	 *
	 * @since 0.0.0
	 */
	class SceneManager
	{
	public:
		/**
		 * @since 0.0.0
		 * @param g A reference to the Game instance this SceneManager belongs
		 * to.
		 * @param sceneFactory A vector of functors that build and return
		 * instances of user defined scene classes (That inherit from Zen::Scene)
		 */
		SceneManager (
			Game& g,
			std::queue<std::function<std::unique_ptr<Scene>(Game&)>>& sceneFactory);

		/**
		 * @since 0.0.0
		 */
		~SceneManager ();

		/**
		 * Process the Scene operations queue.
		 *
		 * @since 0.0.0
		 */
		void processQueue ();

		/**
		 * Adds a new Scene into the SceneManager.
		 * You must give each Scene a unique key by which you'll identify it.
		 *
		 * @since 0.0.0
		 * @param key A unique key used to reference the Scene.
		 * @param scene A pointer to the Scene instance
		 * @param autoStart If `true` the Scene will be started immediately
		 * after being added.
		 * @param data Optional data object. This will be set as
		 * `Scene.sys.settings.data` and passed to `Scene.init` and
		 * `Scene.create`;
		 * @return A pointer to the added Scene if it was added immediately,
		 * otherwise `nullptr`.
		 */
		Scene* add (
			std::string key,
			std::unique_ptr<Scene> scene,
			bool autoStart = false,
			Data data = {});

		/**
		 * Removes a Scene from the SceneManager.
		 *
		 * The Scene is removed from the local scenes vector, it's key is
		 * cleared from the keys cache.
		 *
		 * If the SceneManager is processing the Scenes when this method is
		 * called, it will queue the operation for the next update sequence.
		 *
		 * @since 0.0.0
		 * @param key A unique key used to reference the Scene.
		 * @return A reference to this SceneManager instance.
		 */
		SceneManager& remove (std::string key);

		/**
		 * Updates the scenes.
		 *
		 * @since 0.0.0
		 * @param time Time elapsed since the game started.
		 * @param delta Delta time from the last update.
		 */
		void update (Uint32 time, Uint32 delta);

		/**
		 * Renders the Scenes.
		 *
		 * @since 0.0.0
		 */
		void render ();

		/**
		 * Returns a vector of pointers to all the current Scenes being managed
		 * by this SceneManager.
		 *
		 * You can filter the output by the active state of the Scene and choose
		 * to have the vector returned in normal or reversed order.
		 *
		 * @since 0.0.0
		 * @param isActive Only include Scenes that are currently active.
		 * @param inReverse Return the vector of Scenes in reverse.
		 * @return A vector containing pointers to all the Scenes in this
		 * SceneManager.
		 */
		std::vector<Scene*> getScenes (
				bool isActive = true,
				bool inReverse = false);

		/**
		 * Retrieves a Scene.
		 *
		 * @since 0.0.0
		 * @param key The key of the scene to retrieve.
		 * @return The scene.
		 */
		Scene* getScene (std::string key);

		/**
		 * Determines whether a Scene is running.
		 *
		 * @since 0.0.0
		 * @param key The key of the scene to check.
		 * @return Whether the Scene is running.
		 */
		bool isActive (std::string key);

		/**
		 * Determines whether a Scene is paused.
		 *
		 * @since 0.0.0
		 * @param key The key of the scene to check.
		 * @return Whether the Scene is paused.
		 */
		bool isPaused (std::string key);

		/**
		 * Determines whether a Scene is visible.
		 *
		 * @since 0.0.0
		 * @param key The key of the scene to check.
		 * @return Whether the Scene is visible.
		 */
		bool isVisible (std::string key);

		/**
		 * Determines whether a Scene is sleeping.
		 *
		 * @since 0.0.0
		 * @param key The key of the scene to check.
		 * @return Whether the Scene is sleeping.
		 */
		bool isSleeping (std::string key);

		/**
		 * Pauses the given Scene.
		 *
		 * @since 0.0.0
		 * @param key The Scene to pause.
		 * @param data An optional data object to be passed to the Scene and
		 * emitted by its pause event.
		 * @return A reference to this SceneManager.
		 */
		SceneManager& pause (std::string key, Data data = {});

		/**
		 * Resumes the given Scene.
		 *
		 * @since 0.0.0
		 * @param key The Scene to resume.
		 * @param data An optional data object to be passed to the Scene and
		 * emitted by its resume event.
		 * @return A reference to this SceneManager.
		 */
		SceneManager& resume (std::string key, Data data = {});

		/**
		 * Puts the given Scene to sleep.
		 *
		 * @since 0.0.0
		 * @param key The Scene to put to sleep.
		 * @param data An optional data object to be passed to the Scene and
		 * emitted by its sleep event.
		 * @return A reference to this SceneManager.
		 */
		SceneManager& sleep (std::string key, Data data = {});

		/**
		 * Awakens the given Scene.
		 *
		 * @since 0.0.0
		 * @param key The Scene to put to wake up.
		 * @param data An optional data object to be passed to the Scene and
		 * emitted by its wake event.
		 * @return A reference to this SceneManager.
		 */
		SceneManager& wake (std::string key, Data data = {});

		/**
		 * Runs the given Scene.
		 *
		 * If the given Scene is paused, it will resume it. If sleeping, it will
		 * wake it. If not running at all, it will be started.
		 *
		 * Use this if you wish to open a modal Scene by calling `pause` on
		 * the current Scene, then `run` on the modal Scene.
		 *
		 * @since 0.0.0
		 * @param key The Scene to put to run.
		 * @param data An optional data object to be passed to the Scene and
		 * emitted by its start, wake or resume event.
		 * @return A reference to this SceneManager.
		 */
		SceneManager& run (std::string key, Data data = {});

		/**
		 * Starts the given Scene.
		 *
		 * @since 0.0.0
		 * @param key The Scene to put to start.
		 * @param data An optional data object to be passed to the Scene and
		 * emitted by its start event.
		 * @return A reference to this SceneManager.
		 */
		SceneManager& start (std::string key, Data data = {});

		/**
		 * Stops the given Scene.
		 *
		 * @since 0.0.0
		 * @param key The Scene to put to stop.
		 * @param data An optional data object to pass to Scene.shutdown.
		 * @return A reference to this SceneManager.
		 */
		SceneManager& stop (std::string key, Data data = {});

		/**
		 * Put to sleep one Scene and starts the other.
		 *
		 * @since 0.0.0
		 * @param keyFrom The Scene to put to put to sleep.
		 * @param keyTo The Scene to start.
		 * @return A reference to this SceneManager.
		 */
		SceneManager& swap (std::string keyFrom, std::string keyTo);

		/**
		 * Retrieves a Scene by numeric index.
		 *
		 * @since 0.0.0
		 * @param index The Scene to put to put to sleep.
		 * @return A pointer to the Scene.
		 */
		Scene* getAt (int index);

		/**
		 * Retrieves the numeric index of a Scene.
		 *
		 * @since 0.0.0
		 * @param key The key of the Scene.
		 * @return The index of the Scene.
		 */
		int getIndex (std::string key);

		/**
		 * Brings a Scene to the top of the Scenes list.
		 *
		 * This means it will render above all other Scenes.
		 *
		 * @since 0.0.0
		 * @param key The key of the Scene to move.
		 * @return A reference to this SceneManager instance.
		 */
		SceneManager& bringToTop (std::string key);

		/**
		 * Sends a Scene to the back of the Scenes list.
		 *
		 * This means it will render below all other Scenes.
		 *
		 * @since 0.0.0
		 * @param key The key of the Scene to move.
		 * @return A reference to this SceneManager instance.
		 */
		SceneManager& sendToBack (std::string key);

		/**
		 * Moves a Scene down one position in the Scenes list.
		 *
		 * This means it will render below all other Scenes.
		 *
		 * @since 0.0.0
		 * @param key The key of the Scene to move.
		 * @return A reference to this SceneManager instance.
		 */
		SceneManager& moveDown (std::string key);

		/**
		 * Moves a Scene up one position in the Scenes list.
		 *
		 * This means it will render below all other Scenes.
		 *
		 * @since 0.0.0
		 * @param key The key of the Scene to move.
		 * @return A reference to this SceneManager instance.
		 */
		SceneManager& moveUp (std::string key);

		/**
		 * Moves a Scene so it is immediately above another Scene in the Scenes
		 * list.
		 *
		 * This means it will render over the top of the other Scene.
		 *
		 * @since 0.0.0
		 * @param keyA The key of the Scene that Scene B will be moved over.
		 * @param keyB The key of the Scene to move.
		 * @return A reference to this SceneManager instance.
		 */
		SceneManager& moveAbove (std::string keyA, std::string keyB);

		/**
		 * Moves a Scene so it is immediately below another Scene in the Scenes
		 * list.
		 *
		 * This means it will render behind the other Scene.
		 *
		 * @since 0.0.0
		 * @param keyA The key of the Scene that Scene B will be moved under.
		 * @param keyB The key of the Scene to move.
		 * @return A reference to this SceneManager instance.
		 */
		SceneManager& moveBelow (std::string keyA, std::string keyB);

		/**
		 * Queue a Scene operation for the next update.
		 *
		 * @since 0.0.0
		 * @param operation The operation to perform.
		 * @param keyA The key of Scene A.
		 * @param keyB The key of Scene B.
		 * @param data A data object.
		 * @return A reference to this SceneManager instance.
		 */
		SceneManager& queueOp (
				std::string operation,
				std::string keyA,
				std::string keyB = "",
				Data data = {});

		/**
		 * Swaps the positions of two Scenes in the Scenes list.
		 *
		 * @since 0.0.0
		 * @param keyA The key of Scene A.
		 * @param keyB The key of Scene B.
		 * @return A reference to this SceneManager instance.
		 */
		SceneManager& swapPosition (std::string keyA, std::string keyB);


	private:
		/**
		 * The Game that this SceneManager belongs to.
		 *
		 * @since 0.0.0
		 */
		Game& game;

		/**
		 * A map of keys and scenes to quickly get a scene from a key without
		 * iteration.
		 *
		 * @since 0.0.0
		 */
		std::map<std::string, Scene&> keys;

		/**
		 * The vector in which all of the scenes are kept.
		 *
		 * @since 0.0.0
		 */
		std::vector<std::unique_ptr<Scene>> scenes;

		/**
		 * Scenes pending to be added are stored in here until the manager has
		 * time to add it.
		 *
		 * @since 0.0.0
		 */
		std::vector<SceneConfig> _pending;

		/**
		 * A vector of keys of scenes to be started once the game has booted.
		 *
		 * @since 0.0.0
		 */
		std::vector<std::string> _start;

		/**
		 * An operations queue, because we don't manipulate the scenes vector
		 * during processing.
		 *
		 * @since 0.0.0
		 */
		std::queue<SceneOperation> _queue;

		std::map<std::string, SceneConfig> _data;

		/**
		 * Internal first-time Scene boot handler.
		 *
		 * @since 0.0.0
		 */
		void bootQueue (Data data);

		/**
		 * Boot the given Scene.
		 *
		 * @since 0.0.0
		 */
		void bootScene (Scene *scene);

		/**
		 * Handles load completion for a Scene's Loader.
		 *
		 * Starts the Scene that the Loader belongs to.
		 *
		 * @since 0.0.0
		 * @param data A data object, used here to pass the Scene's key.
		 */
		void loadComplete (Data data);

		/**
		 * Calls the given Scene's `create` method.
		 *
		 * @since 0.0.0
		 * @param scene The scene to create.
		 */
		void create (Scene *scene);

		/**
		 * Retrieves the key of a Scene from a Scene config.
		 *
		 * @since 0.0.0
		 * @param key The key to check in the Scene config.
		 * @param sceneConfig The Scene config.
		 */
		std::string getKey (
			std::string key,
			SceneSettingsConfig sceneConfig
			);

		/**
		 * Is the SceneManager actively processing the Scenes list?
		 *
		 * @since 0.0.0
		 */
		bool isProcessing;

		/**
		 * Has the SceneManager properly started?
		 *
		 * @since 0.0.0
		 */
		bool isBooted;
	};
}

// Declarations of the forward declared elements
#include "../core/game.h"

#endif
