/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../size.hpp"

#include <cmath>
#include "../../utils/assert.hpp"

#include "../../components/size.hpp"
#include "../../components/scale.hpp"
#include "../../components/textured.hpp"
#include "../../components/update.hpp"
#include "../../components/zoom.hpp"

#include "../../texture/components/frame.hpp"

namespace Zen {

extern entt::registry registry;

double GetDisplayWidth (Entity entity)
{
	auto [scale, textured, size, zoom] = registry.try_get<Components::Scale, Components::Textured, Components::Size, Components::Zoom>(entity);
	ZEN_ASSERT((scale && textured) || (size && zoom), "The entity has no 'Scale', 'Textured', 'Size' or 'Zoom' component.");

	if (scale && textured)
	{
		auto frame = registry.get<Components::Frame>(textured->frame);
		return std::abs( scale->x * frame.data.sourceSize.width );
	}
	else if (size && zoom)
	{
		return size->width / zoom->x;
	}
	else
	{
		return 0;
	}
}

double GetDisplayHeight (Entity entity)
{
	auto [scale, textured, size, zoom] = registry.try_get<Components::Scale, Components::Textured, Components::Size, Components::Zoom>(entity);
	ZEN_ASSERT((scale && textured) || (size && zoom), "The entity has no 'Scale', 'Textured', 'Size' or 'Zoom' component.");

	if (scale && textured)
	{
		auto frame = registry.get<Components::Frame>(textured->frame);
		return std::abs( scale->y * frame.data.sourceSize.height );
	}
	else if (size && zoom)
	{
		return size->height / zoom->y;
	}
	else
	{
		return 0;
	}
}

void SetDisplayWidth (Entity entity, double value)
{
	auto [scale, textured] = registry.try_get<Components::Scale, Components::Textured>(entity);
	ZEN_ASSERT(scale && textured, "The entity has no 'Scale' or 'Textured' component.");

	// Get frame
	auto frame = registry.get<Components::Frame>(textured->frame);

	scale->x = value / frame.data.sourceSize.width;
}

void SetDisplayHeight (Entity entity, double value)
{
	auto [scale, textured] = registry.try_get<Components::Scale, Components::Textured>(entity);
	ZEN_ASSERT(scale && textured, "The entity has no 'Scale' or 'Textured' component.");

	// Get frame
	auto frame = registry.get<Components::Frame>(textured->frame);

	scale->y = value / frame.data.sourceSize.height;
}

void SetSizeToFrame (Entity entity, Entity frame)
{
	auto [size, textured] = registry.try_get<Components::Size, Components::Textured>(entity);
	ZEN_ASSERT(size && textured, "The entity has no 'Size' or 'Textured' component.");

	Components::Frame *fr;

	if (frame == entt::null)
	{
		fr = registry.try_get<Components::Frame>(textured->frame);
	}
	else
	{
		fr = registry.try_get<Components::Frame>(frame);
	}
	ZEN_ASSERT(fr, "The entity has no 'Frame' component.");

	size->width = fr->data.sourceSize.width;
	size->height = fr->data.sourceSize.height;
}

void SetSize (Entity entity, double width, double height)
{
	auto [size, update] = registry.try_get<Components::Size, Components::Update<Components::Size>>(entity);
	ZEN_ASSERT(size, "The entity has no 'Size' component.");

	size->width = width;
	size->height = height;

	if (update)
		update->update(entity);
}

void SetSize (Entity entity, double value)
{
	SetSize(entity, value, value);
}

void SetDisplaySize (Entity entity, double width, double height)
{
	auto [scale, textured] = registry.try_get<Components::Scale, Components::Textured>(entity);
	ZEN_ASSERT(scale && textured, "The entity has no 'Scale' or 'Textured' component.");

	// Get frame
	auto& frame = registry.get<Components::Frame>(textured->frame);

	scale->x = width / frame.data.sourceSize.width;
	scale->y = height / frame.data.sourceSize.height;
}

void SetWidth (Entity entity, double value)
{
	auto [size, update] = registry.try_get<Components::Size, Components::Update<Components::Size>>(entity);
	ZEN_ASSERT(size, "The entity has no 'Size' component.");

	size->width = value;

	if (update)
		update->update(entity);
}

void SetHeight (Entity entity, double value)
{
	auto [size, update] = registry.try_get<Components::Size, Components::Update<Components::Size>>(entity);
	ZEN_ASSERT(size, "The entity has no 'Size' component.");

	size->height = value;

	if (update)
		update->update(entity);
}

double GetWidth (Entity entity)
{
	auto size = registry.try_get<Components::Size>(entity);
	ZEN_ASSERT(size, "The entity has no 'Size' component.");

	return size->width;
}

double GetHeight (Entity entity)
{
	auto size = registry.try_get<Components::Size>(entity);
	ZEN_ASSERT(size, "The entity has no 'Size' component.");

	return size->height;
}

}	// namespace Zen
