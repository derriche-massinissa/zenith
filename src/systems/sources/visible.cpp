/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../visible.hpp"

#include "../../utils/assert.hpp"
#include "../../components/visible.hpp"
#include "../../components/renderable.hpp"

#define FLAG 0b0001

namespace Zen {

extern entt::registry g_registry;

bool GetVisible (Entity entity)
{
	auto visible = g_registry.try_get<Components::Visible>(entity);

	ZEN_ASSERT(visible, "The entity has no 'Visible' component.");

	return visible->value;
}

void SetVisible (Entity entity, bool value)
{
	auto visible = g_registry.try_get<Components::Visible>(entity);

	ZEN_ASSERT(visible, "The entity has no 'Visible' component.");

	visible->value = value;

	if(auto renderable = g_registry.try_get<Components::Renderable>(entity))
	{
		if (value)
			// Turn the visibility bit to 1
			renderable->flags |= FLAG;
		else
			// Turn the visibility bit to 0
			renderable->flags &= ~FLAG;
	}
}

}	// namespace Zen

#undef FLAG
