/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../transparent.hpp"

#include "../../utils/assert.hpp"
#include "../../components/transparent.hpp"

namespace Zen {

extern entt::registry g_registry;

void SetTransparency (Entity entity, bool value)
{
	auto transparent = g_registry.try_get<Components::Transparent>(entity);
	ZEN_ASSERT(transparent, "The entity has no 'Transparent' component.");

	transparent->value = value;
}

bool IsTransparent (Entity entity)
{
	auto transparent = g_registry.try_get<Components::Transparent>(entity);
	ZEN_ASSERT(transparent, "The entity has no 'Transparent' component.");

	return transparent->value;
}

}	// namespace Zen
