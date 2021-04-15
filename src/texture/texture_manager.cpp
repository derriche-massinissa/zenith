/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "texture_manager.hpp"

#include "parsers/json_array.hpp"
#include "parsers/json_hash.hpp"
#include "parsers/sprite_sheet.hpp"
#include "parsers/sprite_sheet_atlas.hpp"

#include <tuple>
#include <utility>
#include <fstream>
#include "../utils/messages.hpp"
#include "../core/config.hpp"
#include "../window/window.hpp"
#include "components/source.hpp"
#include "components/frame.hpp"
#include "systems/texture.hpp"
#include "systems/frame.hpp"
#include "../geom/rectangle.hpp"
#include "../display/color.hpp"

namespace Zen {

extern entt::registry g_registry;
extern Window g_window;

TextureManager::~TextureManager ()
{
	// Free surfaces
	for (auto surface_ = alphaCache.begin(); surface_ != alphaCache.end(); surface_++)
	{
		SDL_FreeSurface(surface_->second);
	}
}

void TextureManager::boot (GameConfig *config_)
{
	config = config_;

	addBase64("__DEFAULT", config->defaultImage);
	addBase64("__MISSING", config->missingImage);
	addBase64("__WHITE", config->whiteImage);
}

bool TextureManager::checkKey (std::string key_)
{
	if (exists(key_))
	{
		MessageError("The texture key is already in use: ", key_);

		return false;
	}

	return true;
}

TextureManager& TextureManager::remove (std::string key_)
{
	if (!exists(key_))
	{
		MessageWarning("No texture found matching the key: ", key_);

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

Entity TextureManager::addBase64 (
		std::string key_, std::string data_)
{
	return addImage(key_, data_);
}

Entity TextureManager::addImage (std::string key_, std::string path_)
{
	Entity texture_ = entt::null;

	if (checkKey(key_))
	{
		texture_ = create(key_, path_);

		if (texture_ != entt::null)
		{
			int sourceIndex_ = 0;

			// Get the source
			Components::TextureSource *source_ = nullptr;
			for (auto entity_ : g_registry.view<Components::TextureSource>())
			{
				auto& src_ = g_registry.get<Components::TextureSource>(entity_);

				if (src_.texture == texture_ && src_.index == sourceIndex_)
				{
					source_ = &src_;
					break;
				}
			}

			AddFrame(
					texture_,
					"__BASE",
					sourceIndex_,
					0,
					0,
					source_->width,
					source_->height);

			emit("add", key_);
		}
	}

	return texture_;
}

Entity TextureManager::addRenderTexture (std::string key_, Entity renderTexture_)
{
		/*
		 * TODO
	Texture *texture_ = nullptr;

	if (checkKey(key_))
	{
		texture_ = create(key_, renderTexture_);

		 int sourceIndex_ = 0;

		 texture_->add("__BASE",
		 sourceIndex_,
		 0,
		 0,
		 renderTexture_.width_,
		 renderTexture_.height_);

		 emit("add", key_);
	}

	return texture_;
		 */
	return entt::null;
}

Entity TextureManager::addAtlas (
		std::string key_, std::vector<std::string> sources_, std::string dataPath_)
{
	// Open file
	std::fstream file_ (dataPath_, std::ios::in);

	if (!file_)
	{
		MessageError("JSON file couldn't be opened: ", dataPath_);

		return entt::null;
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

Entity TextureManager::addAtlas (
		std::string key_, std::string source_, std::string dataPath_)
{
	std::vector<std::string> sources_;
	sources_.emplace_back(source_);

	return addAtlas(key_, sources_, dataPath_);
}

Entity TextureManager::addAtlasJSONArray (
		std::string key_, std::vector<std::string> sources_, nlohmann::json data_)
{
	Entity texture_ = entt::null;

	if (checkKey(key_))
	{
		texture_ = create(key_, sources_);

		ParseJsonArray(texture_, 0, data_);

		emit("add", key_);
	}

	return texture_;
}

Entity TextureManager::addAtlasJSONArray (
		std::string key_, std::vector<std::string> sources_, std::vector<nlohmann::json> data_)
{
	// Multi-Atlas

	Entity texture_ = entt::null;

	if (checkKey(key_))
	{
		texture_ = create(key_, sources_);

		// Multi-pack with on atlas file for all images
		bool singleAtlasFile_ = (data_.size() == 1);

		// Count the sources
		int size_ = 0;
		for (auto entity_ : g_registry.view<Components::TextureSource>())
		{
			auto& src_ = g_registry.get<Components::TextureSource>(entity_);

			if (src_.texture == texture_)
				size_++;
		}

		// Assumes the textures are in the same order in the source array as in the json data
		for (int i_ = 0; i_ < size_; i_++)
		{
			auto atlasData_ = singleAtlasFile_ ? data_[0] : data_[i_];

			ParseJsonArray(texture_, i_, atlasData_);
		}

		emit("add", key_);
	}

	return texture_;
}

Entity TextureManager::addAtlasJSONHash (
		std::string key_, std::vector<std::string> sources_, nlohmann::json data_)
{
	Entity texture_ = entt::null;

	if (checkKey(key_))
	{
		texture_ = create(key_, sources_);

		ParseJsonHash(texture_, 0, data_);

		emit("add", key_);
	}

	return texture_;
}

Entity TextureManager::addAtlasJSONHash (
		std::string key_, std::vector<std::string> sources_, std::vector<nlohmann::json> data_)
{
	Entity texture_ = entt::null;

	if (checkKey(key_))
	{
		texture_ = create(key_, sources_);

		for (int i_ = 0; i_ < data_.size(); i_++)
		{
			ParseJsonHash(texture_, i_, data_[i_]);
		}

		emit("add", key_);
	}

	return texture_;
}

Entity TextureManager::addSpriteSheet (std::string key_, std::string path_, SpriteSheetConfig config_)
{
	Entity texture_ = entt::null;

	if (checkKey(key_))
	{
		texture_ = create(key_, path_);

		// Get the source
		Components::TextureSource *source_ = nullptr;
		for (auto entity_ : g_registry.view<Components::TextureSource>())
		{
			auto& src_ = g_registry.get<Components::TextureSource>(entity_);

			if (src_.texture == texture_)
			{
				source_ = &src_;
				break;
			}
		}

		int width_ = source_->width;
		int height_ = source_->height;

		ParseSpriteSheet(texture_, 0, 0, 0, width_, height_, config_);

		emit("add", key_);
	}

	return texture_;
}

Entity TextureManager::addSpriteSheetFromAtlas (std::string key_, SpriteSheetConfig config_)
{
	if (!checkKey(key_))
		return entt::null;

	std::string atlasKey_ = config_.atlas;
	std::string atlasFrame_ = config_.frame;

	if (atlasKey_ == "" || atlasFrame_ == "")
		return entt::null;

	Entity atlas_ = get(atlasKey_);
	Entity sheet_ = GetFrame(atlas_, atlasFrame_);


	if (sheet_ != entt::null)
	{
		auto& spritesheet_ = g_registry.get<Components::Frame>(sheet_);
		auto& source_ = g_registry.get<Components::TextureSource>(spritesheet_.source);

		Entity texture_ = create(key_, source_.source);

		if (IsFrameTrimmed(sheet_))
		{
			// If trimmed, we need to help the parser adjust
			ParseSpriteSheetFromAtlas(texture_, sheet_, config_);
		}
		else
		{
			ParseSpriteSheet(texture_, 0, spritesheet_.cutX, spritesheet_.cutY, spritesheet_.cutWidth, spritesheet_.cutHeight, config_);
		}

		emit("add", key_);

		return texture_;
	}

	return entt::null;
}

Entity TextureManager::create (std::string key_, std::vector<std::string> sources_)
{
	Entity texture_ = entt::null;

	if (checkKey(key_))
	{
		/*
		Texture tex_ (*this, key_, sources_);

		list.emplace(key_, tex_);
		*/

		auto [it, _] = list.emplace(
				key_,
				CreateTexture(key_, sources_)
				);

		// Check if the Texture was created successfuly
		if (it->second == entt::null)
			list.erase(it);
		else
			texture_ = list.find(key_)->second;
	}

	return texture_;
}

Entity TextureManager::create (std::string key_, std::string source_)
{
	return create(key_, std::vector<std::string> {source_});
}

Entity TextureManager::create (std::string key_, Entity renderTexture_)
{
	// TODO
	return entt::null;
}

bool TextureManager::exists (std::string key_)
{
	return (list.find(key_) != list.end());
}

Entity TextureManager::get (std::string key_)
{
	auto textureIterator_ = list.find(key_);

	if (textureIterator_ != list.end())
	{
		return list.find(key_)->second;
	}
	else
	{
		return list.find("__MISSING")->second;
	}
}

Entity TextureManager::getFrame (std::string key_, std::string frame_)
{
	auto textureIterator_ = list.find(key_);

	if (textureIterator_ != list.end())
	{
		return GetFrame(list.find(key_)->second, frame_);
	}
	else
	{
		return entt::null;
	}
}

Entity TextureManager::getFrame (std::string key_, int frame_)
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

Color TextureManager::getPixel (int x_, int y_, std::string key_, std::string frameName_)
{
	auto textureFrame_ = getFrame(key_, frameName_);
	auto frame_ = g_registry.try_get<Components::Frame>(textureFrame_);

	Color out_;

	if (frame_)
	{
		// Adjust for trim (if not trimmed x and y are just zero)
		x_ -= frame_->x;
		y_ -= frame_->y;

		auto data_ = frame_->data.cut;

		x_ += data_.x;
		y_ += data_.y;

		if (x_ >= data_.x && x_ < GetRight(data_) && y_ >= data_.y && y_ < GetBottom(data_))
		{
			void *pixels_ = nullptr;
			int pitch_ = 0;

			// Get the frame's source SDL texture
			auto& source_ = g_registry.get<Components::TextureSource>(frame_->source);
			SDL_Texture *texture_ = source_.sdlTexture;

			// Check if the texture exists
			if (!texture_)
			{
				MessageError("The frame has no source texture: ", SDL_GetError());
				return out_;
			}

			// Allocate format from window
			Uint32 uformat_ = SDL_GetWindowPixelFormat(g_window.window);
			SDL_PixelFormat *format_ = SDL_AllocFormat(uformat_);

			// Lock texture for manipulation
			if (SDL_LockTexture(texture_, nullptr, &pixels_, &pitch_) != 0)
			{
				MessageError("Unable to lock texture: ", SDL_GetError());

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
			SetTo(&out_, r_, g_, b_, a_);
		}
	}

	return out_;
}

Color TextureManager::getPixel (int x_, int y_, std::string key_, int frameIndex_)
{
	// Get the frame's name
	Entity f_ = getFrame(key_, frameIndex_);
	auto frame_ = g_registry.try_get<Components::Frame>(f_);
	if (frame_)
		return getPixel(x_, y_, key_, frame_->name);
	else
		return Color ();
}

int TextureManager::getPixelAlpha (int x_, int y_, Entity textureFrame_)
{
	int out_ = -1;

	if (textureFrame_ != entt::null)
	{
		auto& frame_ = g_registry.get<Components::Frame>(textureFrame_);

		auto cache_ = alphaCache.find(frame_.source);

		if (cache_ == alphaCache.end())
		{
			auto& src_ = g_registry.get<Components::TextureSource>(frame_.source);
			auto& txt_ = g_registry.get<Components::Texture>(src_.texture);

			MessageError("Reading pixel alpha values can only be done for cached textures! Load the texture \"", txt_.key, "\" with `true` as the last parameter to cache it.");
			return -1;
		}

		if (alphaCache[frame_.source] == nullptr)
		{
			MessageError("No surface is present in the requested alpha cache.");
			return -1;
		}

		// Adjust for trim (if not trimmed x and y are just zero)
		x_ -= frame_.x;
		y_ -= frame_.y;

		auto data_ = frame_.data.cut;

		x_ += data_.x;
		y_ += data_.y;

		if (x_ >= data_.x && x_ < GetRight(data_) && y_ >= data_.y && y_ < GetBottom(data_))
		{
			// Get pixels in unsigned int of 32 bits
			Uint32 *upixels_ = static_cast<Uint32*>(cache_->second->pixels);

			// Read the pixel in `x` and `y`
			Uint32 pixel_ = upixels_[y_ * (cache_->second->pitch/4) + x_];

			// Get the color components of the pixel
			Uint8 r_ = 0;
			Uint8 g_ = 0;
			Uint8 b_ = 0;
			Uint8 a_ = 0;
			SDL_GetRGBA(pixel_, cache_->second->format, &r_, &g_, &b_, &a_);

			// Save the color components to the output variable
			out_ = a_;
		}
	}

	return out_;
}

/*
int TextureManager::getPixelAlpha (int x_, int y_, Entity textureFrame_)
{
	int out_ = -1;

	if (textureFrame_ != entt::null)
	{
		auto& frame_ = g_registry.get<Components::Frame>(textureFrame_);

		// Adjust for trim (if not trimmed x and y are just zero)
		x_ -= frame_.x;
		y_ -= frame_.y;

		auto data_ = frame_.data.cut;

		x_ += data_.x;
		y_ += data_.y;

		if (x_ >= data_.x && x_ < GetRight(data_) && y_ >= data_.y && y_ < GetBottom(data_))
		{
			void *pixels_ = nullptr;
			int pitch_ = 0;

			// Get the frame's source SDL texture
			auto& source_ = g_registry.get<Components::TextureSource>(frame_.source);
			SDL_Texture *texture_ = source_.sdlTexture;

			// Check if the texture exists
			if (!texture_)
			{
				MessageError("The frame has no source texture: ", SDL_GetError());
				return out_;
			}

			// Allocate format from window
			Uint32 uformat_ = SDL_GetWindowPixelFormat(g_window.window);
			SDL_PixelFormat *format_ = SDL_AllocFormat(uformat_);

			// Lock texture for manipulation
			if (SDL_LockTexture(texture_, nullptr, &pixels_, &pitch_) != 0)
			{
				MessageError("Unable to lock texture: ", SDL_GetError());

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
 */

int TextureManager::getPixelAlpha (int x_, int y_, std::string key_, std::string frameName_)
{
	Entity frame_ = getFrame(key_, frameName_);

	return getPixelAlpha(x_, y_, frame_);
}

int TextureManager::getPixelAlpha (int x_, int y_, std::string key_, int frameIndex_)
{
	Entity frame_ = getFrame(key_, frameIndex_);

	return getPixelAlpha(x_, y_, frame_);
}

void TextureManager::createAlphaCache (std::string key_)
{
	// Get texture entity
	Entity texture_ = get(key_);

	// Get _ALL_ image files (Multi Atlases have many sources)
	std::vector<Entity> sources_ = GetTextureSources(texture_);

	// For each image:
	for (auto& source_ : sources_)
	{
		auto& src_ = g_registry.get<Components::TextureSource>(source_);

		// Create a surface
		alphaCache[source_] = IMG_Load(src_.source.c_str());

		if (alphaCache[source_] == nullptr)
		{
			MessageError("Unable to load image ", src_.source.c_str(), ": ", IMG_GetError());
			return;
		}
	}
}

/*
bool TextureManager::renameTexture (std::string currentKey_, std::string newKey_)
{
	Entity texture_ = get(currentKey_);

	if (texture_ != entt::null && currentKey_ != newKey_)
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
*/

}	// namespace Zen
