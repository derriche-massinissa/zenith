/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_SYSTEMS_GETBOUNDS_HPP
#define ZEN_GAMEOBJECTS_SYSTEMS_GETBOUNDS_HPP

#include "../../ecs/entity.hpp"

#include "../../geom/rectangle.h"
#include "../../math/vector2.h"
#include "../../math/rotate_around.h"
#include "size.hpp"

namespace Zen {

static Math::Vector2 PrepareBoundsOutput (Math::Vector2 vector, bool includeParent);

/**
 * Gets the center coordinate of this Game Object, regardless of origin.
 * The returned point is calculated in local space and does not factor in any parent containers
 *
 * @since 0.0.0
 *
 * @return The values stored in the output object.
 */
Math::Vector2 GetCenter (Entity entity);

/**
 * Gets the top-left corner coordinate of this Game Object, regardless of
 * origin.
 * The returned point is calculated in local space and does not factor in any
 * parent containers
 *
 * @since 0.0.0
 *
 * @param output An object to store the values in. If not provided a new 
 * Vector2 will be created.
 * @param includeParent If this Game Object has a parent Container, include it 
 * (and all other ancestors) in the resulting vector?
 *
 * @return The values stored in the output object.
 */
Math::Vector2 GetTopLeft (Entity entity, bool includeParent);

Math::Vector2 GetTopCenter (Entity entity, bool includeParent);

Math::Vector2 GetTopRight (Entity entity, bool includeParent);

Math::Vector2 GetLeftCenter (Entity entity, bool includeParent);

Math::Vector2 GetRightCenter (Entity entity, bool includeParent);

Math::Vector2 GetBottomLeft (Entity entity, bool includeParent);

Math::Vector2 GetBottomCenter (Entity entity, bool includeParent);

Math::Vector2 GetBottomRight (Entity entity, bool includeParent);

Geom::Rectangle GetBounds (Entity entity);

}	// namespace Zen

#endif
