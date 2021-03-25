/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_SYSTEMS_VISIBLE_HPP
#define ZEN_GAMEOBJECTS_SYSTEMS_VISIBLE_HPP

#include "../../ecs/entity.hpp"

namespace Zen {

/**
 * Returns the visibility of this Entity.
 *
 * @since 0.0.0
 *
 * @return The visible state of this Entity.
 */
bool GetVisible (Entity entity);

/**
 * Sets the visibility of this Entity.
 *
 * @since 0.0.0
 *
 * @param value The visible state of this Entity.
 */
void SetVisible (Entity entity, bool value);

}	// namespace Zen

#endif
