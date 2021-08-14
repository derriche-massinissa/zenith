/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_TEXTURE_MANAGER_HPP
#define ZEN_TEXTURES_TEXTURE_MANAGER_HPP

#include <SDL2/SDL.h>
#include <map>
#include <string>
#include <functional>
#include <vector>

#include "json/json.hpp"

#include "../data.h"
#include "../event/event_emitter.hpp"
#include "../display/types/color.hpp"
#include "sprite_sheet_config.hpp"
#include "components/texture.hpp"

#include "../core/config.fwd.hpp"

namespace Zen {

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
	~TextureManager ();

	/**
	 * The boot handler called by the Game instance when it first starts up.
	 *
	 * @since 0.0.0
	 */
	void boot (GameConfig *config_);

	/**
	 * Checks the given texture key to see if it is already in use.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The texture key to check.
	 * @return `true` if it's safe to use the key, otherwise `false`.
	 */
	bool checkKey (std::string key_);

	/**
	 * Removes a Texture from the TextureManager and destroys it. This
	 * will immediately clear all references to it.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The key of the Texture to remove.
	 *
	 * @return This TextureManager instance.
	 */
	TextureManager& remove (std::string key_);

    /**
     * Adds a new Texture to the Texture Manager created from the given Base64
	 * encoded data.
     *
     * @since 0.0.0
     *
     * @param key_ The unique key of the Texture.
     * @param data_ The Base64 encoded data.
     *
     * @return This TextureManager instance.
     */
    Entity addBase64 (std::string key_, std::string data_);

	/**
	 * Adds a new Texture to the TextureManager created from the given image.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The unique key of the Texture.
	 * @param path_ The path to the image file.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Entity addImage (std::string key_, std::string path_);

	/**
	 * Adds a Render Texture to the TextureManager using the given key.
	 *
	 * This allows you to then use the Render Texture as a normal texture for
	 * texture based Game Game Objects like Sprites.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The unique key of the Texture.
	 * @param renderTexture_ The source Render Texture.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Entity addRenderTexture (std::string key_, Entity renderTexture_);

	/**
	 * Adds a new Texture Atlas to this TextureManager.
	 *
	 * It can accept either a JSON Array or JSON Hash formats, as exported by Texture Packer and similar software.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The unique key of the Texture.
	 * @param sources_ A vector of paths to the image file.
	 * @param dataPath_ A path to the Texture Atlas data.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Entity addAtlas (std::string key_, std::vector<std::string> sources_, std::string dataPath_);
	/**
	 * @overload
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The unique key of the Texture.
	 * @param sources_ A path to the image file.
	 * @param dataPath_ A path to the Texture Atlas data.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Entity addAtlas (std::string key_, std::string source_, std::string dataPath_);

	/**
	 * Adds a Texture Atlas to this TextureManager.
	 * The frame data of the atlas must be stored in an Array within the JSON.
	 * This is known as a JSON Array in software such as Texture Packer.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The unique key of the Texture.
	 * @param sources_ A path to the image file.
	 * @param data_ The Texture Atlas data.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Entity addAtlasJSONArray (std::string key_, std::vector<std::string> sources_, nlohmann::json data_);

	/**
	 * Adds a Texture Atlas to this TextureManager.
	 * The frame data of the atlas must be stored in an Object within the JSON.
	 * This is known as a JSON Hash in software such as Texture Packer.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The unique key of the Texture.
	 * @param sources_ A path to the image file.
	 * @param data_ The Texture Atlas data.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Entity addAtlasJSONHash (std::string key_, std::vector<std::string> sources_, nlohmann::json data_);

	/**
	 * Adds a Texture Atlas to this TextureManager.
	 * The frame data of the atlas must be stored in an Object within the JSON.
	 * This is known as a JSON Hash in software such as Texture Packer.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The unique key of the Texture.
	 * @param sources_ A path to the image file.
	 * @param data_ The Texture Atlas data.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Entity addAtlasJSONHash (std::string key_, std::vector<std::string> sources_, std::vector<nlohmann::json> data_);

	/**
	 * Adds a Sprite Sheet to this TextureManager.
	 *
	 * A Sprite Sheet is a texture containing different frames, but each
	 * frame is the exact same size and cannot be trimmed or rotated.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The unique key of the Texture.
	 * @param path_ The path to the image file.
	 * @param config_ The configuration of this Sprite Sheet.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Entity addSpriteSheet (std::string key_, std::string path_, SpriteSheetConfig config_);

	/**
	 * Adds a Sprite Sheet to this TextureManager, where the Sprite Sheet exists
	 * as a Frame within a Texture Atlas.
	 *
	 * A Sprite Sheet is a texture containing different frames, but each
	 * frame is the exact same size and cannot be trimmed or rotated.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The unique key of the Texture.
	 * @param config_ The configuration of this Sprite Sheet.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Entity addSpriteSheetFromAtlas (std::string key_, SpriteSheetConfig config_);

	/**
	 * Creates a new Texture using the given source and dimensions.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The unique key of the Texture.
	 * @param sources_ A vector of file paths.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Entity create (std::string key_, std::vector<std::string> sources_);

	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param key_ The unique key of the Texture.
	 * @param sources_ The path to the image file.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Entity create (std::string key_, std::string source_);

	/**
	 * @todo Create texture from renderTexture game object.
	 *
	 * @overload
	 * @since 0.0.0
	 *
	 * @param key_ The unique key of the Texture.
	 * @param renderTexture_ The RenderTexture GameObject to use as a source.
	 *
	 * @return A pointer to the newly created Texture, or `nullptr` if the key
	 * is already in use.
	 */
	Entity create (std::string key_, Entity renderTexture_);

	/**
	 * Checks the given key to see if a Texture using it exists within
	 * this TextureManager.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The unique key of the Texture.
	 *
	 * @return `true` if a Texture matching the given key exists in this
	 * TextureManager.
	 */
	bool exists (std::string key_ = "__DEFAULT");

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
	 * @param key_ The unique key of the Texture.
	 *
	 * @return A pointer to the Texture.
	 */
	Entity get (std::string key_ = "");

	/**
	 * Takes a Texture key and Frame name and returns a pointer to that
	 * Frame, if found.
	 *
	 * @since 0.0.0
	 *
	 * @param key_ The unique key of the Texture.
	 * @param frame_ The key of the frame to get from the Texture.
	 *
	 * @return A pointer to the Frame, or `nullptr` if no Frame was found.
	 */
	Entity getFrame (std::string key_, std::string frame_);
	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param key_ The unique key of the Texture.
	 * @param frame_ The index of the frame to get from the Texture.
	 *
	 * @return A pointer to the Frame, or `nullptr` if no Frame was found.
	 */
	Entity getFrame (std::string key_, int frame_);

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
	 * @param x_ The x coordinate of the pixel.
	 * @param y_ The y coordinate of the pixel.
	 * @param key_ The unique key of the Texture.
	 * @param frame_ The key of the frame.
	 *
	 * @return Color object populated with the color value of the requested pixel.
	 */
	Color getPixel (int x_, int y_, std::string key_, std::string frameName_);
	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param x_ The x coordinate of the pixel.
	 * @param y_ The y coordinate of the pixel.
	 * @param key_ The unique key of the Texture.
	 * @param frame_ The index of the frame.
	 *
	 * @return Color object populated with the color value of the requested pixel.
	 */
	Color getPixel (int x_, int y_, std::string key_, int frameIndex_);

	/**
	 * Given a frame and an `x` and `y` coordinates, this method will
	 * return a value between 0 and 255 corresponding to the alpha value
	 * of the pixel at that location in the Texture.
	 *
	 * This will return `-1` in case the coordinates are out of bounds.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The x coordinate of the pixel.
	 * @param y_ The y coordinate of the pixel.
	 * @param textureFrame_ The frame entity.
	 *
	 * @return Alpha value (0-255) of the given pixel, or `-1` if the coordinates were out of bounds.
	 */
	int getPixelAlpha (int x_, int y_, Entity textureFrame_);
	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param x_ The x coordinate of the pixel.
	 * @param y_ The y coordinate of the pixel.
	 * @param key_ The unique key of the Texture.
	 * @param frame_ The key of the frame.
	 *
	 * @return Alpha value (0-255) of the given pixel, or `-1` if the coordinates were out of bounds.
	 */
	int getPixelAlpha (int x_, int y_, std::string key_, std::string frameName_);
	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param x_ The x coordinate of the pixel.
	 * @param y_ The y coordinate of the pixel.
	 * @param key_ The unique key of the Texture.
	 * @param frame_ The index of the frame.
	 *
	 * @return Alpha value (0-255) of the given pixel, or `-1` if the coordinates were out of bounds.
	 */
	int getPixelAlpha (int x_, int y_, std::string key_, int frameIndex_);

	void createAlphaCache (std::string key_);

	/*
	 * Changes the key being used by a Texture to the new key provided.
	 *
	 * The old key is removed, allowing it to be re-used.
	 *
	 * Game Objects are linked to Textures by a reference to the Texture object,
	 * so all existing references will be retained.
	 *
	 * @since 0.0.0
	 *
	 * @param currentKey_ The current key of the Texture you wish to rename.
	 * @param newKey_ The new unique key to use for the Texture.
	 *
	 * @return `true` if the Texture key was successfully renamed, otherwise `false`.
	//bool renameTexture (std::string currentKey_, std::string newKey_);
	 */

	/**
	 * Passes all Textures to the given callback.
	 *
	 * The callback is of type `void` and takes in two parameters:
	 * * A vector of Texture pointers.
	 * * A Data object.
	 *
	 * @since 0.0.0
	 *
	 * @param callback_ The callback function to be sent the Textures.
	 * @param scope_ The context of the callback execution.
	 * @param data_ Additional data that will be passed to the callback.
	 */
	template <typename T>
	void each (void (T::* callback_)(std::vector<Entity>, Data), T* scope_, Data data_ = {})
	{
		std::function<void(std::vector<Entity>, Data)> cb_ = std::bind(
				callback_,
				scope_,
				std::placeholders::_1,
				std::placeholders::_2);

		std::vector<Entity> tex_;

		for (auto it_ = list.begin(); it_ != list.end(); it_++) {
			tex_.emplace_back(&it_->second);
		}

		cb_(tex_, data_);
	}

	/**
	 * The Game that this TextureManager belongs to.
	 *
	 * @since 0.0.0
	 */
	GameConfig *config;

private:
	/**
	 * Avector holding all the textures that the TextureManager creates.
	 * Textures are assigned to keys so we can access to any texture that this
	 * object has directly by key value without iteration.
	 *
	 * @since 0.0.0
	 */
	std::map<std::string, Entity> list;

	/**
	 * TextureSource - Pixel array
	 *
	 * @since 0.0.0
	 */
	std::map<Entity, SDL_Surface*> alphaCache;
};

}	// namespace Zen

#endif
