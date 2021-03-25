/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_SYSTEMS_TEXTURE_HPP
#define ZEN_TEXTURES_SYSTEMS_TEXTURE_HPP

#include "../../ecs/entity.hpp"
#include <vector>

namespace Zen {

/**
 * @since 0.0.0
 *
 * @param key_ The unique key of the Texture.
 * @param sources_ A vector of paths to the files that are used to create
 * the texture.
 */
Entity CreateTexture (const char* key, std::vector<const char*> sources);

/**
 * Adds a new Frame to this Texture.
 *
 * A Frame is a rectangular region of a Texture with a unique index
 * or string-based key.
 *
 * The name given must be unique within this Texture. If it already exists,
 * this method will return `null`.
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
Entity AddFrame (Entity texture, const char* name, int sourceIndex, int x, int y, int width, int height);

/**
 * @overload
 * @since 0.0.0
 *
 * @param name The name of this Frame. The name is unique within the Texture.
 * @param sourceIndex - The index of the TextureSource that this Frame is a part of.
 * @param x The x coordinate of the top-left of this Frame.
 * @param y The y coordinate of the top-left of this Frame.
 * @param width The width of this Frame.
 * @param height The height of this Frame.
 *
 * @return A pointer to the Frame that was added to this Texture, or `null`
 * if the given name already exists.
 */
Entity AddFrame (Entity texture, int index, int sourceIndex, int x, int y, int width, int height);

/**
 * Removes the given Frame from this Texture. The Frame is destroyed
 * immediately.
 * Any Game Objects using this Frame should stop using it _before_ you remove
 * it, as it does not happen automatically.
 *
 * @since 0.0.0
 *
 * @param name The key of the Frame to remove.
 *
 * @return `true` if a Frame with the matching key was removed from
 * this Texture.
 */
bool RemoveFrame (Entity texture, std::string name);

/**
 * @overload
 * @since 0.0.0
 *
 * @param index The index of the Frame to remove.
 *
 * @return `true` if a Frame with the matching key was removed from
 * this Texture.
 */
bool RemoveFrame (Entity texture, int index);

/**
 * Checks to see if a Frame matching the given key exists within this Texture.
 *
 * @since 0.0.0
 *
 * @param name The key of the Frame to check for.
 *
 * @return `true` if a Frame with the matching key exists in this Texture.
 */
bool HasFrame (Entity texture, std::string name);

/**
 * @overload
 * @since 0.0.0
 *
 * @param index The index of the Frame to check for.
 *
 * @return `true` if a Frame with the matching key exists in this Texture.
 */
bool HasFrame (Entity texture, int index);

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
 * @param name The name of the Frame to get from this Texture.
 *
 * @return The Texture Frame.
 */
Entity GetFrame (Entity texture, std::string name = "");

/**
 * @overload
 * @since 0.0.0
 *
 * @param index The index of the Frame to get from this Texture.
 *
 * @return The Texture Frame.
 */
Entity GetFrame (Entity texture, int index);

/**
 * Takes the given TextureSource and returns the index of it within
 * this Texture. If it's not in this Texture, it returns -1.
 *
 * Unless this Texture has multiple TextureSources, such as with a
 * multi-atlas, this method will always return zero or -1.
 *
 * @since 0.0.0
 *
 * @param source The TextureSource to check.
 *
 * @return The index of the TextureSource within this Texture, or -1 if not in this Texture.
 */
int GetTextureSourceIndex (Entity source);

/**
 * Returns an array of all the Frames in the given TextureSource.
 *
 * @since 0.0.0
 *
 * @param sourceIndex The index of the TextureSource to get the Frames from.
 * @param includeBase Include the `__BASE` Frame in the output array?
 *
 * @return An array of Texture Frames.
 */
std::vector<Entity> GetFramesFromSource (Entity source, bool includeBase = false);

/**
 * Returns an array with all of the names of the Frames in the given Texture.
 *
 * Useful if you want to randomly assign a Frame to a Game Object, as you can
 * pick a random element from the returned array.
 *
 * @since 0.0.0
 *
 * @param includeBase Include the `__BASE` Frame in the output array?
 *
 * @return An array of all Frame names in this Texture.
 */
std::vector<std::string> GetFrameNames (Entity texture, bool includeBase = false);

}	// namespace Zen

#endif
