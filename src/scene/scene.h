/**
 * @file		scene.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef SCENE_H
#define SCENE_H

#include <string>

// Forward declarations
#include "../core/game.fwd.h"

#include "scene_systems.h"
#include "scene_plugin.h"
#include "../event/event_emitter.h"

namespace Zen {
	/**
	 * A base Scene class which can be extended for your own use.
	 *
	 * You can also define the optional methods init(), preload(), create();
	 *
	 * @since 0.0.0
	 */
	class Scene
	{
	public:
		Scene (Game& g);
		~Scene ();

		/**
		 * The key of the Scene. You have to override it with the key of your
		 * own Scene.
		 *
		 * This key has to be unique, no two Scenes can have the same key!
		 * Or very bad things WILL happen...
		 *
		 * @since 0.0.0
		 */
		const std::string key = "default";


		/**
		 * The Scene Systems.
		 *
		 * @since 0.0.0
		 */
		SceneSystems sys;

		/**
		 * A reference to the Zenith Game instance.
		 *
		 * @since 0.0.0
		 */
		Game& game;

		/**
		 * A reference to the game Animation Manager.
		 *
		 * @since 0.0.0
		 */
		//AnimationManager& anims;

		/**
		 * A reference to the game Data Manager.
		 *
		 * @since 0.0.0
		 */
		//DataManager& registry;

		/**
		 * A reference to the game Audio Manager.
		 *
		 * @since 0.0.0
		 */
		//AudioManager& audio;

		/**
		 * A reference to the game Texture Manager.
		 *
		 * @since 0.0.0
		 */
		TextureManager& textures;

		/**
		 * A scene level Event Emitter.
		 *
		 * @since 0.0.0
		 */
		EventEmitter events;

		/**
		 * A scene level Camera System.
		 *
		 * @since 0.0.0
		 */
		CameraManager cameras;

		/**
		 * A scene level Game Object Factory.
		 *
		 * @since 0.0.0
		 */
		//GameObjectFactory add;

		/**
		 * A scene level Game Object Creator.
		 *
		 * @since 0.0.0
		 */
		//GameObjectCreator make;

		/**
		 * The Scene Manager Plugin of this Scene, used as an interface to
		 * interact with the Game level Scene Manager.
		 *
		 * @since 0.0.0
		 */
		ScenePlugin scene;

		//DisplayList children;

		//DataManager data;

		//InputPlugin input;

		//LoaderPlugin load;

		//ClockPlugin time;

		//TweenManager tweens;

		//ScaleManager scale;

		/**
		 * Can be defined on your own Scenes.
		 *
		 * This method is called by the Scene Manager when the Scene starts,
		 * before `preload()` and `create()`.
		 *
		 * @since 0.0.0
		 * @param data Any data passed via `ScenePlugin.add()` or
		 * `ScenePlugin.start()`. Same as Scene.sys.settings.data.
		 */
		virtual void init (Data data = {});

		/**
		 * Can be defined on your own Scenes. Use it to load assets.
		 *
		 * This method is called by the Scene Manager, after `init()` and 
		 * before `create()`, only if the Scene has a LoaderPlugin.
		 * After this method completes, if the LoaderPlugin's isn't empty, the
		 * LoaderPlugin will start automatically.
		 *
		 * @since 0.0.0
		 */
		virtual void preload ();

		/**
		 * Can be defined on your own Scenes. Use it to create your game
		 * objects.
		 *
		 * This method is called by the Scene Manager when the scene starts,
		 * after `init()` and `preload()`.
		 * If the LoaderPlugin started after `preload()`, then this method is
		 * called only after loading is complete.
		 *
		 * @since 0.0.0
		 * @param data Any data passed via `ScenePlugin.add()` or
		 * `ScenePlugin.start()`. Same as Scene.sys.settings.data.
		 */
		virtual void create (Data data = {});

		/**
		 * Should be overridden by your own Scenes.
		 * This method is called once per game step while the scene is running.
		 *
		 * @since 0.0.0
		 * @param time The time in ms since SDL started.
		 * @param delta The delta time in ms since the last frame.
		 */
		virtual void update (Uint32 time, Uint32 delta);
	};
}

// Declarations of the forward declared elements
//#include "../core/game.h"

#endif
