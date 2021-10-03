/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_STRUCTS_TEXTURE_HPP
#define ZEN_TEXTURES_STRUCTS_TEXTURE_HPP

#include <string>
#include <vector>
#include <map>
#include "source.hpp"
#include "frame.hpp"

namespace Zen {

/**
 * A Texture consists of a source, and a collection of Frames. The Frames represent
 * the different areas of the Texture. For example a texture atlas may have many
 * Frames, one for each element within the atlas. Where-as a single image would
 * have just one frame, that encompasses the whole image.
 *
 * Every Texture, no matter where it comes from, always has at least 1 frame called
 * the `__BASE` frame.  This frame represents the entirety of the source image.
 *
 * Textures are managed by the global TextureManager. This is a class that is
 * responsible for creating and delivering Textures and their corresponding Frames
 * to Game Objects.
 *
 * Sprites and other Game Objects get the texture data they need from the
 * TextureManager.
 *
 * @struct Texture
 * @since 0.0.0
 */
struct Texture
{
	/**
	 * The unique key of this Texture.
	 *
	 * @property
	 * @since 0.0.0
	 */
	std::string key;

	/**
	 * An array of TextureSource instances.
	 * These are unique to this Texture and contain the actual image data.
	 *
	 * @since 0.0.0
	 */
	std::vector<TextureSource> sources;

	/**
	 * An array of TextureSource data instances.
	 * Used to store additional data images, such as normal maps or specular maps.
	 *
	 * @since 0.0.0
	 */
	std::vector<TextureSource> dataSources;

	/**
	 * A map associating the unique Frame keys with the Frames objects.
	 *
	 * @since 0.0.0
	 */
	std::map<std::string, TextureFrame> frames;

	/**
	 * The key of the first frame of the Texture.
	 *
	 * @since 0.0.0
	 */
	std::string firstFrame = "__BASE";

	/**
	 * The total number of Frames in this Texture, including the `__BASE` frame.
	 *
	 * A Texture will always contain at least 1 frame because every Texture
	 * contains a `__BASE` frame by default, in addition to any extra frames that
	 * have been added to it, such as when parsing a Sprite Sheet or Texture Atlas.
	 *
	 * @since 0.0.0
	 */
	int frameTotal;
};

}	// namespace Zen

#endif
