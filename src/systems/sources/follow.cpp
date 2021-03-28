/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../follow.hpp"

#include "../../utils/assert.hpp"
#include "../../math/clamp.hpp"
#include "../../components/follow.hpp"

namespace Zen {

extern entt::registry g_registry;

void StartFollow (Entity entity, Entity target, double lerpX, double lerpY, double offsetX, double offsetY)
{
	auto follow = g_registry.try_get<Components::Follow>(entity);
	ZEN_ASSERT(follow, "The entity has no 'Follow' component.");

	follow->target = target;

	follow->lerp.x = Math::Clamp(lerpX, 0., 1.);
	follow->lerp.y = Math::Clamp(lerpY, 0., 1.);

	follow->offset.x = offsetX;
	follow->offset.y = offsetY;
}

void StopFollow (Entity entity)
{
	auto follow = g_registry.try_get<Components::Follow>(entity);
	ZEN_ASSERT(follow, "The entity has no 'Follow' component.");

	follow->target = entt::null;
}

void SetFollowOffset (Entity entity, double x, double y)
{
	auto follow = g_registry.try_get<Components::Follow>(entity);
	ZEN_ASSERT(follow, "The entity has no 'Follow' component.");

	follow->offset.x = x;
	follow->offset.y = y;
}

void SetFollowLerp (Entity entity, double x, double y)
{
	auto follow = g_registry.try_get<Components::Follow>(entity);
	ZEN_ASSERT(follow, "The entity has no 'Follow' component.");

	follow->lerp.x = Math::Clamp(x, 0., 1.);
	follow->lerp.y = Math::Clamp(y, 0., 1.);
}

}	// namespace Zen
