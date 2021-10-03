/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_COMPONENTS_RENDERABLE_HPP
#define ZEN_COMPONENTS_RENDERABLE_HPP

#include <functional>
#include "../ecs/entity.hpp"
#include "transform_matrix.hpp"

namespace Zen {
namespace Components {

struct Renderable
{
	int flags = 0b1111;

	int filter = 0;

	/**
	 * Renders the Game Object with the Renderer to the given Camera.
	 * The object will not render if any of its renderFlags are set or it is being actively filtered out by the Camera.
	 * This method should not be called directly. It is a utility function of the Render module.
	 *
	 * @since 0.0.0
	 *
	 * @param src The Game Object being rendered in this call.
	 * @param camera The Camera that is rendering the Game Object.
	 * @param parentMatrix This transform matrix is defined if the game object is nested
	 */
	std::function<void(Entity, Entity, TransformMatrix*)> render;
};

}	// namespace Components
}	// namespace Zen

#endif
