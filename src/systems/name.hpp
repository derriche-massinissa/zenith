/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_NAME_HPP
#define ZEN_SYSTEMS_NAME_HPP

#include "../ecs/entity.hpp"

namespace Zen {

void SetName (Entity entity, const char* name);

const char* GetName (Entity entity);

}	// namespace Zen

#endif
