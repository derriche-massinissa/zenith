/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../depth.hpp"

#include "../../utils/assert.hpp"
#include "../../components/depth.hpp"

namespace Zen {

extern entt::registry g_registry;

int GetDepth (Entity entity)
{
	auto depth = g_registry.try_get<Components::Depth>(entity);
	ZEN_ASSERT(depth, "The entity has no 'Depth' component.");

	return depth->value;
}

void SetDepth (Entity entity, int value)
{
	auto depth = g_registry.try_get<Components::Depth>(entity);
	ZEN_ASSERT(depth, "The entity has no 'Depth' component.");

	depth->value = value;
}

}	// namespace Zen
