/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../rotation.hpp"

#include "../../utils/assert.hpp"
#include "../../math/const.hpp"
#include "../../math/angle/wrap_degrees.hpp"
#include "../../math/angle/wrap_radians.hpp"
#include "../../components/rotation.hpp"
#include "../../components/container_item.hpp"
#include "../../components/dirty.hpp"

namespace Zen {

extern entt::registry g_registry;

void SetAngle (Entity entity, double value)
{
	auto [rotation, dirty] = g_registry.try_get<Components::Rotation, Components::Dirty>(entity);
	ZEN_ASSERT(rotation, "The entity has no 'Rotation' component.");

	rotation->value = Math::WrapDegrees(value * Math::DEG_TO_RAD);

	if (dirty)
		dirty->value = true;
}

double GetAngle (Entity entity)
{
	auto rotation = g_registry.try_get<Components::Rotation>(entity);
	ZEN_ASSERT(rotation, "The entity has no 'Rotation' component.");

	return Math::WrapDegrees(rotation->value * Math::RAD_TO_DEG);
}

void SetRotation (Entity entity, double value)
{
	auto [rotation, dirty] = g_registry.try_get<Components::Rotation, Components::Dirty>(entity);
	ZEN_ASSERT(rotation, "The entity has no 'Rotation' component.");

	rotation->value = Math::WrapRadians(value);

	if (dirty)
		dirty->value = true;
}

double GetRotation (Entity entity)
{
	auto rotation = g_registry.try_get<Components::Rotation>(entity);
	ZEN_ASSERT(rotation, "The entity has no 'Rotation' component.");

	return Math::WrapRadians(rotation->value);
}

double GetParentRotation (Entity entity)
{
	auto item = g_registry.try_get<Components::ContainerItem>(entity);
	double rotation = 0.;

	while (item)
	{
		auto parent = item->parent;
		auto parentRotation = g_registry.try_get<Components::Rotation>(parent);

		if (parentRotation)
			rotation += parentRotation->value;

		item = g_registry.try_get<Components::ContainerItem>(parent);
	}

	return rotation;
}

}	// namespace Zen
