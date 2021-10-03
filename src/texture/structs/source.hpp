/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_STRUCTS_SOURCE_HPP
#define ZEN_TEXTURES_STRUCTS_SOURCE_HPP

#include <string>
#include "../../renderer/types/gl_types.hpp"

namespace Zen {

/**
 * A Texture Source is the encapsulation of the actual source data for a Texture.
 *
 * A Texture can contain multiple Texture Sources, which only happens when a
 * multi-atlas is loaded.
 *
 * @struct TextureSource
 * @since 0.0.0
 */
struct TextureSource
{
	/**
	 * The key of the texture this frame is part of.
	 *
	 * @since 0.0.0
	 */
	std::string texture;

	/**
	 * The source used to create this Source.
	 *
	 * Either a path to an image file, or a Base64 image data.
	 *
	 * @property
	 * @since 0.0.0
	 */
	std::string source;

	/**
	 * The resolution of this Source.
	 *
	 * @property
	 * @since 0.0.0
	 */
	double resolution = 1;

	/**
	 * The dimensions of this Source.
	 *
	 * @property
	 * @since 0.0.0
	 */
	int width = 0, height = 0;

	/**
	 * Are the source image dimensions a power of two?
	 *
	 * @since 0.0.0
	 */
	bool isPowerOf2 = false;

	/**
	 * The GL_Texture loaded in memory that this Source represents.
	 *
	 * @property
	 * @since 0.0.0
	 */
	GL_texture glTexture = 0;

	/**
	 * The current texture unit index as assigned by the Renderer.
	 *
	 * @since 0.0.0
	 */
	int glIndex = 0;

	/**
	 * The counter value when this texture was last assigned an index by the Renderer.
	 *
	 * @since 0.0.0
	 */
	int glIndexCounter = -1;
};

}	// namespace Zen

#endif
