/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SCENE_H
#define ZEN_SCENE_H

#include <string>

// Forward declarations
#include "../core/game.fwd.h"
#include "../scale/scale_manager.fwd.h"
#include "../cameras/2d/camera_manager.fwd.h"

#include "scene_systems.h"
#include "scene_plugin.h"
#include "../event/event_emitter.h"
#include "../loader/loader_plugin.h"
#include "../texture/texture_manager.h"

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
	/**
	 * @since 0.0.0
	 */
	Scene (Game& game_);

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
	Scenes::SceneSystems sys;

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
	 * @todo TODO
	 */
	//Animations::AnimationManager& anims;

	/**
	 * A reference to the game Data Manager.
	 *
	 * @since 0.0.0
	 * @todo TODO
	 */
	//Data::DataManager& registry;

	/**
	 * A reference to the game Audio Manager.
	 *
	 * @since 0.0.0
	 * @todo TODO
	 */
	//Audio::AudioManager& audio;

	/**
	 * A reference to the game Texture Manager.
	 *
	 * @since 0.0.0
	 */
	Textures::TextureManager& textures;

	/**
	 * A scene level Event Emitter.
	 *
	 * @since 0.0.0
	 */
	Events::EventEmitter events;

	/**
	 * A scene level Camera System.
	 *
	 * @since 0.0.0
	 */
	Cameras::Scene2D::CameraManager cameras;

	/**
	 * A scene level Game Object Factory.
	 *
	 * @since 0.0.0
	 * @todo TODO
	 */
	//GameObjects::GameObjectFactory add;

	/**
	 * A scene level Game Object Creator.
	 *
	 * @since 0.0.0
	 * @todo TODO
	 */
	//GameObjects::GameObjectCreator make;

	/**
	 * The Scene Manager Plugin of this Scene, used as an interface to
	 * interact with the Game level Scene Manager.
	 *
	 * @since 0.0.0
	 */
	Scenes::ScenePlugin scene;

	/**
	 * A scene level GameObject display list.
	 *
	 * @since 0.0.0
	 * @todo TODO
	 */
	//GameObjects::DisplayList children;

	/**
	 * A scene level DataManager Plugin.
	 *
	 * @since 0.0.0
	 * @todo TODO
	 */
	//Data::DataManager data;

	/**
	 * A scene level InputManager plugin.
	 *
	 * @since 0.0.0
	 * @todo TODO
	 */
	//Input::InputPlugin input;

	/**
	 * A scene level LoaderPlugin.
	 *
	 * @since 0.0.0
	 */
	Loader::LoaderPlugin load;

	/**
	 * A scene level Time and Clock plugin.
	 *
	 * @since 0.0.0
	 * @todo TODO
	 */
	//Time::Clock time;

	/**
	 * A scene level TweenManager plugin.
	 *
	 * @since 0.0.0
	 * @todo TODO
	 */
	//Tweens::TweenManager tweens;

	/**
	 * A scene level Box2D physics plugin.
	 *
	 * @since 0.0.0
	 * @todo TODO
	 */
	//Physics::Box2D::Box2DPhysics physics;

	/**
	 * A reference to the Game level ScaleManager.
	 *
	 * @since 0.0.0
	 */
	Scale::ScaleManager& scale;

	/**
	 * A reference to the renderer instance.
	 *
	 * @since 0.0.0
	 */
	Renderer& renderer;

	/**
	 * Can be defined on your own Scenes.
	 *
	 * This method is called by the Scene Manager when the Scene starts,
	 * before `preload()` and `create()`.
	 *
	 * @since 0.0.0
	 *
	 * @param data Any data passed via `ScenePlugin::add` or
	 * `ScenePlugin::start`. Same as Scene.sys.settings.data.
	 */
	virtual void init (Data data_ = {});

	/**
	 * Can be defined on your own Scenes. Use it to load assets.
	 *
	 * This method is called by the Scene Manager, after `init` and 
	 * before `create`, only if the Scene has a LoaderPlugin.
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
	 * after `init` and `preload`.
	 * If the LoaderPlugin started after `preload`, then this method is
	 * called only after loading is complete.
	 *
	 * @since 0.0.0
	 *
	 * @param data Any data passed via `ScenePlugin::add` or
	 * `ScenePlugin::start`. Same as Scene.sys.settings.data.
	 */
	virtual void create (Data data_ = {});

	/**
	 * Should be overridden by your own Scenes.
	 * This method is called once per game step while the scene is running.
	 *
	 * @since 0.0.0
	 * @param time The time in ms since SDL started.
	 * @param delta The delta time in ms since the last frame.
	 */
	virtual void update (Uint32 time_, Uint32 delta_);
};

}	// namespace Zen

#include "../scale/scale_manager.h"
#include "../cameras/2d/camera_manager.h"

#endif
