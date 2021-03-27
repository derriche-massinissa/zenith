/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../alpha.hpp"

#include "../../math/clamp.hpp"
#include "../../utils/assert.hpp"

#include "../../components/alpha.hpp"
#include "../../components/renderable.hpp"

#define FLAG 0b0010

namespace Zen {

extern entt::registry registry;

void ClearAlpha (Entity entity)
{
	SetAlpha(entity, 1);
}

void SetAlpha (Entity entity, double topLeft, double topRight, double bottomLeft, double bottomRight)
{
	auto alpha = registry.try_get<Components::Alpha>(entity);
	ZEN_ASSERT(alpha, "The entity has no 'Alpha' component.");

	alpha->value = topLeft;
	alpha->tl = Math::Clamp(topLeft, 0.0, 1.0);
	alpha->tr = Math::Clamp(topRight, 0.0, 1.0);
	alpha->bl = Math::Clamp(bottomLeft, 0.0, 1.0);
	alpha->br = Math::Clamp(bottomRight, 0.0, 1.0);

	if(auto renderable = registry.try_get<Components::Renderable>(entity))
	{
		if (!topLeft && !topRight && !bottomLeft && !bottomRight)
			// Turn the alpha bit to 0
			renderable->flags &= ~FLAG;
		else
			// Turn the alpha bit to 1
			renderable->flags |= FLAG;
	}
}

double GetAlpha (Entity entity)
{
	const auto alpha = registry.try_get<Components::Alpha>(entity);
	ZEN_ASSERT(alpha, "The entity has no 'Alpha' component.");

	return alpha->value;
}

double GetAlphaTopLeft (Entity entity)
{
	const auto alpha = registry.try_get<Components::Alpha>(entity);
	ZEN_ASSERT(alpha, "The entity has no 'Alpha' component.");

	return alpha->tl;
}

double GetAlphaTopRight (Entity entity)
{
	const auto alpha = registry.try_get<Components::Alpha>(entity);
	ZEN_ASSERT(alpha, "The entity has no 'Alpha' component.");

	return alpha->tr;
}

double GetAlphaBottomLeft (Entity entity)
{
	const auto alpha = registry.try_get<Components::Alpha>(entity);
	ZEN_ASSERT(alpha, "The entity has no 'Alpha' component.");

	return alpha->bl;
}

double GetAlphaBottomRight (Entity entity)
{
	const auto alpha = registry.try_get<Components::Alpha>(entity);
	ZEN_ASSERT(alpha, "The entity has no 'Alpha' component.");

	return alpha->br;
}

void SetAlpha (Entity entity, double value)
{
	auto alpha = registry.try_get<Components::Alpha>(entity);
	ZEN_ASSERT(alpha, "The entity has no 'Alpha' component.");

	double v = Math::Clamp(value, 0.0, 1.0);

	alpha->value = v;
	alpha->tl = v;
	alpha->tr = v;
	alpha->bl = v;
	alpha->br = v;

	if(auto renderable = registry.try_get<Components::Renderable>(entity))
	{
		if (!v)
			// Turn the alpha bit to 0
			renderable->flags &= ~FLAG;
		else
			// Turn the alpha bit to 1
			renderable->flags |= FLAG;
	}
}

void SetAlphaTopLeft (Entity entity, double value)
{
	auto alpha = registry.try_get<Components::Alpha>(entity);
	ZEN_ASSERT(alpha, "The entity has no 'Alpha' component.");

	double v = Math::Clamp(value, 0.0, 1.0);

	alpha->tl = v;

	if(auto renderable = registry.try_get<Components::Renderable>(entity))
	{
		if (!v)
			// Turn the alpha bit to 0
			renderable->flags &= ~FLAG;
	}
}

void SetAlphaTopRight (Entity entity, double value)
{
	auto alpha = registry.try_get<Components::Alpha>(entity);
	ZEN_ASSERT(alpha, "The entity has no 'Alpha' component.");

	double v = Math::Clamp(value, 0.0, 1.0);

	alpha->tr = v;

	if(auto* renderable = registry.try_get<Components::Renderable>(entity))
	{
		if (!v)
			// Turn the alpha bit to 0
			renderable->flags &= ~FLAG;
	}
}

void SetAlphaBottomLeft (Entity entity, double value)
{
	auto alpha = registry.try_get<Components::Alpha>(entity);
	ZEN_ASSERT(alpha, "The entity has no 'Alpha' component.");

	double v = Math::Clamp(value, 0.0, 1.0);

	alpha->bl = v;

	if(auto* renderable = registry.try_get<Components::Renderable>(entity))
	{
		if (!v)
			// Turn the alpha bit to 0
			renderable->flags &= ~FLAG;
	}
}

void SetAlphaBottomRight (Entity entity, double value)
{
	auto alpha = registry.try_get<Components::Alpha>(entity);
	ZEN_ASSERT(alpha, "The entity has no 'Alpha' component.");

	double v = Math::Clamp(value, 0.0, 1.0);

	alpha->br = v;

	if(auto* renderable = registry.try_get<Components::Renderable>(entity))
	{
		if (!v)
			// Turn the alpha bit to 0
			renderable->flags &= ~FLAG;
	}
}

}	// namespace Zen

#undef FLAG
