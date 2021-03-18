/**
 * @file		texture_manager.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "texture_manager.h"

namespace Zen {
namespace Textures {

TextureManager::TextureManager (Game& g)
	: game_(g)
{
	game_.events.once("SYS_BOOT", &TextureManager::boot, this);
}

void TextureManager::boot ()
{
	addBase64("__DEFAULT", game_.config.defaultImage);
	addBase64("__MISSING", game_.config.missingImage);
	addBase64("__WHITE", game_.config.whiteImage);
}

bool TextureManager::checkKey (std::string key)
{
	if (exists(key)) {
		messageError("The texture key is already in use: ", key);

		return false;
	}

	return true;
}

TextureManager& TextureManager::remove (std::string key)
{
	if (!exists(key)) {
		messageWarning("No texture found matching the key: ", key);
		
		return *this;
	}

	for (auto it = list_.begin(); it != list_.end(); it++) {
		if (it->first == key) {
			list_.erase(it);

			emit("SYS_REMOVE", {{}, {key}});

			break;
		}
	}

	return *this;
}

TextureManager& TextureManager::addBase64 (
		std::string key, std::string data)
{
	return addImage(key, data);
}

Texture* TextureManager::addImage (std::string key, std::string path)
{
	Texture *texture = nullptr;

	if (checkKey(key)) {
		texture = create(key, path);

		int sourceIndex = 0;

		texture->add("__BASE",
				sourceIndex,
				0,
				0,
				texture->source_[sourceIndex].width_,
				texture->source_[sourceIndex].height_);

		emit("SYS_ADD", {{}, {key}});
	}

	return texture;
}

Texture* TextureManager::addRenderTexture (std::string key, GameObjects::RenderTexture renderTexture)
{
	Texture *texture = nullptr;

	if (checkKey(key)) {
		texture = create(key, renderTexture);

		int sourceIndex = 0;

		texture->add("__BASE",
				sourceIndex,
				0,
				0,
				renameTexture.width_,
				renameTexture.height_);

		emit("SYS_ADD", {{}, {key}});
	}

	return texture;
}

Texture* TextureManager::addAtlas (
		std::string key, std::vector<std::string> sources, std::string dataPath)
{
	// Open file
	std::fstream file (dataPath, std::ios::in);

	if (!file) {
		messageError("JSON file couldn't be opened: ", dataPath);

		return nullptr;
	}
	// Create a JSON object
	nlohmann::json data;
	file >> data;

	// Close file
	file.close();

	auto texturesIt = data.find("textures");
	auto framesIt = data.find("frames");

	if ((texturesIt != data.end() && texturesIt->is_array()) ||
		(framesIt != data.end() && framesIt->is_array()))
	{
		return addAtlasJSONArray(key, sources, data);
	}
	else
	{
		return addAtlasJSONHash(key, sources, data);
	}
}

Texture* TextureManager::addAtlas (
		std::string key, std::string source, std::string dataPath)
{
	std::vector<std::string> sources;
	sources.emplace_back(source);

	return addAtlas(key, sources, dataPath);
}

Texture* TextureManager::addAtlasJSONArray (
		std::string key, std::vector<std::string> sources, nlohmann::json data)
{
	Texture *texture = nullptr;

	if (checkKey(key)) {
		texture = create(key, sources);
		
		parseJsonArray(texture, 0, data);

		emit("SYS_ADD", {{}, {key}});
	}

	return texture;
}

Texture* TextureManager::addAtlasJSONArray (
		std::string key, std::vector<std::string> sources, std::vector<nlohmann::json> data)
{
	// Multi-Atlas

	Texture *texture = nullptr;

	if (checkKey(key)) {
		texture = create(key, sources);

		// Multi-pack with on atlas file for all images
		bool singleAtlasFile = (data.size() == 1);

		// Assumes the textures are in the same order in the source array as in the json data
		for (int i = 0; i < texture->source_.size(); i++) {
			auto atlasData = singleAtlasFile ? data[0] : data[i];

			parseJsonArray(texture, i, atlasData);
		}

		emit("SYS_ADD", {{}, {key}});
	}

	return texture;
}

Texture* TextureManager::addAtlasJSONHash (
		std::string key, std::vector<std::string> sources, nlohmann::json data)
{
	Texture *texture = nullptr;

	if (checkKey(key)) {
		texture = create(key, sources);

		Parser.JSONHash(*texture, 0, data);

		emit("SYS_ADD", {{}, {key}});
	}

	return texture;
}

Texture* TextureManager::addAtlasJSONHash (
		std::string key, std::vector<std::string> sources, std::vector<nlohmann::json> data)
{
	Texture *texture = nullptr;

	if (checkKey(key)) {
		texture = create(key, sources);

		for (int i = 0; i < data.size(), i++) {
			Parser.JSONHash(*texture, i, data[i]);
		}

		emit("SYS_ADD", {{}, {key}});
	}

	return texture;
}

Texture* TextureManager::addSpriteSheet (std::string key, std::string path, SpriteSheetConfig config)
{
	Texture *texture = nullptr;

	if (checkKey(key)) {
		texture = create(key, source);

		int width = texture.source_[0].width_;
		int height = texture.source_[0].height;

		Parser.SpriteSheet(*texture, 0, 0, 0, width, height, config);

		emit("SYS_ADD", {{}, {key}});
	}

	return texture;
}

Texture* TextureManager::addSpriteSheetFromAtlas (std::string key, SpriteSheetConfig config)
{
	if (!checkKey(key))
		return nullptr;

	std::string atlasKey = config.atlas;
	int atlasFrame = config.frame;

	if (atlasKey == "" || atlasFrame < 0)
		return nullptr;

	auto atlas = get(atlasKey);
	auto sheet = atlas.get(atlasFrame);

	if (sheet) {
		Texture *texture = create(key, sheet.source_.image_);

		if (sheet.trimmed) {
			// If trimmed, we need to help the parser adjust
			Parser.SpriteSheetFromAtlas(*texture, sheet, config);
		} else {
			Parser.SpriteSheet(*texture, 0, sheet.cutX, sheet.cutY, sheet.cutWidth, cut.cutHeight, config);
		}

		emit ("SYS_ADD", {{}, {key}});

		return texture;
	}

	return nullptr;
}

Texture* TextureManager::create (std::string key, std::vector<std::string> sources)
{
	Texture *texture = nullptr;

	if (checkKey(key)) {
		Texture tex (*this, key, source);

		list_.emplace(key, tex);

		texture = &list_.find(key)->second;
	}

	return texture;
}

bool TextureManager::exists (std::string key)
{
	return (list_.find(key) != list_.end());
}

Texture* TextureManager::get (std::string key)
{
	if (list_.find(key) != list_.end())
		return list_.find(key)->second;
	else
		return &list_.find("__MISSING")->second;
}

Frame* TextureManager::getFrame (std::string key, std::string frame)
{
	if (list_.find(key) != list_.end())
		return &list_.find(key)->second;
}

Frame* TextureManager::getFrame (std::string key, int frame)
{
	if (list_.find(key) != list_.end())
		return &list_.at(frame)->second;
}

std::vector<std::string> TextureManager::getTextureKeys ()
{
	std::vector<std::string> out;

	for (auto it = list_.begin(); it != list_.end(); it++) {
		if (it->first != "__DEFAULT" && it->first != "__MISSING")
			out.emplace_back(it->first);
	}

	return out;
}

Display::Color TextureManager::getPixel (int x, int y, std::string key, std::string frame)
{
	auto textureFrame = getFrame(key, frame);

	Display::Color out;

	if (textureFrame) {
		// Adjust for trim (if not trimmed x and y are just zero)
		x -= textureFrame.x;
		y -= textureFrame.y;

		auto data = textureFrame.data.cut;

		x += data.x;
		y += data.y;

		if (x >= data.x && x < data.r && y >= data.y && y < data.b) {
			void *pixels = nullptr;
			int pitch = 0;

			/*
			// Create blank streamable texture
			SDL_Texture *blank = SDL_CreateTexture(
				game.renderer,
				uformat,
				SDL_GetWindowPixelFormat(game.window.getSDLWindow()),
				SDL_TEXTUREACCESS_STREAMING,
				textureFrame->width_,
				textureFrame->height);

			// Check if the texture was created
			if (!blank) {
				messageError("Unable to create a blank texture: ", SDL_GetError());
				return out;
			}
			*/

			// Get the frame's source SDL texture
			SDL_Texture *texture = textureFrame->source_.sdlTexture_;

			// Check if the texture exists
			if (!texture) {
				messageError("The frame has no source texture: ", SDL_GetError());
				return out;
			}

			// Allocate format from window
			Uint32 uformat = SDL_GetWindowPixelFormat(game.window.getSDLWindow());
			SDL_PixelFormat *format = SDL_AllocFormat(uformat);

			// Lock texture for manipulation
			if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) {
				messageError("Unable to lock texture: ", SDL_GetError());

				SDL_FreeFormat(format);

				return out;
			}

			// Get pixels in unsigned int of 32 bits
			Uint32 *upixels = static_cast<Uint32>(pixels);

			// Read the pixel in `x` and `y`
			Uint32 pixel = upixels[y * pitch + x];

			// Get the color components of the pixel
			Uint8 r = 0, g = 0, b = 0, a = 0;
			SDL_GetRGBA(pixel, format, &r, &g, &b, &a);

			// Unlock texture
			SDL_UnlockTexture(texture);
			pixels = nullptr;
			upixels = nullptr;

			// Free the allocated format
			SDL_FreeFormat(format);

			// Save the color components to the output color object
			out.setTo(r, g, b, a);
		}
	}

	return out;
}

Display::Color TextureManager::getPixel (int x, int y, std::string key, int frame)
{
	auto textureFrame = getFrame(key, frame);

	Display::Color out;

	if (textureFrame) {
		// Adjust for trim (if not trimmed x and y are just zero)
		x -= textureFrame.x;
		y -= textureFrame.y;

		auto data = textureFrame.data.cut;

		x += data.x;
		y += data.y;

		if (x >= data.x && x < data.r && y >= data.y && y < data.b) {
			void *pixels = nullptr;
			int pitch = 0;

			// Get the frame's source SDL texture
			SDL_Texture *texture = textureFrame->source_.sdlTexture_;

			// Check if the texture exists
			if (!texture) {
				messageError("The frame has no source texture: ", SDL_GetError());
				return out;
			}

			// Allocate format from window
			Uint32 uformat = SDL_GetWindowPixelFormat(game.window.getSDLWindow());
			SDL_PixelFormat *format = SDL_AllocFormat(uformat);

			// Lock texture for manipulation
			if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) {
				messageError("Unable to lock texture: ", SDL_GetError());

				SDL_FreeFormat(format);

				return out;
			}

			// Get pixels in unsigned int of 32 bits
			Uint32 *upixels = static_cast<Uint32>(pixels);

			// Read the pixel in `x` and `y`
			Uint32 pixel = upixels[y * pitch + x];

			// Get the color components of the pixel
			Uint8 r = 0, g = 0, b = 0, a = 0;
			SDL_GetRGBA(pixel, format, &r, &g, &b, &a);

			// Unlock texture
			SDL_UnlockTexture(texture);
			pixels = nullptr;
			upixels = nullptr;

			// Free the allocated format
			SDL_FreeFormat(format);

			// Save the color components to the output color object
			out.setTo(r, g, b, a);
		}
	}

	return out;
}

int TextureManager::getPixelAlpha (int x, int y, std::string key, std::string frame)
{
	auto textureFrame = getFrame(key, frame);

	int out = -1;

	if (textureFrame) {
		// Adjust for trim (if not trimmed x and y are just zero)
		x -= textureFrame.x;
		y -= textureFrame.y;

		auto data = textureFrame.data.cut;

		x += data.x;
		y += data.y;

		if (x >= data.x && x < data.r && y >= data.y && y < data.b) {
			void *pixels = nullptr;
			int pitch = 0;

			// Get the frame's source SDL texture
			SDL_Texture *texture = textureFrame->source_.sdlTexture_;

			// Check if the texture exists
			if (!texture) {
				messageError("The frame has no source texture: ", SDL_GetError());
				return out;
			}

			// Allocate format from window
			Uint32 uformat = SDL_GetWindowPixelFormat(game.window.getSDLWindow());
			SDL_PixelFormat *format = SDL_AllocFormat(uformat);

			// Lock texture for manipulation
			if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) {
				messageError("Unable to lock texture: ", SDL_GetError());

				SDL_FreeFormat(format);

				return out;
			}

			// Get pixels in unsigned int of 32 bits
			Uint32 *upixels = static_cast<Uint32>(pixels);

			// Read the pixel in `x` and `y`
			Uint32 pixel = upixels[y * pitch + x];

			// Get the color components of the pixel
			Uint8 a = 0;
			SDL_GetRGBA(pixel, format, nullptr, nullptr, nullptr, &a);

			// Unlock texture
			SDL_UnlockTexture(texture);
			pixels = nullptr;
			upixels = nullptr;

			// Free the allocated format
			SDL_FreeFormat(format);

			// Save the color components to the output variable
			out = a;
		}
	}

	return out;
}

int TextureManager::getPixelAlpha (int x, int y, std::string key, int frame)
{
	auto textureFrame = getFrame(key, frame);

	int out = -1;

	if (textureFrame) {
		// Adjust for trim (if not trimmed x and y are just zero)
		x -= textureFrame.x;
		y -= textureFrame.y;

		auto data = textureFrame.data.cut;

		x += data.x;
		y += data.y;

		if (x >= data.x && x < data.r && y >= data.y && y < data.b) {
			void *pixels = nullptr;
			int pitch = 0;

			// Get the frame's source SDL texture
			SDL_Texture *texture = textureFrame->source_.sdlTexture_;

			// Check if the texture exists
			if (!texture) {
				messageError("The frame has no source texture: ", SDL_GetError());
				return out;
			}

			// Allocate format from window
			Uint32 uformat = SDL_GetWindowPixelFormat(game.window.getSDLWindow());
			SDL_PixelFormat *format = SDL_AllocFormat(uformat);

			// Lock texture for manipulation
			if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) {
				messageError("Unable to lock texture: ", SDL_GetError());

				SDL_FreeFormat(format);

				return out;
			}

			// Get pixels in unsigned int of 32 bits
			Uint32 *upixels = static_cast<Uint32>(pixels);

			// Read the pixel in `x` and `y`
			Uint32 pixel = upixels[y * pitch + x];

			// Get the color components of the pixel
			Uint8 a = 0;
			SDL_GetRGBA(pixel, format, nullptr, nullptr, nullptr, &a);

			// Unlock texture
			SDL_UnlockTexture(texture);
			pixels = nullptr;
			upixels = nullptr;

			// Free the allocated format
			SDL_FreeFormat(format);

			// Save the color components to the output variable
			out = a;
		}
	}

	return out;
}

bool TextureManager::renameTexture (std::string currentKey, std::string newKey)
{
	Texture *texture = get(currentKey);

	if (texture && currentKey != newKey) {
		texture.key_ = newKey;

		// Move the node containing the texture without copying
		auto nodeHandler = list_.extract(currentKey);
		nodeHandler.key() = newKey;
		list_.insert(std::move(nodeHandler));

		return true;
	}

	return false;
}

}	// namespace Textures
}	// namespace Zen
