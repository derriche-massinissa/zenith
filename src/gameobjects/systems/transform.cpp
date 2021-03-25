/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "transform.hpp"

#include "../../math/const.h"
#include "../../math/transform_xy.h"
#include "../../math/angle/wrap.h"
#include "../../math/angle/wrap_degrees.h"
#include "../components/actor.hpp"
#include "../../scale/scale_manager.h"

#define FLAG 0b0100

namespace Zen {

extern ScaleManager g_scale;
extern entt::g_registry registry;

void SetScale (Entity entity, double value)
{
	auto [transform, renderable] = g_registry.try_get<TransformComponent, RenderableComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	transform->scaleX = value;
	transform->scaleY = value;

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
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	return (transform->scaleX + transform->scaleY) / 2.;
}

void SetScaleX (Entity entity, double value)
{
	auto [transform, renderable] = g_registry.try_get<TransformComponent, RenderableComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	transform->scaleX = value;

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
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	return transform->scaleX;
}

void SetScaleY (Entity entity, double value)
{
	auto [transform, renderable] = g_registry.try_get<TransformComponent, RenderableComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	transform->scaleY = value;

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
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	return transform->scaleY;
}

void SetAngle (Entity entity, double value)
{
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	transform->rotation = Math::WrapDegrees(value * Math::DEG_TO_RAD);
}

double GetAngle (Entity entity)
{
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	return Math::WrapDegrees(transform->rotation * Math::RAD_TO_DEG);
}

void SetRotation (Entity entity, double value)
{
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	transform->rotation = Math::WrapRadians(value);
}

double GetRotation (Entity entity)
{
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	return Math::WrapRadians(transform->rotation);
}

void SetPosition (Entity entity, double x, double y, double z, double w)
{
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	transform->x = x;
	transform->y = y;
	transform->z = z;
	transform->w = w;
}

void SetPosition (Entity entity, Math::Vector2 source)
{
	return SetPosition(entity, source.x, source.y);
}

/**
 * TODO
 * @todo Vector3 & Vector4
 void CopyPosition (Entity entity, Math::Vector3 source)
 {
	 return setPosition(source.x, source.y, source.z);
 }

 void CopyPosition (Entity, entity, Math::Vector4 source)
 {
	 return setPosition(source.x, source.y, source.z, source.w);
 }
 */

void SetRandomPosition (Entity entity, double x, double y, double width, double height)
{
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	if (width == 0)
		width = g_scale.getWidth();

	if (height == 0)
		height = g_scale.getHeight();

	transform->x = Math::Random.between(x, width);
	transform->y = Math::Random.between(y, height);
}

void SetX (Entity entity, double value)
{
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	transform->x = value;
}

void SetY (Entity entity, double value)
{
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	transform->y = value;
}

void SetZ (Entity entity, double value)
{
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	transform->z = value;
}

void SetW (Entity entity, double value)
{
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	transform->w = value;
}

double GetX (Entity entity)
{
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	return transform->x;
}

double GetY (Entity entity)
{
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	return transform->y;
}

double GetZ (Entity entity)
{
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	return transform->z;
}

double GetW (Entity entity)
{
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	return transform->w;
}

TransformMatrix GetLocalTransformMatrix (Entity entity)
{
	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	TransformMatrix out;

	ApplyITRS(&out,
			transform->x,
			transform->y,
			transform->rotation,
			transform->scaleX,
			transform->scaleY);

	return out;
}

TransformMatrix GetWorldTransformMatrix (Entity entity)
{
	auto item = g_registry.try_get<ContainerItemComponent>(entity);

	if (!item)
	{
		return GetLocalTransformMatrix(entity);
	}

	auto transform = g_registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	TransformMatrix out, parentMatrix;

	ApplyITRS(&out,
			transform->x,
			transform->y,
			transform->rotation,
			transform->scaleX,
			transform->scaleY);

	while (item)
	{
		auto parent = item->parent;
		auto parentTransform = g_registry.try_get<TransformComponent>(parent);

		if (parentTransform)
		{
			ApplyITRS(&parentMatrix,
					parentTransform->x,
					parentTransform->y,
					parentTransform->rotation,
					parentTransform->scaleX,
					parentTransform->scaleY);

			Multiply(&out, parentMatrix);
		}

		item = g_registry.try_get<ContainerItemComponent>(parent);
	}

	return out;
}

Math::Vector2 GetLocalPoint (Entity entity, double x, double y, Entity camera)
{
	auto [actor, scrollFactor, transform, origin, item] = g_registry.try_get<
		ActorComponent,
		ScrollFactorComponent,
		TransformComponent,
		OriginComponent,
		ContainerItemComponent>(entity);
	ZEN_ASSERT(actor & scrollFactor & transform, "The entity has no 'Actor', 'ScrollFactor' or 'Transform' component.");

	Math::Vector2 out;

	if (camera == entt::null)
	{
		auto scene = actor->scene;
		auto& cameraManager = g_registry.get<CameraManagerComponent>(scene);
		camera = cameraManager.main;
	}

	auto& scroll = g_registry.get<ScrollComponent>(camera);

	int csx = scroll->x;
	int csy = scroll->y;

	int px = x + (csx * scrollFactor.x) - csx;
	int py = y + (csy * scrollFactor.y) - csy;

	if (item)
	{
		out = ApplyInverse(GetWorldTransformMatrix(entity), px, py);
	}
	else
	{
		out = Math::TransformXY(px, py, transform->x, transform->y, transform->rotation, transform->scaleX, transform->scaleY);
	}

	// Normalize origin
	if (origin)
	{
		out.x = out.x + GetDisplayOriginX(entity);
		out.y = out.y + GetDisplayOriginY(entity);
	}

	return out;
}

double GetParentRotation (Entity entity)
{
	auto item = g_registry.try_get<ContainerItemComponent>(entity);
	double rotation = 0.;

	while (item)
	{
		auto parent = item->parent;
		auto parentTransform = g_registry.try_get<TransformComponent>(parent);

		if (parentTransform)
			rotation += parentTransform->rotation;

		item = g_registry.try_get<ContainerItemComponent>(parent);
	}

	return rotation;
}

}	// namespace Zen

#undef FLAG
