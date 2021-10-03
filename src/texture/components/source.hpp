/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_COMPONENTS_SOURCE_HPP
#define ZEN_TEXTURES_COMPONENTS_SOURCE_HPP

#include <string>
#include <SDL2/SDL_render.h>
#include "../../ecs/entity.hpp"
#include "../../renderer/types/gl_types.hpp"

namespace Zen {
namespace Components {

/**
 * A Texture Source is the encapsulation of the actual source data for a Texture.
 *
 * A Texture can contain multiple Texture Sources, which only happens when a
 * multi-atlas is loaded.
 *
 * @struct TextureSource
 * @since 0.0.0
 */
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
	 * Either a path to an image file, or a Base64 image data.
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
	 * The dimensions of this Source.
	 *
	 * @property
	 * @since 0.0.0
	 */
	int width, height;

	/**
	 * The GL_Texture loaded in memory that this Source represents.
	 *
	 * @property
	 * @since 0.0.0
	 */
	GL_texture glTexture = 0;

	/**
	 * A temporary surface populated when the source is created. It is used to
	 * create an OpenGL texture and is freed as soon as that is done.
	 *
	 * @property
	 * @since 0.0.0
	 */
	SDL_Surface *tmp = nullptr;

	/**
	 * The current texture unit index as assigned by the Renderer.
	 *
	 * @since 0.0.0
	 */
	int glIndex = 0;

	/**
	 * The counter value when this texture was last assigned an index by the Renderer.
	 *
	 * @since 0.0.0
	 */
	int glIndexCounter = -1;
};

} // namespace Components
} // namespace Zen

#endif
