/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../name.hpp"

#include "../../components/name.hpp"

namespace Zen {

extern entt::registry g_registry;

void SetName (Entity entity, const char* name)
{
	g_registry.emplace_or_replace<Components::Name>(entity, name);
}

const char* GetName (Entity entity)
{
	auto name = g_registry.try_get<Components::Name>(entity);

	if (name)
		return name->value;
	else
		return nullptr;
}

}	// namespace Zen
