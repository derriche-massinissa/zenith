/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../mask.hpp"

#include "../../components/mask.hpp"
#include "../../components/renderable.hpp"
#include "../../utils/assert.hpp"

namespace Zen {

extern entt::registry g_registry;

Entity GetMask (Entity entity)
{
	auto masked = g_registry.try_get<Components::Masked>(entity);
	ZEN_ASSERT(masked, "The entity has no 'Mask' component.");

	return masked->mask;
}

void SetMask (Entity entity, Entity maskEntity)
{
	// Is the entity renderable?
	auto renderable = g_registry.try_get<Components::Renderable>(entity);
	ZEN_ASSERT(renderable, "The entity has no 'Renderable' component.");

	// Is the mask entity a geometry object?
	// TODO
	bool isBitmap = true;

	// Setup the masked entity
	Components::Masked &mask =
		g_registry.emplace_or_replace<Components::Masked>(entity);

	mask.mask = maskEntity;


	// Setup the mask entity
}

void ClearMask (Entity entity)
{
	auto mask = g_registry.try_get<Components::Mask>(entity);
	ZEN_ASSERT(mask, "The entity has no 'Mask' component.");

	mask->mask = entt::null;
	mask->fixed = false;
}

void PreRenderMask (Entity masked, Entity camera)
{
}

}	// namespace Zen
