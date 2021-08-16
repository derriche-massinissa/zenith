/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SCENE_HPP
#define ZEN_SCENE_HPP

#include <string>
#include <SDL2/SDL_types.h>

#include "../data.h"
#include "../event/event_emitter.hpp"

#include "systems.hpp"
#include "plugin.hpp"
#include "../core/game.hpp"
#include "../scale/scale_manager.hpp"
#include "../cameras/2d/camera_manager.hpp"
#include "../loader/loader_plugin.hpp"
#include "../texture/texture_manager.hpp"
#include "../gameobjects/display_list.hpp"
#include "../gameobjects/update_list.h"
#include "../gameobjects/gameobject_factory.h"
#include "../input/input_plugin.hpp"
#include "../audio/audio_manager.hpp"
#include "../tweens/tween_manager.hpp"
#include "../window/window.hpp"

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
	 *
	 * @param key The key of the Scene.
	 */
	Scene (std::string key);

	virtual ~Scene () = default;

	/**
	 * The Scene Systems.
	 *
	 * @since 0.0.0
	 */
	SceneSystems sys;

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
	 * A reference to the window instance.
	 *
	 * @since 0.0.0
	 */
	Window& window;

	/**
	 * A reference to the game Audio Manager.
	 *
	 * @since 0.0.0
	 */
	AudioManager& audio;

	/**
	 * A reference to the game Texture Manager.
	 *
	 * @since 0.0.0
	 */
	TextureManager& textures;

	/**
	 * A scene level EventEmitter.
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
	GameObjectFactory add;

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
	ScenePlugin scene;

	/**
	 * A scene level GameObject display list.
	 *
	 * @since 0.0.0
	 */
	DisplayList children;

	/**
	 * A scene level GameObject update list.
	 *
	 * @since 0.0.0
	 */
	GameObjects::UpdateList updateList;

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
	InputPlugin input;

	/**
	 * A scene level LoaderPlugin.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin load;

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
	 */
	TweenManager tweens;

	/**
	 * A scene level Box2D physics plugin.
	 *
	 * @since 0.0.0
	 * @todo TODO
	 */
	//Physics::Box2D::Physics physics;

	/**
	 * A reference to the Game level ScaleManager.
	 *
	 * @since 0.0.0
	 */
	ScaleManager& scale;

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
	virtual void init (Data data = {});

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
	virtual void create (Data data = {});

	/**
	 * Should be overridden by your own Scenes.
	 * This method is called once per game step while the scene is running.
	 *
	 * @since 0.0.0
	 *
	 * @param time The time in ms since SDL started.
	 * @param delta The delta time in ms since the last frame.
	 */
	virtual void update (Uint32 time, Uint32 delta);
};

}	// namespace Zen

#endif
