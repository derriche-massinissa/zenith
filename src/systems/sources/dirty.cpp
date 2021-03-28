/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../dirty.hpp"

#include "../../utils/assert.hpp"
#include "../../components/dirty.hpp"

namespace Zen {

extern entt::registry g_registry;

bool IsDirty (Entity entity)
{
	auto dirty = g_registry.try_get<Components::Dirty>(entity);
	ZEN_ASSERT(dirty, "The entity has no 'Dirty' component.");

	return dirty->value;
}

void SetDirty (Entity entity, bool value)
{
	auto dirty = g_registry.try_get<Components::Dirty>(entity);
	ZEN_ASSERT(dirty, "The entity has no 'Dirty' component.");

	dirty->value = value;
}

}	// namespace Zen
