/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "texture_manager.h"

#include "parsers/json_array.h"
#include "parsers/json_hash.h"
#include "parsers/sprite_sheet.h"
#include "parsers/sprite_sheet_atlas.h"

#include "../core/game.h"
#include "../gameobjects/rendertexture/render_texture.h"
#include "texture_source.h"
#include "frame.h"
#include <tuple>
#include <utility>

namespace Zen {
namespace Textures {

TextureManager::TextureManager (Game* game_)
	: game (game_)
{
	game_->events.once("boot", &TextureManager::boot, this);
}

void TextureManager::boot ()
{
	addBase64("__DEFAULT", game->config.defaultImage);
	addBase64("__MISSING", game->config.missingImage);
	addBase64("__WHITE", game->config.whiteImage);
}

bool TextureManager::checkKey (std::string key_)
{
	if (exists(key_))
	{
		messageError("The texture key is already in use: ", key_);

		return false;
	}

	return true;
}

TextureManager& TextureManager::remove (std::string key_)
{
	if (!exists(key_))
	{
		messageWarning("No texture found matching the key: ", key_);

		return *this;
	}

	for (auto it_ = list.begin(); it_ != list.end(); it_++)
	{
		if (it_->first == key_)
		{
			list.erase(it_);

			emit("remove", key_);

			break;
		}
	}

	return *this;
}

Texture* TextureManager::addBase64 (
		std::string key_, std::string data_)
{
	return addImage(key_, data_);
}

Texture* TextureManager::addImage (std::string key_, std::string path_)
{
	Texture *texture_ = nullptr;

	if (checkKey(key_))
	{
		texture_ = create(key_, path_);

		int sourceIndex_ = 0;

		texture_->add("__BASE",
				sourceIndex_,
				0,
				0,
				texture_->source[sourceIndex_].width,
				texture_->source[sourceIndex_].height);

		emit("add", key_);
	}

	return texture_;
}

Texture* TextureManager::addRenderTexture (std::string key_, GameObjects::RenderTexture& renderTexture_)
{
	Texture *texture_ = nullptr;

	if (checkKey(key_))
	{
		texture_ = create(key_, renderTexture_);

		/*
		 * TODO
		 int sourceIndex_ = 0;

		 texture_->add("__BASE",
		 sourceIndex_,
		 0,
		 0,
		 renderTexture_.width_,
		 renderTexture_.height_);

		 emit("add", key_);
		 */
	}

	return texture_;
}

Texture* TextureManager::addAtlas (
		std::string key_, std::vector<std::string> sources_, std::string dataPath_)
{
	// Open file
	std::fstream file_ (dataPath_, std::ios::in);

	if (!file_)
	{
		messageError("JSON file couldn't be opened: ", dataPath_);

		return nullptr;
	}
	// Create a JSON object
	nlohmann::json data_;
	file_ >> data_;

	// Close file
	file_.close();

	auto texturesIt_ = data_.find("textures");
	auto framesIt_ = data_.find("frames");

	if ((texturesIt_ != data_.end() && texturesIt_->is_array()) ||
			(framesIt_ != data_.end() && framesIt_->is_array()))
	{
		return addAtlasJSONArray(key_, sources_, data_);
	}
	else
	{
		return addAtlasJSONHash(key_, sources_, data_);
	}
}

Texture* TextureManager::addAtlas (
		std::string key_, std::string source_, std::string dataPath_)
{
	std::vector<std::string> sources_;
	sources_.emplace_back(source_);

	return addAtlas(key_, sources_, dataPath_);
}

Texture* TextureManager::addAtlasJSONArray (
		std::string key_, std::vector<std::string> sources_, nlohmann::json data_)
{
	Texture *texture_ = nullptr;

	if (checkKey(key_))
	{
		texture_ = create(key_, sources_);

		parseJsonArray(texture_, 0, data_);

		emit("add", key_);
	}

	return texture_;
}

Texture* TextureManager::addAtlasJSONArray (
		std::string key_, std::vector<std::string> sources_, std::vector<nlohmann::json> data_)
{
	// Multi-Atlas

	Texture *texture_ = nullptr;

	if (checkKey(key_))
	{
		texture_ = create(key_, sources_);

		// Multi-pack with on atlas file for all images
		bool singleAtlasFile_ = (data_.size() == 1);

		// Assumes the textures are in the same order in the source array as in the json data
		for (int i_ = 0; i_ < texture_->source.size(); i_++)
		{
			auto atlasData_ = singleAtlasFile_ ? data_[0] : data_[i_];

			parseJsonArray(texture_, i_, atlasData_);
		}

		emit("add", key_);
	}

	return texture_;
}

Texture* TextureManager::addAtlasJSONHash (
		std::string key_, std::vector<std::string> sources_, nlohmann::json data_)
{
	Texture *texture_ = nullptr;

	if (checkKey(key_))
	{
		texture_ = create(key_, sources_);

		parseJsonHash(texture_, 0, data_);

		emit("add", key_);
	}

	return texture_;
}

Texture* TextureManager::addAtlasJSONHash (
		std::string key_, std::vector<std::string> sources_, std::vector<nlohmann::json> data_)
{
	Texture *texture_ = nullptr;

	if (checkKey(key_))
	{
		texture_ = create(key_, sources_);

		for (int i_ = 0; i_ < data_.size(); i_++)
		{
			parseJsonHash(texture_, i_, data_[i_]);
		}

		emit("add", key_);
	}

	return texture_;
}

Texture* TextureManager::addSpriteSheet (std::string key_, std::string path_, SpriteSheetConfig config_)
{
	Texture *texture_ = nullptr;

	if (checkKey(key_))
	{
		texture_ = create(key_, path_);

		int width_ = texture_->source[0].width;
		int height_ = texture_->source[0].height;

		parseSpriteSheet(texture_, 0, 0, 0, width_, height_, config_);

		emit("add", key_);
	}

	return texture_;
}

Texture* TextureManager::addSpriteSheetFromAtlas (std::string key_, SpriteSheetConfig config_)
{
	if (!checkKey(key_))
		return nullptr;

	std::string atlasKey_ = config_.atlas;
	std::string atlasFrame_ = config_.frame;

	if (atlasKey_ == "" || atlasFrame_ == "")
		return nullptr;

	Texture* atlas_ = get(atlasKey_);
	Frame* sheet_ = atlas_->get(atlasFrame_);

	if (sheet_)
	{
		Texture *texture_ = create(key_, sheet_->source->source);

		if (sheet_->isTrimmed())
		{
			// If trimmed, we need to help the parser adjust
			parseSpriteSheetFromAtlas(texture_, sheet_, config_);
		}
		else
		{
			parseSpriteSheet(texture_, 0, sheet_->cutX, sheet_->cutY, sheet_->cutWidth, sheet_->cutHeight, config_);
		}

		emit("add", key_);

		return texture_;
	}

	return nullptr;
}

Texture* TextureManager::create (std::string key_, std::vector<std::string> sources_)
{
	Texture *texture_ = nullptr;

	if (checkKey(key_))
	{
		/*
		Texture tex_ (*this, key_, sources_);

		list.emplace(key_, tex_);
		*/

		list.emplace(
				std::piecewise_construct,
				std::forward_as_tuple(key_),
				std::forward_as_tuple(*this, key_, sources_)
				);

		texture_ = &list.find(key_)->second;
	}

	return texture_;
}

Texture* TextureManager::create (std::string key_, std::string source_)
{
	return create(key_, std::vector<std::string> {source_});
}

Texture* TextureManager::create (std::string key_, GameObjects::RenderTexture& renderTexture_)
{
	// TODO
}

bool TextureManager::exists (std::string key_)
{
	return (list.find(key_) != list.end());
}

Texture* TextureManager::get (std::string key_)
{
	auto textureIterator_ = list.find(key_);

	if (textureIterator_ != list.end())
	{
		return &list.find(key_)->second;
	}
	else
	{
		return &list.find("__MISSING")->second;
	}
}

Frame* TextureManager::getFrame (std::string key_, std::string frame_)
{
	auto textureIterator_ = list.find(key_);

	if (textureIterator_ != list.end())
	{
		return list.find(key_)->second.get(frame_);
	}
	else
	{
		return nullptr;
	}
}

Frame* TextureManager::getFrame (std::string key_, int frame_)
{
	return getFrame(key_, std::to_string(frame_));
}

std::vector<std::string> TextureManager::getTextureKeys ()
{
	std::vector<std::string> out_;

	for (auto it_ = list.begin(); it_ != list.end(); it_++)
	{
		if (it_->first != "__DEFAULT" && it_->first != "__MISSING")
			out_.emplace_back(it_->first);
	}

	return out_;
}

Display::Color TextureManager::getPixel (int x_, int y_, std::string key_, std::string frame_)
{
	auto textureFrame_ = getFrame(key_, frame_);

	Display::Color out_;

	if (textureFrame_)
	{
		// Adjust for trim (if not trimmed x and y are just zero)
		x_ -= textureFrame_->x;
		y_ -= textureFrame_->y;

		auto data_ = textureFrame_->data.cut;

		x_ += data_.x;
		y_ += data_.y;

		if (x_ >= data_.x && x_ < data_.getRight() && y_ >= data_.y && y_ < data_.getBottom())
		{
			void *pixels_ = nullptr;
			int pitch_ = 0;

			// Get the frame's source SDL texture
			SDL_Texture *texture_ = textureFrame_->source->sdlTexture;

			// Check if the texture exists
			if (!texture_)
			{
				messageError("The frame has no source texture: ", SDL_GetError());
				return out_;
			}

			// Allocate format from window
			Uint32 uformat_ = SDL_GetWindowPixelFormat(game->window.window);
			SDL_PixelFormat *format_ = SDL_AllocFormat(uformat_);

			// Lock texture for manipulation
			if (SDL_LockTexture(texture_, nullptr, &pixels_, &pitch_) != 0)
			{
				messageError("Unable to lock texture: ", SDL_GetError());

				SDL_FreeFormat(format_);

				return out_;
			}

			// Get pixels in unsigned int of 32 bits
			Uint32 *upixels_ = static_cast<Uint32*>(pixels_);

			// Read the pixel in `x` and `y`
			Uint32 pixel_ = upixels_[y_ * pitch_ + x_];

			// Get the color components of the pixel
			Uint8 r_ = 0, g_ = 0, b_ = 0, a_ = 0;
			SDL_GetRGBA(pixel_, format_, &r_, &g_, &b_, &a_);

			// Unlock texture
			SDL_UnlockTexture(texture_);
			pixels_ = nullptr;
			upixels_ = nullptr;

			// Free the allocated format
			SDL_FreeFormat(format_);

			// Save the color components to the output color object
			out_.setTo(r_, g_, b_, a_);
		}
	}

	return out_;
}

Display::Color TextureManager::getPixel (int x_, int y_, std::string key_, int frame_)
{
	// Get the frame's name
	Frame *f_ = getFrame(key_, frame_);
	if (f_)
		return getPixel(x_, y_, key_, f_->name);
	else
		return Display::Color ();

	/*
	   auto textureFrame = getFrame(key, frame);

	   Display::Color out;

	   if (textureFrame)
	   {
	// Adjust for trim (if not trimmed x and y are just zero)
	x -= textureFrame.x;
	y -= textureFrame.y;

	auto data = textureFrame.data.cut;

	x += data.x;
	y += data.y;

	if (x >= data.x && x < data.r && y >= data.y && y < data.b)
	{
	void *pixels = nullptr;
	int pitch = 0;

	// Get the frame's source SDL texture
	SDL_Texture *texture = textureFrame->source_.sdlTexture_;

	// Check if the texture exists
	if (!texture)
	{
	messageError("The frame has no source texture: ", SDL_GetError());
	return out;
	}

	// Allocate format from window
	Uint32 uformat = SDL_GetWindowPixelFormat(game->window.getSDLWindow());
	SDL_PixelFormat *format = SDL_AllocFormat(uformat);

	// Lock texture for manipulation
	if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0)
	{
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
	*/
}

int TextureManager::getPixelAlpha (int x_, int y_, std::string key_, std::string frame_)
{
	Frame* textureFrame_ = getFrame(key_, frame_);

	int out_ = -1;

	if (textureFrame_)
	{
		// Adjust for trim (if not trimmed x and y are just zero)
		x_ -= textureFrame_->x;
		y_ -= textureFrame_->y;

		auto data_ = textureFrame_->data.cut;

		x_ += data_.x;
		y_ += data_.y;

		if (x_ >= data_.x && x_ < data_.getRight() && y_ >= data_.y && y_ < data_.getBottom())
		{
			void *pixels_ = nullptr;
			int pitch_ = 0;

			// Get the frame's source SDL texture
			SDL_Texture *texture_ = textureFrame_->source->sdlTexture;

			// Check if the texture exists
			if (!texture_)
			{
				messageError("The frame has no source texture: ", SDL_GetError());
				return out_;
			}

			// Allocate format from window
			Uint32 uformat_ = SDL_GetWindowPixelFormat(game->window.window);
			SDL_PixelFormat *format_ = SDL_AllocFormat(uformat_);

			// Lock texture for manipulation
			if (SDL_LockTexture(texture_, nullptr, &pixels_, &pitch_) != 0)
			{
				messageError("Unable to lock texture: ", SDL_GetError());

				SDL_FreeFormat(format_);

				return out_;
			}

			// Get pixels in unsigned int of 32 bits
			Uint32 *upixels_ = static_cast<Uint32*>(pixels_);

			// Read the pixel in `x` and `y`
			Uint32 pixel_ = upixels_[y_ * pitch_ + x_];

			// Get the color components of the pixel
			Uint8 a_ = 0;
			SDL_GetRGBA(pixel_, format_, nullptr, nullptr, nullptr, &a_);

			// Unlock texture
			SDL_UnlockTexture(texture_);
			pixels_ = nullptr;
			upixels_ = nullptr;

			// Free the allocated format
			SDL_FreeFormat(format_);

			// Save the color components to the output variable
			out_ = a_;
		}
	}

	return out_;
}

int TextureManager::getPixelAlpha (int x_, int y_, std::string key_, int frame_)
{
	// Get the frame's name
	Frame* f_ = getFrame(key_, frame_);
	if (f_)
		return getPixelAlpha(x_, y_, key_, f_->name);
	else
		return -1;

	/*
	   auto textureFrame = getFrame(key, frame);

	   int out = -1;

	   if (textureFrame)
	   {
	// Adjust for trim (if not trimmed x and y are just zero)
	x -= textureFrame.x;
	y -= textureFrame.y;

	auto data = textureFrame.data.cut;

	x += data.x;
	y += data.y;

	if (x >= data.x && x < data.r && y >= data.y && y < data.b)
	{
	void *pixels = nullptr;
	int pitch = 0;

	// Get the frame's source SDL texture
	SDL_Texture *texture = textureFrame->source_.sdlTexture_;

	// Check if the texture exists
	if (!texture)
	{
	messageError("The frame has no source texture: ", SDL_GetError());
	return out;
	}

	// Allocate format from window
	Uint32 uformat = SDL_GetWindowPixelFormat(game->window.getSDLWindow());
	SDL_PixelFormat *format = SDL_AllocFormat(uformat);

	// Lock texture for manipulation
	if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0)
	{
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
	*/
}

bool TextureManager::renameTexture (std::string currentKey_, std::string newKey_)
{
	Texture *texture_ = get(currentKey_);

	if (texture_ && currentKey_ != newKey_)
	{
		texture_->key = newKey_;

		// Move the node containing the texture without copying
		auto nodeHandler_ = list.extract(currentKey_);
		nodeHandler_.key() = newKey_;
		list.insert(std::move(nodeHandler_));

		return true;
	}

	return false;
}

}	// namespace Textures
}	// namespace Zen
