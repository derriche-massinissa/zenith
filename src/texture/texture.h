/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_TEXTURE_H
#define ZEN_TEXTURES_TEXTURE_H

#include <map>
#include <string>
#include <vector>

#include "../data.h"
#include "../display/color.h"

#include "frame.h"
#include "texture_source.h"

#include "texture_manager.fwd.h"

namespace Zen {
namespace Textures {

/**
 * A Texture consists of a source, usually an Image from the Cache, and a
 * collection of Frames.
 * The Frames represent the different areas of the Texture. For example a
 * texture atlas may have many Frames, one for each element within the atlas.
 * Where-as a single image would have just one frame, that encompasses the
 * whole image.
 *
 * Every Texture, no matter where it comes from, always has at least 1 frame
 * called the `__BASE` frame. This frame represents the entirety of the
 * source image.
 *
 * Textures are managed by the global TextureManager. This class is responsible
 * for creating and delivering Textures and their corresponding Frames
 * to Game Objects.
 *
 * Sprites and other Game Objects get the texture data they need from the 
 * TextureManager.
 *
 * @class Texture
 * @since 0.0.0
 */
class Texture
{
public:
	/**
	 * @since 0.0.0
	 *
	 * @param manager_ A reference to the Texture Manager this Texture belongs to.
	 * @param key_ The unique key of this Texture.
	 * @param sources_ A vector of paths to the files that are used to create
	 * the texture.
	 */
	Texture(TextureManager& manager_, std::string key_, std::vector<std::string> sources_);

	/**
	 * Adds a new Frame to this Texture.
	 *
	 * A Frame is a rectangular region of a Texture with a unique index
	 * or string-based key.
	 *
	 * The name given must be unique within this Texture. If it already exists, this method will return `null`.
	 *
	 * @since 0.0.0
	 *
	 * @param name_ The name of this Frame. The name is unique within the Texture.
	 * @param sourceIndex_ - The index of the TextureSource that this Frame is a part of.
	 * @param x_ The x coordinate of the top-left of this Frame.
	 * @param y_ The y coordinate of the top-left of this Frame.
	 * @param width_ The width of this Frame.
	 * @param height_ The height of this Frame.
	 *
	 * @return A pointer to the Frame that was added to this Texture, or `null`
	 * if the given name already exists.
	 */
	Frame* add (std::string name_, int sourceIndex_, int x_, int y_, int width_, int height_);
	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param name_ The name of this Frame. The name is unique within the Texture.
	 * @param sourceIndex_ - The index of the TextureSource that this Frame is a part of.
	 * @param x_ The x coordinate of the top-left of this Frame.
	 * @param y_ The y coordinate of the top-left of this Frame.
	 * @param width_ The width of this Frame.
	 * @param height_ The height of this Frame.
	 *
	 * @return A pointer to the Frame that was added to this Texture, or `null`
	 * if the given name already exists.
	 */
	Frame* add (int name_, int sourceIndex_, int x_, int y_, int width_, int height_);

	/**
	 * Removes the given Frame from this Texture. The Frame is destroyed immediately.
	 *
	 * Any Game Objects using this Frame should stop using it _before_ you remove it,
	 * as it does not happen automatically.
	 *
	 * @since 0.0.0
	 *
	 * @param name_ The key of the Frame to remove.
	 *
	 * @return `true` if a Frame with the matching key was removed from
	 * this Texture.
	 */
	bool remove (std::string name_);

	/**
	 * Checks to see if a Frame matching the given key exists within this Texture.
	 *
	 * @since 0.0.0
	 *
	 * @param name_ The key of the Frame to check for.
	 *
	 * @return `true` if a Frame with the matching key exists in this Texture.
	 */
	bool has (std::string name_);

	/**
	 * Gets a Frame from this Texture based on either the key or the index of
	 * the Frame.
	 *
	 * In a Texture Atlas Frames are typically referenced by a key.
	 * In a Sprite Sheet Frames are referenced by an index.
	 * Passing no value for the name returns the base texture.
	 *
	 * @since 0.0.0
	 *
	 * @param name_ The name of the Frame to get from this Texture.
	 *
	 * @return The Texture Frame.
	 */
	Frame* get (std::string name_ = "");
	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param index_ The index, of the Frame to get from this Texture.
	 *
	 * @return The Texture Frame.
	 */
	Frame* get (int index_);

	/**
	 * Takes the given TextureSource and returns the index of it within
	 * this Texture. If it's not in this Texture, it returns -1.
	 *
	 * Unless this Texture has multiple TextureSources, such as with a
	 * multi-atlas, this method will always return zero or -1.
	 *
	 * @since 0.0.0
	 *
	 * @param source_ The TextureSource to check.
	 *
	 * @return The index of the TextureSource within this Texture, or -1 if not in this Texture.
	 */
	int getTextureSourceIndex (TextureSource& source_);

	/**
	 * Returns an array of all the Frames in the given TextureSource.
	 *
	 * @since 0.0.0
	 *
	 * @param sourceIndex_ The index of the TextureSource to get the Frames from.
	 * @param includeBase_ Include the `__BASE` Frame in the output array?
	 *
	 * @return An array of Texture Frames.
	 */
	std::vector<Frame*> getFramesFromTextureSource (int sourceIndex_, bool includeBase_ = false);

	/**
	 * Returns an array with all of the names of the Frames in this Texture.
	 *
	 * Useful if you want to randomly assign a Frame to a Game Object, as you can
	 * pick a random element from the returned array.
	 *
	 * @since 0.0.0
	 *
	 * @param includeBase_ Include the `__BASE` Frame in the output array?
	 *
	 * @return An array of all Frame names in this Texture.
	 */
	std::vector<std::string> getFrameNames (bool includeBase_ = false);

	/**
	 * A reference to the Texture Manager this Texture belongs to.
	 *
	 * @since 0.0.0
	 */
	TextureManager* manager;

	/**
	 * The unique key of this Texture.
	 *
	 * @since 0.0.0
	 */
	std::string key;

	/**
	 * A vector of TextureSource instances.
	 *
	 * These are unique to this Texture and contain the actual image data.
	 *
	 * @since 0.0.0
	 */
	std::vector<TextureSource> source;

	/**
	 * A map associating the unique Frame keys with the Frames of this Texture.
	 *
	 * @since 0.0.0
	 */
	std::map<std::string, Frame> frames;

	/**
	 * Any additional data that was set in the source JSON (if any), or any
	 * extra data you'd like to store relating to this texture.
	 *
	 * @since 0.0.0
	 */
	Data data;

	/**
	 * The name of the first frame of the Texture.
	 *
	 * @since 0.0.0
	 */
	std::string firstFrame = "__BASE";

	/**
	 * The total number of Frames in this Texture, including the `__BASE` frame.
	 *
	 * A Texture will always contain at least 1 frame because every Texture
	 * contains a `__BASE` frame by default,
	 * in addition to any extra frames that have been added to it, such as when
	 * parsing a Sprite Sheet or Texture Atlas.
	 *
	 * @since 0.0.0
	 */
	int frameTotal = 0;
};	// class Texture

}	// namespace Textures
}	// namespace Zen

#endif
