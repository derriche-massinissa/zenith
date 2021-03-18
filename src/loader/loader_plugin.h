/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_LOADER_LOADER_PLUGIN_H
#define ZEN_LOADER_LOADER_PLUGIN_H

#include <vector>
#include <string>
#include "../messages.h"
#include "../texture/sprite_sheet_config.h"
#include "../texture/texture_manager.fwd.h"
#include "../scene/scene.fwd.h"
#include "../scene/scene_manager.fwd.h"
#include "../event/event_emitter.h"
#include "../cameras/2d/camera_manager.fwd.h"

#include "nlohmann/json.hpp"

namespace Zen {
namespace Loader {

class LoaderPlugin
{
public:
	/**
	 * @since 0.0.0
	 *
	 * @param scene_ The Scene this LoaderPlugin belongs to.
	 */
	LoaderPlugin (Scene& scene_);

	/**
	 * @since 0.0.0
	 */
	~LoaderPlugin ();

	/**
	 * The Scene which owns this Loader instance.
	 *
	 * @since 0.0.0
	 */
	Scene& scene;

	/**
	 * A reference to the global Texture Manager.
	 *
	 * @since 0.0.0
	 */
	Textures::TextureManager& textureManager;

	/**
	 * A reference to the global Scene Manager.
	 *
	 * @since 0.0.0
	 */
	Scenes::SceneManager& sceneManager;

	/**
	 * An optional prefix that is automatically prepended to the start of every 
	 * file key.
	 *
	 * If prefix was `MENU.` and you load an image with the key 'Background' 
	 * the resulting key would be `MENU.Background`.
	 * You can set this by calling Loader::setPrefix. It will then 
	 * affect every file added to the Loader from that point on. It does _not_ 
	 * change any file already in the load queue.
	 *
	 * @since 0.0.0
	 */
	std::string prefix = "";

	/**
	 * The value of `path`, if set, is placed before any _relative_ file path 
	 * given. For example:
	 *
	 * ```c++
	 * load.image("ball", "ball.png");
	 * load.image("tree", "level1/oaktree.png");
	 * ```
	 *
	 * Would load the `ball` file from `images/sprites/ball.png` and the tree 
	 * from
	 * `images/sprites/level1/oaktree.png`.
	 *
	 * @since 0.0.0
	 */
	std::string path = "";

	/**
	 * This method is called automatically by the Scene when it is starting up.
	 *
	 * It is responsible for creating local systems, properties and listening for 
	 * Scene events.
	 * Do not invoke it directly.
	 *
	 * @since 0.0.0
	 */
	void pluginStart ();

	/**
	 * Sets the path member.
	 *
	 * @since 0.0.0
	 *
	 * @param path_ The path to use. Leave empty to reset.
	 *
	 * @return This LoaderPlugin object.
	 */
    LoaderPlugin& setPath (std::string path_ = "");

	/**
	 * Sets the prefix member.
	 *
	 * @since 0.0.0
	 *
	 * @param prefix_ The prefix to use. Leave empty to reset.
	 *
	 * @return This LoaderPlugin object.
	 */
    LoaderPlugin& setPrefix (std::string prefix_ = "");

	/**
	 * Load an image file.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& image (std::string key_, std::string path_);

	/**
	 * Load an atlas texture.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& atlas (std::string key_, std::string texturePath_, std::string atlasPath_);

	/**
	 * Load a multi-atlas texture.
	 *
	 * It is basically a single atlas JSON config file with multiple image files.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& multiatlas (std::string key_, std::string atlasPath_, std::string path_);

	/**
	 * Load a spritesheet.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& spritesheet (std::string key_, std::string path_, Textures::SpriteSheetConfig config_);

	/**
	 * Load an audio file.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& audio (std::string key_, std::string path_);

	/**
	 * Load a font file.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& font (std::string key_, std::string path_);

	/**
	 * Resets the loader, reseting it's path and prefix too.
	 *
	 * @since 0.0.0
	 */
	void reset ();

	/**
	 * Cleans up when the owning scene is shutting down.
	 *
	 * @since 0.0.0
	 */
	void shutdown ();
};

}	// namespace Loader
}	// namespace Zen

#endif
