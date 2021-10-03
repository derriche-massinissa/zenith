/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../type.hpp"
#include "../../components/type.hpp"
#include "../../utils/assert.hpp"

namespace Zen {

extern entt::registry g_registry;

std::string GetType (Entity entity)
{
	auto blendMode = g_registry.try_get<Components::Type>(entity);
	ZEN_ASSERT(blendMode, "The entity has no 'Type' component.");

	return blendMode->type;
}

}	// namespace Zen
