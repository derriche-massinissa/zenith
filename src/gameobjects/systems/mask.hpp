/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_SYSTEMS_MASK_HPP
#define ZEN_GAMEOBJECTS_SYSTEMS_MASK_HPP

#include "../../ecs/entity.hpp"

namespace Zen {

Entity GetMask (Entity entity);

void SetMask (Entity entity, Entity maskEntity);

void ClearMask (Entity entity);

}	// namespace Zen

#endif
