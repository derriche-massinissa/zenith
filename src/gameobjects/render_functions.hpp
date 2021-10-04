/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_RENDERFUNCTIONS_HPP
#define ZEN_GAMEOBJECTS_RENDERFUNCTIONS_HPP

#include "../ecs/entity.hpp"
#include "../components/transform_matrix.hpp"

namespace Zen {

void Render_image (Entity entity, Entity camera,
		Components::TransformMatrix* matrix);

void Render_text (Entity entity, Entity camera,
		Components::TransformMatrix* matrix);

}	// namespace Zen

#endif
