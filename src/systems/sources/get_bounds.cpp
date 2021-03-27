/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../get_bounds.hpp"

#include "../../utils/assert.hpp"
#include "../../components/rotation.hpp"
#include "../../components/position.hpp"
#include "../../components/container_item.hpp"
#include "../origin.hpp"
#include "../size.hpp"
#include "../position.hpp"
#include "../transform_matrix.hpp"
#include "../transform.hpp"

namespace Zen {

extern entt::registry g_registry;

Math::Vector2 PrepareBoundsOutput (Entity entity, Math::Vector2 vector, bool includeParent)
{
	auto [rotation, position] = g_registry.try_get<Components::Rotation, Components::Position>(entity);
	ZEN_ASSERT(rotation && position, "The entity has no 'Rotation' or 'Position' component.");

	Math::Vector2 output;

	// Take rotation into account
	if (rotation->value != 0)
		vector = Math::RotateAround(vector, position->x, position->y, rotation->value);

	// Take parent's transforms into account
	auto item = g_registry.try_get<Components::ContainerItem>(entity);
	if (includeParent && item)
	{
		auto parent = item->parent;
		auto parentMatrix = GetWorldTransformMatrix(parent);

		output = TransformPoint(parentMatrix, vector.x, vector.y);
	}

	return output;
}

Math::Vector2 GetCenter (Entity entity)
{
	Math::Vector2 output;

	output.x = GetX(entity) -
		GetDisplayWidth(entity) * GetOriginX(entity) +
		GetDisplayWidth(entity) / 2.0;
	output.y = GetY(entity) -
		GetDisplayHeight(entity) * GetOriginY(entity) +
		GetDisplayHeight(entity) / 2.0;

	return output;
}

Math::Vector2 GetTopLeft (Entity entity, bool includeParent)
{
	Math::Vector2 output;

	output.x = GetX(entity) - GetDisplayWidth(entity) * GetOriginX(entity);
	output.y = GetY(entity) - GetDisplayHeight(entity) * GetOriginY(entity);

	return PrepareBoundsOutput(output, includeParent);
}

Math::Vector2 GetTopCenter (Entity entity, bool includeParent)
{
	Math::Vector2 output;

	output.x = GetX(entity) -
		(GetDisplayWidth(entity) * GetOriginX(entity)) +
		(GetDisplayWidth(entity) / 2.0);

	output.y = GetY(entity) -
		(GetDisplayHeight(entity) * GetOriginY(entity));

	return PrepareBoundsOutput(output, includeParent);
}

Math::Vector2 GetTopRight (Entity entity, bool includeParent)
{
	Math::Vector2 output;

	output.x =
		GetX(entity) -
		(GetDisplayWidth(entity) * GetOriginX(entity)) +
		GetDisplayWidth(entity);

	output.y =
		GetY(entity) -
		(GetDisplayHeight(entity) * GetOriginY(entity));

	return PrepareBoundsOutput(output, includeParent);
}

Math::Vector2 GetLeftCenter (Entity entity, bool includeParent)
{
	Math::Vector2 output;

	output.x =
		GetX(entity) -
		(GetDisplayWidth(entity) * GetOriginX(entity));

	output.y =
		GetY(entity) -
		(GetDisplayHeight(entity) * GetOriginY(entity)) +
		(GetDisplayHeight(entity) / 2.0);

	return PrepareBoundsOutput(output, includeParent);
}

Math::Vector2 GetRightCenter (Entity entity, bool includeParent)
{
	Math::Vector2 output;

	output.x =
		GetX(entity) -
		(GetDisplayWidth(entity) * GetOriginX(entity)) +
		GetDisplayWidth(entity);

	output.y =
		GetY(entity) -
		(GetDisplayHeight(entity) * GetOriginY(entity)) +
		(GetDisplayHeight(entity) / 2.0);

	return PrepareBoundsOutput(output, includeParent);
}

Math::Vector2 GetBottomLeft (Entity entity, bool includeParent)
{
	Math::Vector2 output;

	output.x =
		GetX(entity) -
		(GetDisplayWidth(entity) * GetOriginX(entity));

	output.y =
		GetY(entity) -
		(GetDisplayHeight(entity) * GetOriginY(entity)) +
		GetDisplayHeight(entity);

	return PrepareBoundsOutput(output, includeParent);
}

Math::Vector2 GetBottomCenter (Entity entity, bool includeParent)
{
	Math::Vector2 output;

	output.x =
		GetX(entity) -
		(GetDisplayWidth(entity) * GetOriginX(entity)) +
		(GetDisplayWidth(entity) / 2.0);

	output.y =
		GetY(entity) -
		(GetDisplayHeight(entity) * GetOriginY(entity)) +
		GetDisplayHeight(entity);

	return PrepareBoundsOutput(output, includeParent);
}

Math::Vector2 GetBottomRight (Entity entity, bool includeParent)
{
	Math::Vector2 output;

	output.x =
		GetX(entity) -
		(GetDisplayWidth(entity) * GetOriginX(entity)) +
		GetDisplayWidth(entity);

	output.y =
		GetY(entity) -
		(GetDisplayHeight(entity) * GetOriginY(entity)) +
		GetDisplayHeight(entity);

	return PrepareBoundsOutput(output, includeParent);
}

Rectangle GetBounds (Entity entity)
{
	Rectangle output;

	double TLx, TLy, TRx, TRy, BLx, BLy, BRx, BRy;
	Math::Vector2 temp;

	// Is this entity an item of a container
	auto item = g_registry.try_get<Components::ContainerItem>(entity);
	if (item)
	{
		auto parent = item->parent;
		auto parentMatrix = GetWorldTransformMatrix(parent);

		temp = GetTopLeft(entity);
		temp = TransformPoint(parentMatrix, temp.x, temp.y);

		TLx = temp.x;
		TLy = temp.y;

		temp = GetTopRight(entity);
		temp = TransformPoint(parentMatrix, temp.x, temp.y);

		TRx = temp.x;
		TRy = temp.y;

		temp = GetBottomLeft(entity);
		temp = TransformPoint(parentMatrix, temp.x, temp.y);

		BLx = temp.x;
		BLy = temp.y;

		temp = GetBottomRight(entity);
		temp = TransformPoint(parentMatrix, temp.x, temp.y);

		BRx = temp.x;
		BRy = temp.y;
	}
	else
	{
		temp = GetTopLeft(entity);

		TLx = temp.x;
		TLy = temp.y;

		temp = GetTopRight(entity);

		TRx = temp.x;
		TRy = temp.y;

		temp = GetBottomLeft(entity);

		BLx = temp.x;
		BLy = temp.y;

		temp = GetBottomRight(entity);

		BRx = temp.x;
		BRy = temp.y;
	}

	output.x = std::min(TLx, std::min(TRx, std::min(BLx, BRx)));
	output.y = std::min(TLy, std::min(TRy, std::min(BLy, BRy)));
	output.width = std::max(TLx, std::min(TRx, std::min(BLx, BRx))) - output.x;
	output.height = std::max(TLy, std::min(TRy, std::min(BLy, BRy))) - output.y;

	return output;
}

}	// namespace Zen
