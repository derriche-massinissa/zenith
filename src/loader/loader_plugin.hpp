/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_LOADER_LOADER_PLUGIN_HPP
#define ZEN_LOADER_LOADER_PLUGIN_HPP

#include <vector>
#include <string>

#include "../texture/sprite_sheet_config.hpp"

#include "../texture/texture_manager.fwd.hpp"
#include "../scene/scene.fwd.hpp"
#include "../scene/scene_manager.fwd.hpp"
#include "../cameras/2d/camera_manager.fwd.hpp"
#include "../event/event_listener.hpp"

namespace Zen {

class LoaderPlugin
{
public:
	/**
	 * @since 0.0.0
	 *
	 * @param scene The Scene this LoaderPlugin belongs to.
	 */
	LoaderPlugin (Scene* scene);

	/**
	 * The Scene which owns this Loader instance.
	 *
	 * @since 0.0.0
	 */
	Scene* scene;

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
	 * @param path The path to use. Leave empty to reset.
	 *
	 * @return This LoaderPlugin object.
	 */
    LoaderPlugin& setPath (std::string path = "");

	/**
	 * Sets the prefix member.
	 *
	 * @since 0.0.0
	 *
	 * @param prefix The prefix to use. Leave empty to reset.
	 *
	 * @return This LoaderPlugin object.
	 */
    LoaderPlugin& setPrefix (std::string prefix = "");

	/**
	 * Load an image file.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& image (std::string key, std::string path, bool alphaCache = false);
	
	LoaderPlugin& cursor (std::string key, std::string path, int hotX = 0, int hotY = 0);

	/**
	 * Load an atlas texture.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& atlas (std::string key, std::string texturePath, std::string atlasPath, bool alphaCache = false);

	/**
	 * Load a multi-atlas texture.
	 *
	 * It is basically a single atlas JSON config file with multiple image files.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& multiatlas (std::string key, std::string atlasPath, std::string path = "", bool alphaCache = false);

	/**
	 * Load a spritesheet.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& spritesheet (std::string key, std::string path, SpriteSheetConfig config = {});

	/**
	 * Load an audio file. Used for short sound effects.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& audioShort (std::string key, std::string path);

	/**
	 * Add a file for audio streaming without loading it into memory.
	 *
	 * This is used for large files that would occupy too much RAM if copied,
	 * so they are read from directly from the disk.
	 *
	 * @since 0.0.0
	 */
	LoaderPlugin& audioStream (std::string key, std::string path);

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
};

}	// namespace Zen

#endif
