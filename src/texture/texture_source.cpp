/**
 * @file		texture_source.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "texture_source.h"

namespace Zen {
namespace Textures {

TextureSource::TextureSource (Texture& texture, std::string source)
	: texture_(texture)
	  , renderer_(texture.manager.game.renderer)
	  , source_(source)
{
	init();

	if (!sdlTexture_) {
		messageError("Texture couldn't be created: ", IMG_GetError());

		// sdlTexture_ will stay `nullptr`
	} else {
		SDL_QueryTexture(sdlTexture_, nullptr, nullptr, &width_, &height_);
	}
}

TextureSource::~TextureSource ()
{
	if (sdlTexture_) {
		SDL_DestroyTexture(sdlTexture_);
		sdlTexture_ = nullptr;
	}
}

void init ()
{
	if (source_.size() > 5 && source_.substr(0, 5) == "data:") {
		// Source is a Base64 Image data
		SDL_RWops *rw = SDL_RWFromConstMem(&source.c_str(), source.size());

		sdlTexture_ = SDL_LoadTextureTyped_RW(
				renderer_.renderer_,
				rw,
				1,		// The SDL_RWops will be closed automatically
				"PNG"
				);
	}
	else {
		// Source is an image file path
		sdlTexture_ = IMG_LoadTexture(
				renderer_.getSDLRenderer(),
				source_.c_str()
				);
	}
}

void TextureSource::update ()
{}

}	// namespace Textures
}	// namespace Zen
