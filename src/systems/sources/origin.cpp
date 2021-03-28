/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../origin.hpp"

#include "../../utils/assert.hpp"

#include "../../components/size.hpp"
#include "../../components/textured.hpp"
#include "../../components/origin.hpp"

#include "../../texture/components/frame.hpp"

namespace Zen {

extern entt::registry registry;

void SetDisplayOriginX (Entity entity, int value)
{
	auto [origin, size] = registry.try_get<Components::Origin, Components::Size>(entity);
	ZEN_ASSERT(origin && size, "The entity has no 'Origin' or 'Size' component.");

	origin->displayX = value;
	origin->x = value / size->width;
}

void SetDisplayOriginY (Entity entity, int value)
{
	auto [origin, size] = registry.try_get<Components::Origin, Components::Size>(entity);
	ZEN_ASSERT(origin && size, "The entity has no 'Origin' or 'Size' component.");

	origin->displayY = value;
	origin->y = value / size->height;
}

void SetDisplayOrigin (Entity entity, int x, int y)
{
	auto [origin, size] = registry.try_get<Components::Origin, Components::Size>(entity);
	ZEN_ASSERT(origin && size, "The entity has no 'Origin' or 'Size' component.");

	origin->displayX = x;
	origin->displayY = y;
	origin->x = x / size->width;
	origin->y = y / size->height;
}

void SetDisplayOrigin (Entity entity, int value = 0)
{
	SetDisplayOrigin(entity, value, value);
}

void SetOrigin (Entity entity, double x, double y)
{
	auto [origin, size] = registry.try_get<Components::Origin, Components::Size>(entity);
	ZEN_ASSERT(origin, "The entity has no 'Origin' component.");

	origin->x = x;
	origin->y = y;

	// Update display origin
	if (size)
	{
		origin->displayX = origin->x * size->width;
		origin->displayY = origin->y * size->height;
	}
}

void SetOrigin (Entity entity, double value)
{
	SetOrigin(entity, value, value);
}

void SetOriginFromFrame (Entity entity)
{
	auto [origin, size, textured] = registry.try_get<Components::Origin, Components::Size, Components::Textured>(entity);
	ZEN_ASSERT(origin && size, "The entity has no 'Origin', 'Size' or 'Textured' component.");

	// Get texture
	auto& frame = registry.get<Components::Frame>(textured->frame);

	if (!textured || !frame.customPivot)
	{
		SetOrigin(entity);
		return;
	}
	else
	{
		origin->x = frame.pivotX;
		origin->y = frame.pivotY;
	}

	// Update display origin
	origin->displayX = origin->x * size->width;
	origin->displayY = origin->y * size->height;
}

double GetOriginX (Entity entity)
{
	auto origin = registry.try_get<Components::Origin>(entity);
	ZEN_ASSERT(origin, "The entity has no 'Origin' component.");

	return origin->x;
}

double GetOriginY (Entity entity)
{
	auto origin = registry.try_get<Components::Origin>(entity);
	ZEN_ASSERT(origin, "The entity has no 'Origin' component.");

	return origin->y;
}

double GetDisplayOriginX (Entity entity)
{
	auto origin = registry.try_get<Components::Origin>(entity);
	ZEN_ASSERT(origin, "The entity has no 'Origin' component.");

	return origin->displayX;
}

double GetDisplayOriginY (Entity entity)
{
	auto origin = registry.try_get<Components::Origin>(entity);

	ZEN_ASSERT(origin, "The entity has no 'Origin' component.");
	return origin->displayY;
}

void UpdateDisplayOrigin (Entity entity)
{
	auto [origin, size] = registry.try_get<Components::Origin, Components::Size>(entity);
	ZEN_ASSERT(origin && size, "The entity has no 'Origin' or 'Size' component.");

	// Update display origin
	origin->displayX = origin->x * size->width;
	origin->displayY = origin->y * size->height;
}

}	// namespace Zen
