/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_COMPONENTS_SOURCE_HPP
#define ZEN_TEXTURES_COMPONENTS_SOURCE_HPP

#include <SDL2/SDL_render.h>
#include <string>
#include "../../ecs/entity.hpp"

namespace Zen {
namespace Components {

struct TextureSource
{
	/**
	 * The texture to which this Source belongs to.
	 *
	 * @property
	 * @since 0.0.0
	 */
	Entity texture;

	/**
	 * The source used to create this Source.
	 *
	 * Typically a path to an image file, but can also be Base64 image data.
	 *
	 * @property
	 * @since 0.0.0
	 */
	std::string source;

	/**
	 * The index of this source in the owning Texture.
	 *
	 * @property
	 * @since 0.0.0
	 */
	int index;

	/**
	 * The resolution of this Source.
	 *
	 * @property
	 * @since 0.0.0
	 */
	double resolution;

	/**
	 * The SDL_Texture loaded in memory that this Source represents.
	 *
	 * @property
	 * @since 0.0.0
	 */
	SDL_Texture *sdlTexture;

	/**
	 * The dimensions of this Source.
	 *
	 * @property
	 * @since 0.0.0
	 */
	int width, height;
};

} // namespace Components
} // namespace Zen

#endif
