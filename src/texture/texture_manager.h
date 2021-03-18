/**
 * @file		texture_manager.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL2/SDL.h>
#include <map>
#include <string>
#include <functional>
#include <vector>
#include <fstream>
#include "../data.h"
#include "../event/event_emitter.h"
#include "../utils/json.h"
#include "sprite_sheet_config.h"

namespace Zen {
namespace Textures {

/**
 * Textures are managed by the game level TextureManager.
 *
 * It is responsible for creating and delivering textures and their
 * corresponding frames to Game Objects.
 *
 * @since 0.0.0
 */
class TextureManager : public EventEmitter
{
public:
	TextureManager (Game& g);

	/**
	 * The boot handler called by the Game instance when it first starts up.
	 *
	 * @since 0.0.0
	 */
	void boot ();

	/**
	 * Checks the given texture key and to see if it is already in use.
	 *
	 * @since 0.0.0
	 * @param key The texture key to check.
	 * @return `true` if it's safe to use the key, otherwise `false`.
	 */
	bool checkKey (std::string key);

	/**
	 * Removes a Texture from the TextureManager and destroys it. This
	 * will immediately clear all references to it.
	 *
	 * @since 0.0.0
	 * @param key The key of the Texture to remove.
	 *
	 * @return This TextureManager instance.
	 */
	TextureManager& remove (std::string key);

    /**
     * Adds a new Texture to the Texture Manager created from the given Base64
	 * encoded data.
     *
     * @since 0.0.0
     *
     * @param key The unique key of the Texture.
     * @param data The Base64 encoded data.
     *
     * @return This TextureManager instance.
     */
    TextureManager& addBase64 (std::string key, std::string data);

	/**
	 * Adds a new Texture to the TextureManager created from the given image.
	 * 
	 * @since 0.0.0
	 *
	 * @param key The unique key of the Texture.
	 * @param path The path to the image file.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Texture* addImage (std::string key, std::string path);

	/**
	 * Adds a Render Texture to the TextureManager using the given key.
	 *
	 * This allows you to then use the Render Texture as a normal texture for
	 * texture based Game Game Objects like Sprites.
	 *
	 * @since 0.0.0
	 *
	 * @param key The unique key of the Texture.
	 * @param renderTexture The source Render Texture.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Texture* addRenderTexture (std::string key, GameObjects::RenderTexture renderTexture);

	/**
	 * Adds a new Texture Atlas to this TextureManager.
	 *
	 * It can accept either a JSON Array or JSON Hash formats, as exported by Texture Packer and similar software.
	 *
	 * @since 0.0.0
	 *
	 * @param key The unique key of the Texture.
	 * @param sources A vector of paths to the image file.
	 * @param dataPath A path to the Texture Atlas data.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Texture* addAtlas (std::string key, std::vector<std::string> sources, std::string dataPath);
	/**
	 * @overload
	 *
	 * @since 0.0.0
	 *
	 * @param key The unique key of the Texture.
	 * @param sources A path to the image file.
	 * @param dataPath A path to the Texture Atlas data.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Texture* addAtlas (std::string key, std::string source, std::string dataPath);

	/**
	 * Adds a Texture Atlas to this TextureManager.
	 * The frame data of the atlas must be stored in an Array within the JSON.
	 * This is known as a JSON Array in software such as Texture Packer.
	 *
	 * @since 0.0.0
	 *
	 * @param key The unique key of the Texture.
	 * @param sources A path to the image file.
	 * @param data The Texture Atlas data.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Texture* addAtlasJSONArray (std::string key, std::vector<std::string> sources, nlohmann::json data);
	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param key The unique key of the Texture.
	 * @param sources A path to the image file.
	 * @param data A vector of Texture Atlas data.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Texture* addAtlasJSONArray (std::string key, std::vector<std::string> sources, nlohmann::json data);

	/**
	 * Adds a Texture Atlas to this TextureManager.
	 * The frame data of the atlas must be stored in an Object within the JSON.
	 * This is known as a JSON Hash in software such as Texture Packer.
	 *
	 * @since 0.0.0
	 *
	 * @param key The unique key of the Texture.
	 * @param sources A path to the image file.
	 * @param data The Texture Atlas data.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Texture* addAtlasJSONHash (std::string key, std::vector<std::string> sources, nlohmann::json data);

	/**
	 * Adds a Sprite Sheet to this TextureManager.
	 *
	 * A Sprite Sheet is a texture containing different frames, but each
	 * frame is the exact same size and cannot be trimmed or rotated.
	 *
	 * @since 0.0.0
	 *
	 * @param key The unique key of the Texture.
	 * @param path The path to the image file.
	 * @param config The configuration of this Sprite Sheet.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Texture* addSpriteSheet (std::string key, std::string path, SpriteSheetConfig config);

	/**
	 * Adds a Sprite Sheet to this TextureManager, where the Sprite Sheet exists
	 * as a Frame within a Texture Atlas.
	 *
	 * A Sprite Sheet is a texture containing different frames, but each
	 * frame is the exact same size and cannot be trimmed or rotated.
	 *
	 * @since 0.0.0
	 *
	 * @param key The unique key of the Texture.
	 * @param config The configuration of this Sprite Sheet.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Texture* addSpriteSheetFromAtlas (std::string key, SpriteSheetConfig config);

	/**
	 * Creates a new Texture using the given source and dimensions.
	 *
	 * @since 0.0.0
	 *
	 * @param key The unique key of the Texture.
	 * @param sources The path to the image file.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Texture* create (std::string key, std::vector<std::string> sources);

	/**
	 * Checks the given key to see if a Texture using it exists within
	 * this TextureManager.
	 *
	 * @since 0.0.0
	 *
	 * @param key The unique key of the Texture.
	 *
	 * @return `true` if a Texture matching the given key exists in this
	 * TextureManager.
	 */
	bool exists (std::string key = "__DEFAULT");

	/**
	 * Returns a Texture from the TextureManager that matches the given
	 * key.
	 *
	 * If no key is given, returns the "__DEFAULT" Texture.
	 *
	 * If a key is given but not found, returns the "__MISSING" Texture.
	 *
	 * @since 0.0.0
	 *
	 * @param key The unique key of the Texture.
	 *
	 * @return A pointer to the Texture.
	 */
	Texture* get (std::string key = "");

	/**
	 * Takes a Texture key and Frame name and returns a reference to that
	 * Frame, if found.
	 *
	 * @since 0.0.0
	 *
	 * @param key The unique key of the Texture.
	 * @param frame The key of the frame to get from the Texture.
	 *
	 * @return A pointer to the Frame, or `nullptr` if no Frame was found.
	 */
	Frame* getFrame (std::string key, std::string frame);
	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param key The unique key of the Texture.
	 * @param frame The index of the frame to get from the Texture.
	 *
	 * @return A pointer to the Frame, or `nullptr` if no Frame was found.
	 */
	Frame* getFrame (std::string key, int frame);

	/**
	 * Returns a vector with all the keys of all the Textures in this
	 * TextureManager.
	 *
	 * This will exclude the "__DEFAULT" and "__MISSING" keys.
	 *
	 * @since 0.0.0
	 *
	 * @return Vector of Texture keys.
	 */
	std::vector<std::string> getTextureKeys ();

	/**
	 * Given a Texture and an `x` and `y` coordinates, this method will
	 * return a new Color object that has been populated with the
	 * color and alpha values of the pixel at that location in the
	 * Texture.
	 *
	 * If the coordinates are out of bounds, the object will be invalide with
	 * `-1` as color values.
	 *
	 * @since 0.0.0
	 *
	 * @param x The x coordinate of the pixel.
	 * @param y The y coordinate of the pixel.
	 * @param key The unique key of the Texture.
	 * @param frame The key of the frame.
	 *
	 * @return Color object populated with the color value of the requested pixel.
	 */
	Display::Color getPixel (int x, int y, std::string key, std::string frame);
	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param x The x coordinate of the pixel.
	 * @param y The y coordinate of the pixel.
	 * @param key The unique key of the Texture.
	 * @param frame The index of the frame.
	 *
	 * @return Color object populated with the color value of the requested pixel.
	 */
	Display::Color getPixel (int x, int y, std::string key, int frame);

	/**
	 * Given a Texture and an `x` and `y` coordinates, this method will
	 * return a value between 0 and 255 corresponding to the alpha value
	 * of the pixel at that location in the Texture.
	 *
	 * This will return `-1` in case the coordinates are out of bounds.
	 *
	 * @since 0.0.0
	 *
	 * @param x The x coordinate of the pixel.
	 * @param y The y coordinate of the pixel.
	 * @param key The unique key of the Texture.
	 * @param frame The key of the frame.
	 *
	 * @return Alpha value (0-255) of the given pixel, or `-1` if the coordinates were out of bounds.
	 */
	int getPixelAlpha (int x, int y, std::string key, std::string frame);
	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param x The x coordinate of the pixel.
	 * @param y The y coordinate of the pixel.
	 * @param key The unique key of the Texture.
	 * @param frame The index of the frame.
	 *
	 * @return Alpha value (0-255) of the given pixel, or `-1` if the coordinates were out of bounds.
	 */
	int getPixelAlpha (int x, int y, std::string key, int frame);

	/**
	 * Changes the key being used by a Texture to the new key provided.
	 *
	 * The old key is removed, allowing it to be re-used.
	 *
	 * Game Objects are linked to Textures by a reference to the Texture object,
	 * so all existing references will be retained.
	 *
	 * @since 0.0.0
	 *
	 * @param currentKey The current key of the Texture you wish to rename.
	 * @param newKey The new unique key to use for the Texture.
	 *
	 * @return `true` if the Texture key was successfully renamed, otherwise `false`.
	 */
	bool renameTexture (std::string currentKey, std::string newKey);

	/**
	 * Passes all Textures to the given callback.
	 *
	 * The callback is of type `void` and takes in two parameters:
	 * * A vector of Texture pointers.
	 * * A Data object.
	 *
	 * @since 0.0.0
	 *
	 * @param callback The callback function to be sent the Textures.
	 * @param scope The context of the callback execution.
	 * @param data Additional data that will be passed to the callback.
	 */
	template <typename T>
		void each (void (T::* callback)(std::vector<Texture*>, Data), T* scope, Data data = {})
		{
			std::function<void(std::vector<Texture*>, Data)> cb = std::bind(
					callback,
					scope,
					std::placeholders::_1,
					std::placeholders::_2);

			std::vector<Texture*> tex;
			for (auto it = list_.begin(); it != list_.end(); it++) {
				tex.emplace_back(&it->second);
			}

			cb(tex, data);
		}

private:
	/**
	 * The Game that this TextureManager belongs to.
	 *
	 * @since 0.0.0
	 */
	Game& game_;

	/**
	 * Avector that has all the textures that the TextureManager creates.
	 * Textures are assigned to keys so we can access to any texture that this object has directly by key value without iteration.
	 *
	 * @since 0.0.0
	 */
	std::map<std::string, Texture> list_;
};

}	// namespace Textures
}	// namespace Zen

#endif
