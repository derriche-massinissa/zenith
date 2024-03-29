/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_CORE_CONFIG_HPP
#define ZEN_CORE_CONFIG_HPP

#include <string>
#include <queue>
#include <memory>
#include <functional>

#include "../scale/scale_modes.hpp"
#include "../display/types/color.hpp"

#include "../scene/scene.fwd.hpp"
#include "types/render_config.hpp"

namespace Zen {

class GameConfig {

public:
	// Setters
	/**
	 * Sets the window width member variable.
	 *
	 * @since 0.0.0
	 * @param w Window width in pixels.
	 */
	GameConfig& setWidth (int w);

	/**
	 * Sets the height member variable.
	 *
	 * @since 0.0.0
	 * @param h Window height in pixels.
	 */
	GameConfig& setHeight (int h);

	/**
	 * Sets the scale mode member variable.
	 *
	 * @since 0.0.0
	 * @param sm Scale mode for the scale manager to use.
	 */
	GameConfig& setScaleMode (SCALE_MODE sm);

	/**
	 * Sets the fullscreen member variable.
	 *
	 * @since 0.0.0
	 * @param flag If the flag is true, the window will open in fullscreen.
	 */
	GameConfig& setFullscreen (bool flag);

	/**
	 * Sets the minimum width member variable.
	 *
	 * @since 0.0.0
	 * @param mw The minimum width of the window in pixels.
	 */
	GameConfig& setMinWidth (int mw);

	/**
	 * Sets the maximum width member variable.
	 *
	 * @since 0.0.0
	 * @param mw The maximum width of the window in pixels.
	 */
	GameConfig& setMaxWidth (int mw);

	/**
	 * Sets the minimum height member variable.
	 *
	 * @since 0.0.0
	 * @param mh The minimum height of the window in pixels.
	 */
	GameConfig& setMinHeight (int mh);

	/**
	 * Sets the maximum height member variable.
	 *
	 * @since 0.0.0
	 * @param mh The maximum height of the window in pixels.
	 */
	GameConfig& setMaxHeight (int mh);

	/**
	 * Add a scene to the scenes vector.
	 *
	 * @since 0.0.0
	 *
	 * @tparam T A single scene class.
	 */
	template <typename T>
	GameConfig& addScenes ()
	{
		sceneFactory.emplace(
			[] () -> std::unique_ptr<T>
				{
					return std::make_unique<T> ();
				}
			);

		return *this;
	}

	/**
	 * Add one or more scene to the scenes vector.
	 *
	 * @since 0.0.0
	 *
	 * @tparam T A scene class.
	 * @tparam O A second scene class.
	 * @tparam Args 0 or more scene classes.
	 */
	template <typename T, typename O, typename... Args>
	GameConfig& addScenes ()
	{
		addScenes<T>();
		addScenes<O, Args...>();

		return *this;
	}

	/**
	 * Sets the title of the window.
	 *
	 * @since 0.0.0
	 * @param t The window title.
	 */
	GameConfig& setTitle (std::string t);

	/**
	 * Sets the version of the game.
	 *
	 * @since 0.0.0
	 * @param v The game version. The format is up to the user.
	 */
	GameConfig& setVersion (std::string v);

	/**
	 * Enables the keyboard input.
	 *
	 * @since 0.0.0
	 * @param flag If true, enable keyboard input.
	 */
	GameConfig& setKeyboardInput (bool flag);

	/**
	 * Enables the mouse input.
	 *
	 * @since 0.0.0
	 * @param flag If true, enable mouse input.
	 */
	GameConfig& setMouseInput (bool flag);

	/**
	 * Enable antialiasing.
	 *
	 * @since 0.0.0
	 * @param flag If true, enable antialiasing.
	 */
	GameConfig& setAntialiasing (bool flag);

	/**
	 * Disable all antialiasing options.
	 *
	 * @since 0.0.0
	 * @param flag If true, enable pixel art mode.
	 */
	GameConfig& setPixelArt (bool flag);

	/**
	 * @since 0.0.0
	 *
	 * @param flag If true, enable pixel round.
	 */
	GameConfig& setRoundPixels (bool flag);

	/**
	 * Whether the renderer will be cleared between each rendering frame. You can
	 * disable this if you have a full-screen background image or game object.
	 *
	 * @since 0.0.0
	 * @param flag If `true`, the renderer will be cleared each frame.
	 */
	GameConfig& setClearBeforeRender (bool flag = true);

	/**
	 * @since 0.0.0
	 *
	 * @param flag If true, do not clear OpenGL buffers.
	 */

	GameConfig& setPreserveDrawingBuffer (bool flag);

	/**
	 * @since 0.0.0
	 *
	 * @param size The number of quads in a render batch.
	 */
	GameConfig& setBatchSize (int size);

	/**
	 * @since 0.0.0
	 *
	 * @param number The maximum number of textures before triggering a flush.
	 */
	GameConfig& setMaxTextures (int number);

	/**
	 * @since 0.0.0
	 *
	 * @param size The maximum texture size.
	 */
	GameConfig& setMaxTextureSize (int size);

	/**
	 * @since 0.0.0
	 *
	 * @param filter The mipmapping filter to use.
	 */
	GameConfig& setMipMapFilter (GLenum filter);

	/**
	 * Sets the background color used by the renderer for cleaning the screen.
	 *
	 * @since 0.0.0
	 * @param color The color value in hexadecimal.
	 */
	GameConfig& setBackgroundColor (unsigned int color);
	/**
	 * @overload
	 *
	 * @since 0.0.0
	 * @param red The red color value, between 0 and 255.
	 * @param green The green color value, between 0 and 255.
	 * @param blue The blue color value, between 0 and 255.
	 */
	GameConfig& setBackgroundColor (
			unsigned int red, unsigned int green, unsigned int blue);

	/**
	 * Sets the relative loader path, for resource loading.
	 *
	 * @since 0.0.0
	 *
	 * @param path The resource path, relative to the executable.
	 */
	GameConfig& setLoaderPath (std::string path);

	/**
	 * Sets the relative loader path, for resource loading.
	 *
	 * @since 0.0.0
	 *
	 * @param prefix The resource path, relative to the executable.
	 */
	GameConfig& setLoaderPrefix (std::string prefix);

	/**
	 * The delta time between each game step when the window isn't visible, as it
	 * isn't rendering and so not vsynced.
	 * It is recommended to never set it to zero.
	 *
	 * @since 0.0.0
	 */
	GameConfig& setHiddenDelta (unsigned int delta);

	// Member variables
	/**
	 * The width of the window, in pixels.
	 *
	 * @since 0.0.0
	 */
	int width = 640;

	/**
	 * The height of the window, in pixels.
	 *
	 * @since 0.0.0
	 */
	int height = 480;

	/** 
	 * The scale mode, as used by the Scale Manager.
	 *
	 * @since 0.0.0
	 */
	SCALE_MODE scaleMode = SCALE_MODE::RESIZE;

	/**
	 * Indicates if the window starts at fullscreen;
	 *
	 * @since 0.0.0
	 */
	bool fullscreen = false;

	/**
	 * The minimum width, in pixels, the window will scale down to. A value of
	 * zero means no minimum.
	 *
	 * @since 0.0.0
	 */
	int minWidth = 0;

	/**
	 * The maximum width, in pixels, the window will scale up to. A value of
	 * zero means no maximum.
	 *
	 * @since 0.0.0
	 */
	int maxWidth = 0;

	/**
	 * The minimum height, in pixels, the window will scale down to. A value of
	 * zero means no minimum.
	 *
	 * @since 0.0.0
	 */
	int minHeight = 0;

	/**
	 * The maximum height, in pixels, the window will scale up to. A value of
	 * zero means no maximum.
	 *
	 * @since 0.0.0
	 */
	int maxHeight = 0;

	/**
	 * The delta time between each game step when the window isn't visible, as it
	 * isn't rendering and so not vsynced.
	 * It is recommended to never set it to zero.
	 *
	 * @since 0.0.0
	 */
	unsigned int hiddenDelta = 20;

	/**
	 * A queue of functors responsible for making and returning a 
	 * unique pointer to a new Scene instance.
	 *
	 * @since 0.0.0
	 */
	std::queue<std::function<std::unique_ptr<Scene>()>> sceneFactory;

	/**
	 * The title of the window.
	 *
	 * @since 0.0.0
	 */
	std::string title = "Zenith";

	/**
	 * The version of the game.
	 *
	 * @since 0.0.0
	 */
	std::string version = "1.0.0";

	/**
	 * Enable the keyboard plugin. This can be disabled in games that don't
	 * need keyboard input.
	 *
	 * @since 0.0.0
	 */
	bool inputKeyboard = true;

	/**
	 * Enable the mouse plugin. This can be disabled in games that don't
	 * need mouse input.
	 *
	 * @since 0.0.0
	 */
	bool inputMouse = true;

	/**
	 * The configuration of all render related systems.
	 *
	 * @since 0.0.0
	 */
	RenderConfig renderConfig;

	/**
	 * @const The smoothing factor to apply during Pointer movement.
	 */
	double inputSmoothFactor = 0.;

	/**
	 * A path used to resolve relative paths given to the loader.
	 *
	 * @since 0.0.0
	 */
	std::string loaderPath = "";

	/**
	 * A prefix to add to every file loaded.
	 *
	 * @since 0.0.0
	 */
	std::string loaderPrefix = "";

	// Default / Missing Images
	std::string pngPrefix = "iVBORw0KGgoAAAANSUhEUgAAACAAAAAg";

	/**
	 * A base64 encoded PNG that will be used as the default blank texture.
	 *
	 * @since 0.0.0
	 */
	std::string defaultImage = pngPrefix + "AQMAAABJtOi3AAAAA1BMVEX///+nxBvIAAAAAXRSTlMAQObYZgAAABVJREFUeF7NwIEAAAAAgKD9qdeocAMAoAABm3DkcAAAAABJRU5ErkJggg==";

	/**
	 * A base64 encoded PNG that will be used as the default texture when a texture 
	 * is assigned that is missing or not loaded.
	 *
	 * @since 0.0.0
	 */
	std::string missingImage = pngPrefix + "CAIAAAD8GO2jAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAAJ9JREFUeNq01ssOwyAMRFG46v//Mt1ESmgh+DFmE2GPOBARKb2NVjo+17PXLD8a1+pl5+A+wSgFygymWYHBb0FtsKhJDdZlncG2IzJ4ayoMDv20wTmSMzClEgbWYNTAkQ0Z+OJ+A/eWnAaR9+oxCF4Os0H8htsMUp+pwcgBBiMNnAwF8GqIgL2hAzaGFFgZauDPKABmowZ4GL369/0rwACp2yA/ttmvsQAAAABJRU5ErkJggg==";

	/**
	 * A base64 encoded PNG that will be used as the default texture when a texture 
	 * is assigned that is white or not loaded.
	 *
	 * @since 0.0.0
	 */
	std::string whiteImage = "iVBORw0KGgoAAAANSUhEUgAAAAQAAAAECAIAAAAmkwkpAAAAGXRFWHRTb2Z0d2FyZQBBZG9iZSBJbWFnZVJlYWR5ccllPAAAABdJREFUeNpi/P//PwMMMDEgAdwcgAADAJZuAwXJYZOzAAAAAElFTkSuQmCC";
};

}	// namespace GameConfig

#endif
