/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../actor.hpp"

#include "../../utils/assert.hpp"
#include "../../components/actor.hpp"
#include "../../components/update.hpp"

namespace Zen {

extern entt::registry g_registry;

void SetScene (Entity entity, Scene *scene)
{
	auto [actor, update] = g_registry.try_get<Components::Actor, Components::Update<Components::Actor>>(entity);
	ZEN_ASSERT(actor, "The entity has no 'Actor' component.");

	actor->scene = scene;

	if (update)
		update->update(entity);
}

Scene* GetScene (Entity entity)
{
	auto actor = g_registry.try_get<Components::Actor>(entity);
	ZEN_ASSERT(actor, "The entity has no 'Actor' component.");

	return actor->scene;
}

}	// namespace Zen
