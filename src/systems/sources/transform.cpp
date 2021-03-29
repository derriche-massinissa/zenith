/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../transform.hpp"

#include "../../utils/assert.hpp"
#include "../../math/transform_xy.hpp"
#include "../transform_matrix.hpp"
#include "../../components/transform_matrix.hpp"
#include "../../components/container_item.hpp"
#include "../../components/position.hpp"
#include "../../components/rotation.hpp"
#include "../../components/scale.hpp"
#include "../../components/actor.hpp"
#include "../../components/scroll_factor.hpp"
#include "../../components/origin.hpp"
#include "../../components/scroll.hpp"
#include "../origin.hpp"

namespace Zen {

extern entt::registry g_registry;

Components::TransformMatrix GetLocalTransformMatrix (Entity entity)
{
	auto [position, rotation, scale] = g_registry.try_get<Components::Position, Components::Rotation, Components::Scale>(entity);
	ZEN_ASSERT(position && rotation && scale, "The entity has no 'Position', 'rotation' or 'scale' component.");

	Components::TransformMatrix out;

	ApplyITRS(&out,
			position->x,
			position->y,
			rotation->value,
			scale->x,
			scale->y);

	return out;
}

Components::TransformMatrix GetWorldTransformMatrix (Entity entity)
{
	auto item = g_registry.try_get<Components::ContainerItem>(entity);

	if (!item)
	{
		return GetLocalTransformMatrix(entity);
	}

	auto [position, rotation, scale] = g_registry.try_get<Components::Position, Components::Rotation, Components::Scale>(entity);
	ZEN_ASSERT(position && rotation && scale, "The entity has no 'Position', 'rotation' or 'scale' component.");

	Components::TransformMatrix out, parentMatrix;

	ApplyITRS(&out,
			position->x,
			position->y,
			rotation->value,
			scale->x,
			scale->y);

	while (item)
	{
		auto parent = item->parent;
		auto [parentPosition, parentRotation, parentScale] = g_registry.try_get<Components::Position, Components::Rotation, Components::Scale>(parent);

		if (parentPosition && parentRotation && parentScale)
		{
			ApplyITRS(&parentMatrix,
					parentPosition->x,
					parentPosition->y,
					parentRotation->value,
					parentScale->x,
					parentScale->y);

			Multiply(&out, parentMatrix);
		}

		item = g_registry.try_get<Components::ContainerItem>(parent);
	}

	return out;
}

Math::Vector2 GetLocalPoint (Entity entity, double x, double y, Entity camera)
{
	auto [actor, scrollFactor, position, rotation, scale, origin, item] = g_registry.try_get<
		Components::Actor,
		Components::ScrollFactor,
		Components::Position,
		Components::Rotation,
		Components::Scale,
		Components::Origin,
		Components::ContainerItem>(entity);
	ZEN_ASSERT(actor && scrollFactor && position && rotation && scale, "The entity has no 'Actor', 'ScrollFactor', 'Position', 'Rotation' or 'Scale' component.");

	Math::Vector2 out;

	/**
	 * @todo TODO
	 */
	if (camera == entt::null)
	{
		return {-1, -1};

		///auto scene = actor->scene;
		///auto& cameraManager = g_registry.get<Components::CameraManager>(scene);
		///camera = cameraManager.main;
	}

	auto& scroll = g_registry.get<Components::Scroll>(camera);

	int csx = scroll.x;
	int csy = scroll.y;

	int px = x + (csx * scrollFactor->x) - csx;
	int py = y + (csy * scrollFactor->y) - csy;

	if (item)
	{
		out = ApplyInverse(GetWorldTransformMatrix(entity), px, py);
	}
	else
	{
		out = Math::TransformXY(px, py, position->x, position->y, rotation->value, scale->x, scale->y);
	}

	// Normalize origin
	if (origin)
	{
		out.x = out.x + GetDisplayOriginX(entity);
		out.y = out.y + GetDisplayOriginY(entity);
	}

	return out;
}

}	// namespace Zen

