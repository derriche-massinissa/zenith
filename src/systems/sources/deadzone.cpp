/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../deadzone.hpp"

#include "../../geom/rectangle.hpp"

#include "../position.hpp"
#include "../size.hpp"
#include "../../components/deadzone.hpp"
#include "../../utils/assert.hpp"

namespace Zen {

extern entt::registry g_registry;

void SetDeadzone (Entity entity, double width, double height)
{
	auto deadzone = g_registry.try_get<Components::Deadzone>(entity);
	ZEN_ASSERT(deadzone, "The entity has no 'Deadzone' component.");

	if (width < 0)
	{
		g_registry.remove_if_exists<Components::Deadzone>(entity);
	}
	else {
		g_registry.emplace_or_replace<Components::Deadzone>(entity, Rectangle {0., 0., width, height});
	}
}

}	// namespace Zen
