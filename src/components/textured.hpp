/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_COMPONENT_TEXTURED_HPP
#define ZEN_COMPONENT_TEXTURED_HPP

#include "../ecs/entity.hpp"

namespace Zen {
namespace Components {

struct Textured
{
	Entity texture = entt::null;

	Entity frame = entt::null;

	bool isCropped = false;
};

}	// namespace Components
}	// namespace Zen

#endif
