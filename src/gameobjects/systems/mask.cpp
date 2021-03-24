/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "mask.hpp"

#include "entt/entt.hpp"
#include "../components/mask.hpp"
#include "../../utils/assert.hpp"

namespace Zen {

extern entt::registry registry;

Entity GetMask (Entity entity)
{
	auto mask = registry.try_get<MaskComponent>(entity);

	ZEN_ASSERT(mask, "The entity has no 'Mask' component.");

	return mask->mask;
}

void SetMask (Entity entity, Entity maskEntity)
{
	auto mask = registry.try_get<MaskComponent>(entity);

	ZEN_ASSERT(mask, "The entity has no 'Mask' component.");

	mask->mask = maskEntity;
}

void ClearMask (Entity entity)
{
	auto mask = registry.try_get<MaskComponent>(entity);

	ZEN_ASSERT(mask, "The entity has no 'Mask' component.");

	mask->mask = entt::null;
}

}	// namespace Zen
