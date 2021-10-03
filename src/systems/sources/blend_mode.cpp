/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../blend_mode.hpp"

#include "../../utils/assert.hpp"
#include "../../components/blend_mode.hpp"

namespace Zen {

extern entt::registry g_registry;

int GetBlendMode (Entity entity)
{
	auto blendMode = g_registry.try_get<Components::BlendMode>(entity);
	ZEN_ASSERT(blendMode, "The entity has no 'BlendMode' component.");

	return blendMode->value;
}

void SetBlendMode (Entity entity, int value)
{
	auto blendMode = g_registry.try_get<Components::BlendMode>(entity);
	ZEN_ASSERT(blendMode, "The entity has no 'BlendMode' component.");

	blendMode->value = value;
}

void SetBlendMode (Entity entity, BLEND_MODE value)
{
	auto blendMode = g_registry.try_get<Components::BlendMode>(entity);
	ZEN_ASSERT(blendMode, "The entity has no 'BlendMode' component.");

	blendMode->value = static_cast<int>(value);
}

}	// namespace Zen
