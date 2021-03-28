/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../zoom.hpp"

#include <algorithm>
#include "../../utils/assert.hpp"
#include "../../components/zoom.hpp"
#include "../../components/dirty.hpp"

namespace Zen {

extern entt::registry g_registry;

void SetZoomX (Entity entity, double value)
{
	auto [zoom, dirty] = g_registry.try_get<Components::Zoom, Components::Dirty>(entity);
	ZEN_ASSERT(zoom, "The entity has no 'Zoom' component.");

	value = std::max(value, 0.001);

	zoom->x = value;

	if (dirty)
		dirty->value = true;
}

void SetZoomY (Entity entity, double value)
{
	auto [zoom, dirty] = g_registry.try_get<Components::Zoom, Components::Dirty>(entity);
	ZEN_ASSERT(zoom, "The entity has no 'Zoom' component.");

	value = std::max(value, 0.001);

	zoom->y = value;

	if (dirty)
		dirty->value = true;
}

void SetZoom (Entity entity, double x, double y)
{
	auto [zoom, dirty] = g_registry.try_get<Components::Zoom, Components::Dirty>(entity);
	ZEN_ASSERT(zoom, "The entity has no 'Zoom' component.");

	x = std::max(x, 0.001);

	if (y <= 0.)
		y = x;

	zoom->x = x;
	zoom->y = y;

	if (dirty)
		dirty->value = true;
}

double GetZoomX (Entity entity)
{
	auto zoom = g_registry.try_get<Components::Zoom>(entity);
	ZEN_ASSERT(zoom, "The entity has no 'Zoom' component.");

	return zoom->x;
}

double GetZoomY (Entity entity)
{
	auto zoom = g_registry.try_get<Components::Zoom>(entity);
	ZEN_ASSERT(zoom, "The entity has no 'Zoom' component.");

	return zoom->y;
}

double GetZoom (Entity entity)
{
	auto zoom = g_registry.try_get<Components::Zoom>(entity);
	ZEN_ASSERT(zoom, "The entity has no 'Zoom' component.");

	return (zoom->x + zoom->y) / 2.;
}

}	// namespace Zen
