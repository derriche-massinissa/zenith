/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../tint.hpp"

#include "../../components/tint.hpp"
#include "../../utils/assert.hpp"
#include "../../display/color.hpp"

namespace Zen {

extern entt::registry g_registry;

void ClearTint (Entity entity)
{
	SetTint(entity, 0xffffff);
}

void SetTint (Entity entity, int topLeft, int topRight, int bottomLeft, int bottomRight)
{
	auto tint = g_registry.try_get<Components::Tint>(entity);

	ZEN_ASSERT(tint, "The entity has no 'Tint' component.");

	tint->tint = topLeft;

	tint->tl = topLeft;

	if (topRight < 0)
	{
		tint->tr = topLeft;
		tint->bl = topLeft;
		tint->br = topLeft;
	}
	else
	{
		tint->tr = topRight;
		tint->bl = bottomLeft;
		tint->br = bottomRight;
	}

	tint->fill = false;
}

void SetTintFill (Entity entity, int topLeft, int topRight, int bottomLeft, int bottomRight)
{
	SetTint(entity, topLeft, topRight, bottomLeft, bottomRight);

	g_registry.get<Components::Tint>(entity).fill = true;
}

Color GetTint (Entity entity)
{
	auto tint = g_registry.try_get<Components::Tint>(entity);
	ZEN_ASSERT(tint, "The entity has no 'Tint' component.");

	Color output;
	SetHex(&output, tint->tint);

	return output;
}

void GetTint (Entity entity, int *tl, int *tr, int *bl, int *br)
{
	auto tint = g_registry.try_get<Components::Tint>(entity);
	ZEN_ASSERT(tint, "The entity has no 'Tint' component.");

	*tl = tint->tl;
	*tr = tint->tr;
	*bl = tint->bl;
	*br = tint->br;
}

bool IsTinted (Entity entity)
{
	auto tint = g_registry.try_get<Components::Tint>(entity);
	ZEN_ASSERT(tint, "The entity has no 'Tint' component.");

	return (tint->fill ||
			tint->tl != 0xffffff ||
			tint->tr != 0xffffff ||
			tint->bl != 0xffffff ||
			tint->br != 0xffffff
		   );
}

bool IsTintFilled (Entity entity)
{
	auto tint = g_registry.try_get<Components::Tint>(entity);
	ZEN_ASSERT(tint, "The entity has no 'Tint' component.");

	return tint->fill;
}

}	// namespace Zen
