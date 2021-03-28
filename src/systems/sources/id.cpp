/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../id.hpp"

#include "../../utils/assert.hpp"
#include "../../components/id.hpp"

namespace Zen {

extern entt::registry g_registry;

void SetId (Entity entity, int i)
{
	auto id = g_registry.try_get<Components::Id>(entity);
	ZEN_ASSERT(id, "The entity has no 'Id' component.");

	id->value = i;
}

int GetId (Entity entity)
{
	auto id = g_registry.try_get<Components::Id>(entity);
	ZEN_ASSERT(id, "The entity has no 'Id' component.");

	return id->value;
}

}	// namespace Zen
