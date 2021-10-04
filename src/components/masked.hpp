/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_COMPONENTS_MASKED_HPP
#define ZEN_COMPONENTS_MASKED_HPP

#include "../ecs/entity.hpp"
#include "../renderer/types/gl_types.hpp"

namespace Zen {
namespace Components {

struct Masked
{
	/**
	 * The mask entity.
	 *
	 * @since 0.0.0
	 */
	Entity mask = entt::null;

	/**
	 * If the masked entity is a camera, this hold the camera that should be
	 * used to render the mask, either the camera itself (the mask moves with it)
	 * or the scene's main camera (the mask is independent of the camera).
	 */
	Entity camera = entt::null;
};

}	// namespace Components
}	// namespace Zen

#endif
