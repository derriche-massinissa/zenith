/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_BLENDMODE_HPP
#define ZEN_SYSTEMS_BLENDMODE_HPP

#include "../ecs/entity.hpp"
#include "../enums/blend_modes.hpp"

namespace Zen {

int GetBlendMode (Entity entity);

void SetBlendMode (Entity entity, int value);

void SetBlendMode (Entity entity, BLEND_MODE value);

}	// namespace Zen

#endif
