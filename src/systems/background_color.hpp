/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_BACKGROUNDCOLOR_HPP
#define ZEN_SYSTEMS_BACKGROUNDCOLOR_HPP

#include "../ecs/entity.hpp"
#include "../display/types/color.hpp"

namespace Zen {

void SetBackgroundColor (Entity entity, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

void SetBackgroundColor (Entity entity, uint32_t color);

void SetBackgroundColor (Entity entity, Color color);

}	// namespace Zen

#endif
