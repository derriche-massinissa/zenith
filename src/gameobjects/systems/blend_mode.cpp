/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "blend_mode.hpp"

#include "entt/entt.hpp"
#include "../components/blend_mode.hpp"
#include "../../utils/assert.hpp"

namespace Zen {

extern entt::registry registry;

BLEND_MODE GetBlendMode (Entity entity)
{
	auto blendMode = registry.try_get<BlendModeComponent>(entity);

	ZEN_ASSERT(blendMode, "The entity has no 'BlendMode' component.");

	return blendMode->value;
}

void SetBlendMode (Entity entity, BLEND_MODE value)
{
	auto blendMode = registry.try_get<BlendModeComponent>(entity);

	ZEN_ASSERT(blendMode, "The entity has no 'BlendMode' component.");

	blendMode->value = value;
}

BLEND_MODE blendMode = BLEND_MODE::NORMAL;

}	// namespace Zen
