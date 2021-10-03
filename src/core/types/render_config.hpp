/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_CORE_CONFIG_RENDER_HPP
#define ZEN_CORE_CONFIG_RENDER_HPP

#include <GL/glew.h>
#include "../../display/types/color.hpp"

namespace Zen {

/*
 * Config object containing various rendering settings.
 *
 * @since 0.0.0
 */
struct RenderConfig {
	/**
	 * When set to `true`, OpenGL uses linear interpolation to draw scaled or
	 * rotated textures, giving a smooth appearance. When set to `false`, OpenGL
	 * uses nearest-neighbor interpolation, giving a crisper appearance.
	 *
	 * @since 0.0.0
	 */
	bool antialias = true;
	
	/**
	 * Sets `antialias` to false and `roundPixels` to true. This is the best
	 * setting for pixel-art games.
	 *
	 * @since 0.0.0
	 */
	bool pixelArt = false;

	/**
	 * Draw texture-based Game Objects at only whole-integer positions.
	 * Game Objects without textures, like Graphics, ignore this property.
	 *
	 * @since 0.0.0
	 */
	bool roundPixels = false;

	 /**
	  * Whether the game canvas will be cleared between each rendering frame.
	 *
	 * @since 0.0.0
	 */
	bool clearBeforeRender = true;

	 /**
	 * If the value is true the OpenGL buffers will not be cleared and will
	 * preserve their values until cleared or overwritten by the author.
	 *
	 * @since 0.0.0
	 */
	bool preserveDrawingBuffer = false;

	/**
	 * The default OpenGL batch size. Represents the number of _quads_ that can
	 * be added to a single batch.
	 *
	 * @since 0.0.0
	 */
	int batchSize = 4096;

	/**
	 * This sets the maximum number of GPU Textures to use. The default, -1, will
	 * use all available units.
	 *
	 * @since 0.0.0
	 */
	int maxTextures = -1;

	/**
	 * The maximum allowed size for any texture for both width and height. The
	 * default, -1, will use the maximum of the hardware, which is different
	 * depending on the device.
	 *
	 * @since 0.0.0
	 */
	int maxTextureSize = -1;

	/**
	 * The mipmap magFilter to be used when creating OpenGL textures.
	 *
	 * @since 0.0.0
	 */
	GLenum mipmapFilter = GL_LINEAR;

	Color backgroundColor;
};

}	// namespace Zen

#endif
