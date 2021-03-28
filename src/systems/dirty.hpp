/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_DIRTY_HPP
#define ZEN_SYSTEMS_DIRTY_HPP

#include "../ecs/entity.hpp"

namespace Zen {

bool IsDirty (Entity entity);

void SetDirty (Entity entity, bool value);

}	// namespace Zen

#endif
