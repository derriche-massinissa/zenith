/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../viewport.hpp"

#include "../position.hpp"
#include "../size.hpp"
#include "../../components/viewport.hpp"
#include "../../utils/assert.hpp"

namespace Zen {

extern entt::registry g_registry;

void SetViewport (Entity entity, double x, double y, double width, double height)
{
	auto viewport = g_registry.try_get<Components::Viewport>(entity);
	ZEN_ASSERT(viewport, "The entity has no 'Viewport' component.");

	height = (height == 0.) ? width : height;

	SetPosition(entity, x, y);
	SetSize(entity, width, height);
}

bool GetViewport (Entity entity)
{
	auto viewport = g_registry.try_get<Components::Viewport>(entity);
	ZEN_ASSERT(viewport, "The entity has no 'Viewport' component.");

	return viewport->value;
}

}	// namespace Zen
