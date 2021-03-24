/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "depth.hpp"

#include "entt/entt.hpp"
#include "../components/depth.hpp"
#include "../../utils/assert.hpp"

namespace Zen {

extern entt::registry registry;

int GetDepth (Entity entity)
{
	auto depth = registry.try_get<DepthComponent>(entity);

	ZEN_ASSERT(depth, "The entity has no 'Depth' component.");

	return depth->value;
}

void SetDepth (Entity entity, int value = 0)
{
	auto depth = registry.try_get<DepthComponent>(entity);

	ZEN_ASSERT(depth, "The entity has no 'Depth' component.");

	depth->value = value;
}

}	// namespace Zen
