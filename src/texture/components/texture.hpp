/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_COMPONENTS_TEXTURE_HPP
#define ZEN_TEXTURES_COMPONENTS_TEXTURE_HPP

#include "../../ecs/entity.hpp"
#include <string>

namespace Zen {
namespace Components {

struct Texture
{
	/**
	 * The unique key of this Texture.
	 *
	 * @property
	 * @since 0.0.0
	 */
	std::string key;

	/**
	 * The total number of Frames in this Texture, including the `__BASE` frame.
	 *
	 * A Texture will always contain at least 1 frame because every Texture
	 * contains a `__BASE` frame by default,
	 * in addition to any extra frames that have been added to it, such as when
	 * parsing a Sprite Sheet or Texture Atlas.
	 *
	 * @since 0.0.0
	 */
	int frameTotal;

	Entity firstFrame;
};

}	// namespace Components
}	// namespace Zen

#endif
