/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

//#include "../text.hpp"

#include <vector>
#include "../../utils/assert.hpp"
#include "../../components/actor.hpp"
#include "../../components/update.hpp"

namespace Zen {

extern entt::registry g_registry;

void SetScene (Entity entity, Entity scene)
{
	auto [actor, update] = g_registry.try_get<Components::Actor, Components::Update<Components::Actor>>(entity);
	ZEN_ASSERT(actor, "The entity has no 'Actor' component.");

	actor->scene = scene;

	if (update)
		update->update(entity);
}

void f (std::string& text, std::vector<int>& characters)
{
	characters.clear();

	int cplen = 1;
	for (size_t i = 0; i < text.length();)
	{
		cplen = 1;
		if ((text[i] & 0x78) == 0xf0)
			cplen = 4;
		else if ((text[i] & 0xf0) == 0xe0)
			cplen = 3;
		else if ((text[i] & 0xe0) == 0xc0)
			cplen = 2;
	}
}

}	// namespace Zen
