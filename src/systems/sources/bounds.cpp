/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../bounds.hpp"

#include <algorithm>
#include "../../utils/assert.hpp"
#include "../../math/clamp.hpp"
#include "../../geom/rectangle.hpp"
#include "../../components/bounds.hpp"
#include "../../components/dirty.hpp"
#include "../../components/scroll.hpp"
#include "../size.hpp"

namespace Zen {

extern entt::registry g_registry;

void SetBounds (Entity entity, double x, double y, double width, double height)
{
	g_registry.emplace_or_replace<Components::Bounds>(entity, Rectangle {x, y, width, height});

	auto dirty = g_registry.try_get<Components::Dirty>(entity);
	if (dirty)
		dirty->value = true;
}

void RemoveBounds (Entity entity)
{
	g_registry.remove_if_exists<Components::Bounds>(entity);

	auto dirty = g_registry.try_get<Components::Dirty>(entity);
	if (dirty)
		dirty->value = true;
}

Rectangle GetBounds (Entity entity)
{
	auto bounds = g_registry.try_get<Components::Bounds>(entity);
	ZEN_ASSERT(bounds, "The entity has no 'Bounds' component.");

	return bounds->bounds;
}

double ClampX (Entity entity, double x)
{
	auto bounds = g_registry.try_get<Components::Bounds>(entity);
	ZEN_ASSERT(bounds, "The entity has no 'Bounds' component.");

	double bx = bounds->bounds.x + ( (GetDisplayWidth(entity) - GetWidth(entity)) / 2.);
	double bw = std::max(bx, bx + bounds->bounds.width - GetDisplayWidth(entity));

	return Math::Clamp(x, bx, bw);
}

double ClampY (Entity entity, double y)
{
	auto bounds = g_registry.try_get<Components::Bounds>(entity);
	ZEN_ASSERT(bounds, "The entity has no 'Bounds' component.");

	double by = bounds->bounds.y + ( (GetDisplayHeight(entity) - GetHeight(entity)) / 2.);
	double bh = std::max(by, by + bounds->bounds.height - GetDisplayHeight(entity));

	return Math::Clamp(y, by, bh);
}

}	// namespace Zen
