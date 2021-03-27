/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../scale.hpp"

#include "../../utils/assert.hpp"
#include "../../components/scale.hpp"
#include "../../components/renderable.hpp"

#define FLAG 0b0100

namespace Zen {

extern entt::registry g_registry;

void SetScale (Entity entity, double value)
{
	auto [scale, renderable] = g_registry.try_get<Components::Scale, Components::Renderable>(entity);
	ZEN_ASSERT(scale, "The entity has no 'Scale' component.");

	scale->x = value;
	scale->y = value;

	if (!renderable) return;

	if (value == 0)
	{
		// Turn the scale flag to 0
		renderable->flags &= ~FLAG;
	}
	else
	{
		// Turn the scale flag to 1
		renderable->flags |= FLAG;
	}
}

double GetScale (Entity entity)
{
	auto scale = g_registry.try_get<Components::Scale>(entity);
	ZEN_ASSERT(scale, "The entity has no 'Scale' component.");

	return (scale->x + scale->y) / 2.;
}

void SetScaleX (Entity entity, double value)
{
	auto [scale, renderable] = g_registry.try_get<Components::Scale, Components::Renderable>(entity);
	ZEN_ASSERT(scale, "The entity has no 'Scale' component.");

	scale->x = value;

	if (!renderable) return;

	if (value == 0)
	{
		// Turn the scale flag to 0
		renderable->flags &= ~FLAG;
	}
	else
	{
		// Turn the scale flag to 1
		renderable->flags |= FLAG;
	}
}

double GetScaleX (Entity entity)
{
	auto scale = g_registry.try_get<Components::Scale>(entity);
	ZEN_ASSERT(scale, "The entity has no 'Scale' component.");

	return scale->x;
}

void SetScaleY (Entity entity, double value)
{
	auto [scale, renderable] = g_registry.try_get<Components::Scale, Components::Renderable>(entity);
	ZEN_ASSERT(scale, "The entity has no 'Scale' component.");

	scale->y = value;

	if (!renderable) return;

	if (value == 0)
	{
		// Turn the scale flag to 0
		renderable->flags &= ~FLAG;
	}
	else
	{
		// Turn the scale flag to 1
		renderable->flags |= FLAG;
	}
}

double GetScaleY (Entity entity)
{
	auto scale = g_registry.try_get<Components::Scale>(entity);
	ZEN_ASSERT(scale, "The entity has no 'Scale' component.");

	return scale->y;
}

}	// namespace Zen

#undef FLAG
