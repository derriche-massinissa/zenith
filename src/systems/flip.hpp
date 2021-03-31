/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_FLIP_HPP
#define ZEN_SYSTEMS_FLIP_HPP

#include "../ecs/entity.hpp"

namespace Zen {

void ToggleFlipX (Entity entity);

void ToggleFlipY (Entity entity);

void SetFlipX (Entity entity, bool value = true);

void SetFlipY (Entity entity, bool value = true);

void SetFlip (Entity entity, bool x, bool y);

void ResetFlip (Entity entity);

bool GetFlipX (Entity entity);

bool GetFlipY (Entity entity);

}	// namespace Zen

#endif
