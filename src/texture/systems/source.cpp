/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "source.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "entt/entt.hpp"
#include "../../utils/assert.hpp"
#include "../../utils/base64_decode.hpp"
#include "../../utils/messages.hpp"
#include "../components/source.hpp"
#include "../../window/window.hpp"

namespace Zen {

extern entt::registry g_registry;
extern Window g_window;

Entity CreateTextureSource (Entity texture, std::string src, int index)
{
	SDL_Texture *sdlTexture = nullptr;
	int width, height;

	// We first load the texture
	if (src.size() > 10 && src.substr(0, 10) == "iVBORw0KGg")
	{
		// Source is a Base64 Image data
		std::string base64 = Base64Decode(src);

		SDL_RWops *rw_ = SDL_RWFromConstMem(base64.c_str(), base64.size());

		sdlTexture = IMG_LoadTextureTyped_RW(
				g_window.renderer,
				rw_,
				1,		// The SDL_RWops will be closed automatically
				"PNG"
				);
	}
	else
	{
		// Source is an image file path
		sdlTexture = IMG_LoadTexture(
				g_window.renderer,
				src.c_str()
				);
	}

	// Check if the texture loaded correctly
	if (!sdlTexture)
	{
		MessageError("Texture couldn't be created: ", IMG_GetError());

		return entt::null;
	}
	else
	{
		SDL_QueryTexture(sdlTexture, nullptr, nullptr, &width, &height);

		// Create a Texture Source entity
		auto source = g_registry.create();
		g_registry.emplace<TextureSourceComponent>(
				texture,
				src.c_str(),
				1.0,
				index,
				sdlTexture,
				width,
				height
				);

		return source;
	}
}

void DestroyTextureSource (Entity source)
{
	auto src = g_registry.try_get<TextureSourceComponent>(source);
	ZEN_ASSERT(src, "The entity has no 'TextureSource' component.");

	if (src->sdlTexture)
		SDL_DestroyTexture(src->sdlTexture);

	g_registry.destroy(source);
}

}	// namespace Zen
