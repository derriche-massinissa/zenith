/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_DEADZONE_HPP
#define ZEN_SYSTEMS_DEADZONE_HPP

#include "../ecs/entity.hpp"

namespace Zen {

extern entt::registry g_registry;

void SetDeadzone (Entity entity, double width, double height = 0.);

void RemoveDeadzone (Entity entity);

}	// namespace Zen

#endif
