/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../scroll.hpp"

#include "../../utils/assert.hpp"
#include "../../geom/rectangle.hpp"
#include "../../components/scroll.hpp"
#include "../../components/dirty.hpp"
#include "../../components/bounds.hpp"
#include "../../components/size.hpp"
#include "../../components/mid_point.hpp"
#include "../bounds.hpp"

namespace Zen {

extern entt::registry g_registry;

void SetScrollX (Entity entity, double value)
{
	auto [scroll, dirty] = g_registry.try_get<Components::Scroll, Components::Dirty>(entity);
	ZEN_ASSERT(scroll, "The entity has no 'Scroll' component.");

	scroll->x = value;

	if (dirty)
		dirty->value = true;
}

void SetScrollY (Entity entity, double value)
{
	auto [scroll, dirty] = g_registry.try_get<Components::Scroll, Components::Dirty>(entity);
	ZEN_ASSERT(scroll, "The entity has no 'Scroll' component.");

	scroll->y = value;

	if (dirty)
		dirty->value = true;
}

void SetScroll (Entity entity, double x, double y)
{
	auto [scroll, dirty] = g_registry.try_get<Components::Scroll, Components::Dirty>(entity);
	ZEN_ASSERT(scroll, "The entity has no 'Scroll' component.");

	scroll->x = x;
	scroll->y = y;

	if (dirty)
		dirty->value = true;
}

void SetScroll (Entity entity, double value)
{
	SetScroll(entity, value, value);
}

double GetScrollX (Entity entity)
{
	auto scroll = g_registry.try_get<Components::Scroll>(entity);
	ZEN_ASSERT(scroll, "The entity has no 'Scroll' component.");

	return scroll->x;
}

double GetScrollY (Entity entity)
{
	auto scroll = g_registry.try_get<Components::Scroll>(entity);
	ZEN_ASSERT(scroll, "The entity has no 'Scroll' component.");

	return scroll->y;
}

void CenterOnX (Entity entity, double x)
{
	auto [bounds, size, midPoint] = g_registry.try_get<Components::Bounds, Components::Size, Components::MidPoint>(entity);
	ZEN_ASSERT(size, "The entity has no 'Size' component.");

	double originX = size->width * 0.5;

	if (bounds)
		SetScrollX(entity, ClampX(entity, x - originX));
	else
		SetScrollX(entity, x - originX);

	if (midPoint)
		midPoint->x = x;
}

void CenterOnY (Entity entity, double y)
{
	auto [bounds, size, midPoint] = g_registry.try_get<Components::Bounds, Components::Size, Components::MidPoint>(entity);
	ZEN_ASSERT(size, "The entity has no 'Size' component.");

	double originY = size->height * 0.5;

	if (bounds)
		SetScrollY(entity, ClampY(entity, y - originY));
	else
		SetScrollY(entity, y - originY);

	if (midPoint)
		midPoint->y = y;
}

void CenterOn (Entity entity, double x, double y)
{
	auto [bounds, size, midPoint] = g_registry.try_get<Components::Bounds, Components::Size, Components::MidPoint>(entity);
	ZEN_ASSERT(size, "The entity has no 'Size' component.");

	double originX = size->width * 0.5;
	double originY = size->height * 0.5;

	if (bounds)
	{
		SetScrollX(entity, ClampX(entity, x - originX));
		SetScrollY(entity, ClampY(entity, y - originY));
	}
	else
	{
		SetScrollX(entity, x - originX);
		SetScrollY(entity, y - originY);
	}

	if (midPoint)
	{
		midPoint->x = x;
		midPoint->y = y;
	}
}

Math::Vector2 GetScroll (Entity entity, double x, double y)
{
	auto [bounds, size] = g_registry.try_get<Components::Bounds, Components::Size>(entity);
	ZEN_ASSERT(size, "The entity has no 'Size' component.");

	Math::Vector2 output;

	double originX = size->width * 0.5;
	double originY = size->height * 0.5;
	
	if (bounds)
	{
		output.x = ClampX(entity, x - originX);
		output.y = ClampY(entity, y - originY);
	}
	else
	{
		output.x = x - originX;
		output.y = y - originY;
	}

	return output;
}

void CenterToBounds(Entity entity)
{
	auto [bounds, size, midPoint] = g_registry.try_get<Components::Bounds, Components::Size, Components::MidPoint>(entity);
	ZEN_ASSERT(size, "The entity has no 'Size' component.");

	double originX = size->width * 0.5;
	double originY = size->height * 0.5;

	double sx = GetCenterX(bounds->bounds) - originX;
	double sy = GetCenterY(bounds->bounds) - originY;

	SetScrollX(entity, sx);
	SetScrollY(entity, sy);

	if (midPoint)
	{
		midPoint->x = sx;
		midPoint->y = sy;
	}
}

void CenterToSize (Entity entity)
{
	auto size = g_registry.try_get<Components::Size>(entity);
	ZEN_ASSERT(size, "The entity has no 'Size' component.");

	SetScroll(entity, size->width * 0.5, size->height * 0.5);
}

}	// namespace Zen
