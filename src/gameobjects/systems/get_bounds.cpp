/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "get_bounds.hpp"

namespace Zen {

static Math::Vector2 PrepareBoundsOutput (Math::Vector2 vector, bool includeParent)
{
	auto transform = registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	// Take rotation into account
	if (transform->rotation != 0)
		vector = Math::RotateAround(vector, transform->x, transform->y, transform->rotation);

	// Take parent's transforms into account
	auto item = registry.try_get<ContainerItemComponent>(entity);
	if (includeParent && item)
	{
		auto parent = item->parent;

		auto [transform, matrix] = registry.try_get<TransformComponent, TransformMatrix>(parent);

		if (transform & matrix)
		{
			// FIXME GetBounds
			auto parentMatrix = container->parent->getBoundsTransformMatrix();
			parentMatrix.transformPoint(vector.x, vector.y, vector);
		}
	}

	return input;
}

/**
 * Gets the center coordinate of this Game Object, regardless of origin.
 * The returned point is calculated in local space and does not factor in any parent containers
 *
 * @since 0.0.0
 *
 * @return The values stored in the output object.
 */
Math::Vector2 GetCenter (Entity entity)
{
	auto transform = registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	Math::Vector2 output;

	output.x = transform->x -
		GetDisplayWidth(entity) * GetOriginX(entity) +
		GetDisplayWidth(entity) / 2.0;
	output.y = transform->y -
		GetDisplayHeight(entity) * GetOriginY(entity) +
		GetDisplayHeight(entity) / 2.0;

	return output;
}

/**
 * Gets the top-left corner coordinate of this Game Object, regardless of
 * origin.
 * The returned point is calculated in local space and does not factor in any
 * parent containers
 *
 * @since 0.0.0
 *
 * @param output An object to store the values in. If not provided a new 
 * Vector2 will be created.
 * @param includeParent If this Game Object has a parent Container, include it 
 * (and all other ancestors) in the resulting vector?
 *
 * @return The values stored in the output object.
 */
Math::Vector2 GetTopLeft (Entity entity, bool includeParent)
{
	auto transform = registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	Math::Vector2 output;

	output.x = transform->x - GetDisplayWidth(entity) * GetOriginX(entity);
	output.y = transform->y - GetDisplayHeight(entity) * GetOriginY(entity);

	return PrepareBoundsOutput(output, includeParent);
}

Math::Vector2 GetTopCenter (Entity entity, bool includeParent)
{
	auto transform = registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	Math::Vector2 output;

	output.x = transform->x -
		(GetDisplayWidth(entity) * GetOriginX(entity)) +
		(GetDisplayWidth(entity) / 2.0);

	output.y = transform->y -
		(GetDisplayHeight(entity) * GetOriginY(entity));

	return PrepareBoundsOutput(output, includeParent);
}

Math::Vector2 GetTopRight (Entity entity, bool includeParent)
{
	auto transform = registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	Math::Vector2 output;

	output.x =
		transform->x -
		(GetDisplayWidth(entity) * GetOriginX(entity)) +
		GetDisplayWidth(entity);

	output.y =
		transform->y -
		(GetDisplayHeight(entity) * GetOriginY(entity));

	return PrepareBoundsOutput(output, includeParent);
}

Math::Vector2 GetLeftCenter (Entity entity, bool includeParent)
{
	auto transform = registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	Math::Vector2 output;

	output.x =
		transform->x -
		(GetDisplayWidth(entity) * GetOriginX(entity));

	output.y =
		transform->y -
		(GetDisplayHeight(entity) * GetOriginY(entity)) +
		(GetDisplayHeight(entity) / 2.0);

	return PrepareBoundsOutput(output, includeParent);
}

Math::Vector2 GetRightCenter (Entity entity, bool includeParent)
{
	auto transform = registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	Math::Vector2 output;

	output.x =
		transform->x -
		(GetDisplayWidth(entity) * GetOriginX(entity)) +
		GetDisplayWidth(entity);

	output.y =
		transform->y -
		(GetDisplayHeight(entity) * GetOriginY(entity)) +
		(GetDisplayHeight(entity) / 2.0);

	return PrepareBoundsOutput(output, includeParent);
}

Math::Vector2 GetBottomLeft (Entity entity, bool includeParent)
{
	auto transform = registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	Math::Vector2 output;

	output.x =
		transform->x -
		(GetDisplayWidth(entity) * GetOriginX(entity));

	output.y =
		transform->y -
		(GetDisplayHeight(entity) * GetOriginY(entity)) +
		GetDisplayHeight(entity);

	return PrepareBoundsOutput(output, includeParent);
}

Math::Vector2 GetBottomCenter (Entity entity, bool includeParent)
{
	auto transform = registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	Math::Vector2 output;

	output.x =
		transform->x -
		(GetDisplayWidth(entity) * GetOriginX(entity)) +
		(GetDisplayWidth(entity) / 2.0);

	output.y =
		transform->y -
		(GetDisplayHeight(entity) * GetOriginY(entity)) +
		GetDisplayHeight(entity);

	return PrepareBoundsOutput(output, includeParent);
}

Math::Vector2 GetBottomRight (Entity entity, bool includeParent)
{
	auto transform = registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	Math::Vector2 output;

	output.x =
		transform->x -
		(GetDisplayWidth(entity) * GetOriginX(entity)) +
		GetDisplayWidth(entity);

	output.y =
		transform->y -
		(GetDisplayHeight(entity) * GetOriginY(entity)) +
		GetDisplayHeight(entity);

	return PrepareBoundsOutput(output, includeParent);
}

Geom::Rectangle GetBounds (Entity entity)
{
	auto transform = registry.try_get<TransformComponent>(entity);
	ZEN_ASSERT(transform, "The entity has no 'Transform' component.");

	Geom::Rectangle output;

	int TLx, TLy, TRx, TRy, BLx, BLy, BRx, BRy;

	// Is this entity an item of a container
	auto item = registry.try_get<ContainerItemComponent>(entity);
	if (item)
	{
		auto parent = item->parent;
		auto parentTransform = registry.try_get<TransformMatrix>(parent);

		auto& parentMatrix = item->parent->getBoundsTransformMatrix();

		output = GetTopLeft(entity);
		output = parentMatrix.transformPoint(output.x, output.y, output);

		TLx = output.x;
		TLy = output.y;

		output = GetTopRight(entity);
		output = parentMatrix.transformPoint(output.x, output.y, output);

		TRx = output.x;
		TRy = output.y;

		output = GetBottomLeft(entity);
		output = parentMatrix.transformPoint(output.x, output.y, output);

		BLx = output.x;
		BLy = output.y;

		output = GetBottomRight(entity);
		output = parentMatrix.transformPoint(output.x, output.y, output);

		BRx = output.x;
		BRy = output.y;
	}
	else
	{
		output = GetTopLeft(entity);

		TLx = output.x;
		TLy = output.y;

		output = GetTopRight(entity);

		TRx = output.x;
		TRy = output.y;

		output = GetBottomLeft(entity);

		BLx = output.x;
		BLy = output.y;

		output = GetBottomRight(entity);

		BRx = output.x;
		BRy = output.y;
	}

	output.x = std::min(TLx, std::min(TRx, std::min(BLx, BRx)));
	output.y = std::min(TLy, std::min(TRy, std::min(BLy, BRy)));
	output.width = std::max(TLx, std::min(TRx, std::min(BLx, BRx))) - output.x;
	output.height = std::max(TLy, std::min(TRy, std::min(BLy, BRy))) - output.y;

	return output;
}

}	// namespace Zen
