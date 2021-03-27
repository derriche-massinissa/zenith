/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_TRANSFORM_HPP
#define ZEN_SYSTEMS_TRANSFORM_HPP

#include "../ecs/entity.hpp"
#include "../math/types/vector2.hpp"
#include "../components/transform_matrix.hpp"

namespace Zen {

Components::TransformMatrix GetLocalTransformMatrix (Entity entity);

Components::TransformMatrix GetWorldTransformMatrix (Entity entity);

Math::Vector2 GetLocalPoint (Entity entity, double x, double y, Entity camera = entt::null);

}	// namespace Zen

#endif
