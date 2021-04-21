/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_PARSERS_SPRITESHEET_HPP
#define ZEN_TEXTURES_PARSERS_SPRITESHEET_HPP

#include "../../ecs/entity.hpp"
#include "../sprite_sheet_config.hpp"

namespace Zen {

int ParseSpriteSheet (Entity texture, int sourceIndex, int x, int y, int width, int height, SpriteSheetConfig config);

}	// namespace Zen

#endif
