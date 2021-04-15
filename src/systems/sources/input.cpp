/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../input.hpp"

#include "../../utils/assert.hpp"

namespace Zen {

extern entt::registry g_registry;

bool IsInputEnabled (Entity entity)
{
	auto input = g_registry.try_get<Components::Input>(entity);
	ZEN_ASSERT(input, "The entity has no 'Input' component.");

	return input->enabled;
}

void MakeDropZone (Entity entity, bool value)
{
	auto input = g_registry.try_get<Components::Input>(entity);
	ZEN_ASSERT(input, "The entity has no 'Input' component.");

	input->dropZone = value;
}

Components::Input* GetInput (Entity entity)
{
	auto input = g_registry.try_get<Components::Input>(entity);
	ZEN_ASSERT(input, "The entity has no 'Input' component.");

	return input;
}

}	// namespace Zen
