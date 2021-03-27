/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "flip.hpp"

#include "entt/entt.hpp"
#include "../components/flip.hpp"
#include "../../utils/assert.hpp"

namespace Zen {

extern entt::registry registry;

void ToggleFlipX (Entity entity)
{
	auto flip = registry.try_get<FlipComponent>(entity);

	ZEN_ASSERT(flip, "The entity has no 'Flip' component.");

	flip->x = !flip->x;
}

void ToggleFlipY (Entity entity)
{
	auto flip = registry.try_get<FlipComponent>(entity);

	ZEN_ASSERT(flip, "The entity has no 'Flip' component.");

	flip->y = !flip->y;
}

void SetFlipX (Entity entity, bool value)
{
	auto flip = registry.try_get<FlipComponent>(entity);

	ZEN_ASSERT(flip, "The entity has no 'Flip' component.");

	flip->x = value;
}

void SetFlipY (Entity entity, bool value)
{
	auto flip = registry.try_get<FlipComponent>(entity);

	ZEN_ASSERT(flip, "The entity has no 'Flip' component.");

	flip->y = value;
}

void SetFlip (Entity entity, bool x, bool y)
{
	auto flip = registry.try_get<FlipComponent>(entity);

	ZEN_ASSERT(flip, "The entity has no 'Flip' component.");

	flip->x = x;
	flip->y = y;
}

void ResetFlip (Entity entity)
{
	auto flip = registry.try_get<FlipComponent>(entity);

	ZEN_ASSERT(flip, "The entity has no 'Flip' component.");

	flip->x = false;
	flip->y = false;
}

bool GetFlipX (Entity entity)
{
	auto flip = registry.try_get<FlipComponent>(entity);

	ZEN_ASSERT(flip, "The entity has no 'Flip' component.");

	return flip->x;
}

bool GetFlipY (Entity entity)
{
	auto flip = registry.try_get<FlipComponent>(entity);

	ZEN_ASSERT(flip, "The entity has no 'Flip' component.");

	return flip->y;
}

}	// namespace Zen
