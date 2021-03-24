/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "size.hpp"

#include <cmath>
#include "entt/entt.hpp"
#include "../components/size.hpp"
#include "../../utils/assert.hpp"
#include "../../texture/frame.h"

namespace Zen {

extern entt::registry registry;

int GetDisplayWidth (Entity entity)
{
	auto [transform, texture] = registry.try_get<TransformComponent, TextureComponent>(entity);

	ZEN_ASSERT(transform & texture, "The entity has no 'Transform' or 'Texture' component.");

	return std::abs( transform->scaleX * texture->frame->realWidth );
}

int GetDisplayHeight (Entity entity)
{
	auto [transform, texture] = registry.try_get<TransformComponent, TextureComponent>(entity);

	ZEN_ASSERT(transform & texture, "The entity has no 'Transform' or 'Texture' component.");

	return std::abs( transform->scaleY * texture->frame->realHeight );
}

void SetDisplayWidth (Entity entity, double value)
{
	auto [transform, texture] = registry.try_get<TransformComponent, TextureComponent>(entity);

	ZEN_ASSERT(transform & texture, "The entity has no 'Transform' or 'Texture' component.");

	transform->scaleX = value / texture->frame->realWidth;
}

void SetDisplayHeight (Entity entity, double value)
{
	auto [transform, texture] = registry.try_get<TransformComponent, TextureComponent>(entity);

	ZEN_ASSERT(transform & texture, "The entity has no 'Transform' or 'Texture' component.");

	transform->scaleY = value / texture->frame->realHeight;
}

// TODO
void SetSizeToFrame (Entity entity, Textures::Frame* frame = nullptr)
{
	if (!frame_)
		frame_ = This()->frame;

	width = frame_->getRealWidth();
	height = frame_->getRealHeight();
}

void SetSize (Entity entity, double width, double height)
{
	auto size = registry.try_get<SizeComponent>(entity);

	ZEN_ASSERT(size, "The entity has no 'Size' component.");

	size->width = width;
	size->height = height;
}

void SetDisplaySize (Entity entity, double width, double height)
{
	auto [transform, texture] = registry.try_get<TransformComponent, TextureComponent>(entity);

	ZEN_ASSERT(transform & texture, "The entity has no 'Transform' or 'Texture' component.");

	transform->scaleX = width / texture->frame->realWidth;
	transform->scaleY = height / texture->frame->realHeight;
}

}	// namespace Zen
