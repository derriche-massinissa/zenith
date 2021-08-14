/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../renderable.hpp"

#include "../../utils/assert.hpp"
#include "../../components/renderable.hpp"
#include "../../components/id.hpp"

namespace Zen {

extern entt::registry g_registry;

bool WillRender (Entity entity, Entity camera)
{
	auto renderable = g_registry.try_get<Components::Renderable>(entity);
	if (!renderable)
		return false;

	auto id = g_registry.try_get<Components::Id>(camera);
	if (!id)
		return false;

	return !(
		0b1111 != renderable->flags ||
		(renderable->filter != 0 &&
		(renderable->filter & id->value))
		);
}

int GetRenderFlags (Entity entity)
{
	auto renderable = g_registry.try_get<Components::Renderable>(entity);
	ZEN_ASSERT(renderable, "The entity has no 'Renderable' component.");

	return renderable->flags;
}

}	// namespace Zen
