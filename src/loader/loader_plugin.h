/**
 * @file		loader_plugin.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef LOADER_PLUGIN_H
#define LOADER_PLUGIN_H

#include <vector>
#include "../messages.h"
#include "../texture/sprite_sheet_config.h"

namespace Zen {
namespace Loader {

class LoaderPlugin
{
public:
	/**
	 * @since 0.0.0
	 */
	LoaderPlugin (Scene& scene);

	/**
	 * @since 0.0.0
	 */
	~LoaderPlugin ();

	/**
	 * The Scene which owns this Loader instance.
	 *
	 * @since 0.0.0
	 */
	Scene& scene_;

	/**
	 * A reference to the global Texture Manager.
	 *
	 * @since 0.0.0
	 */
	TextureManager& textureManager_;

	/**
	 * A reference to the global Scene Manager.
	 *
	 * @since 0.0.0
	 */
	SceneManager& sceneManager_;

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
	std::string prefix_ = "";

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
	std::string path_ = "";

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
	 * Sets the path_ member.
	 *
	 * @since 0.0.0
	 *
	 * @param path The path to use. Leave empty to reset.
	 *
	 * @return This LoaderPlugin object.
	 */
    LoaderPlugin& setPath (std::string path = "");

	/**
	 * Sets the prefix_ member.
	 *
	 * @since 0.0.0
	 *
	 * @param path The prefix to use. Leave empty to reset.
	 *
	 * @return This LoaderPlugin object.
	 */
    LoaderPlugin& setPrefix (std::string prefix = "");

	/**
	 * Load a simple image file.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& image (std::string key, std::string path);

	/**
	 * Load an atlas texture.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& atlas (std::string key, std::string texturePath, std::string atlasPath);

	/**
	 * Load a multi-atlas texture.
	 *
	 * It is basically a single atlas JSON config file with multiple image files.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& multiatlas (std::string key, std::string atlasPath, std::string path);

	/**
	 * Load a spritesheet.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& spritesheet (std::string key, std::string path, SpriteSheetConfig config);

	/**
	 * Load an audio file.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& audio (std::string key, std::string path);

	/**
	 * Load a font file.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& font (std::string key, std::string path);

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
