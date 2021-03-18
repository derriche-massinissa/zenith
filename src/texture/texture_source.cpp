/**
 * @file		texture_source.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "texture_source.h"

namespace Zen {
namespace Textures {

TextureSource::TextureSource (Texture& texture_, std::string source_)
	: source (source_)
	, renderer (texture_.manager.game.renderer)
	, texture (texture_)
{
	init();

	if (!sdlTexture)
	{
		messageError("Texture couldn't be created: ", IMG_GetError());
		// sdlTexture_ will stay `nullptr`
	}
	else
	{
		SDL_QueryTexture(sdlTexture, nullptr, nullptr, &width, &height);
	}
}

TextureSource::~TextureSource ()
{
	if (sdlTexture) {
		SDL_DestroyTexture(sdlTexture);
		sdlTexture = nullptr;
	}
}

void TextureSource::init ()
{
	if (source.size() > 5 && source.substr(0, 5) == "data:") {
		// Source is a Base64 Image data
		SDL_RWops *rw_ = SDL_RWFromConstMem(&source.c_str(), source.size());

		sdlTexture = SDL_LoadTextureTyped_RW(
				renderer.renderer,
				rw_,
				1,		// The SDL_RWops will be closed automatically
				"PNG"
				);
	}
	else {
		// Source is an image file path
		sdlTexture = IMG_LoadTexture(
				renderer.getSDLRenderer(),
				source.c_str()
				);
	}
}

void TextureSource::update ()
{}

}	// namespace Textures
}	// namespace Zen
