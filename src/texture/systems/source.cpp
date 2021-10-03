/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "source.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../../utils/assert.hpp"
#include "../../utils/base64/base64_decode.hpp"
#include "../../utils/messages.hpp"
#include "../components/source.hpp"
#include "../../window/window.hpp"
#include "../../renderer/renderer.hpp"

namespace Zen {

extern entt::registry g_registry;
extern Window g_window;
extern Renderer g_renderer;

Entity CreateTextureSource (Entity texture, std::string src, int index)
{
	SDL_Surface *surface = nullptr;

	// We first load the texture
	if (src.size() > 10 && src.substr(0, 10) == "iVBORw0KGg")
	{
		// Source is a Base64 Image data
		std::string base64 = Base64Decode(src);

		SDL_RWops *rw_ = SDL_RWFromConstMem(base64.c_str(), base64.size());

		surface = IMG_LoadTyped_RW(
				rw_,
				1,		// The SDL_RWops will be closed automatically
				"PNG"
				);
	}
	else
	{
		// Source is an image file path
		surface = IMG_Load(
				src.c_str()
				);
	}

	// Check if the surface loaded correctly
	if (!surface)
	{
		MessageError("Surface couldn't be created: ", IMG_GetError());

		return entt::null;
	}
	else
	{
		// Create a Texture Source entity
		auto source = g_registry.create();
		auto &c = g_registry.emplace<Components::TextureSource>(
				source,
				texture,
				src.c_str(),
				index,
				1.0,
				surface->w,
				surface->h,
				0,
				surface,
				0,
				-1
				);

		// Create a texture from the surface
		c.glTexture = g_renderer.createTextureFromSource(source);

		// Free the temporary surface
		SDL_FreeSurface(c.tmp);
		c.tmp = nullptr;

		return source;
	}
}

void DestroyTextureSource (Entity source)
{
	auto src = g_registry.try_get<Components::TextureSource>(source);
	ZEN_ASSERT(src, "The entity has no 'TextureSource' component.");

	if (src->glTexture)
		glDeleteTextures(1, &src->glTexture);

	g_registry.destroy(source);
}

}	// namespace Zen
