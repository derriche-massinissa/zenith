/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../mask.hpp"

#include "../../components/mask.hpp"
#include "../../utils/assert.hpp"

namespace Zen {

extern entt::registry g_registry;

Entity GetMask (Entity entity)
{
	auto mask = g_registry.try_get<Components::Mask>(entity);
	ZEN_ASSERT(mask, "The entity has no 'Mask' component.");

	return mask->mask;
}

void SetMask (Entity entity, Entity maskEntity, bool fixedPosition)
{
	auto mask = g_registry.try_get<Components::Mask>(entity);
	ZEN_ASSERT(mask, "The entity has no 'Mask' component.");

	mask->mask = maskEntity;
	mask->fixed = fixedPosition;
}

void ClearMask (Entity entity)
{
	auto mask = g_registry.try_get<Components::Mask>(entity);
	ZEN_ASSERT(mask, "The entity has no 'Mask' component.");

	mask->mask = entt::null;
	mask->fixed = false;
}

}	// namespace Zen
