/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_DEPTH_HPP
#define ZEN_SYSTEMS_DEPTH_HPP

#include "../ecs/entity.hpp"

namespace Zen {

int GetDepth (Entity entity);

void SetDepth (Entity entity, int value = 0);

}	// namespace Zen

#endif
