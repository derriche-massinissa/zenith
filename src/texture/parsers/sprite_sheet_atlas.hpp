/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_PARSERS_SPRITESHEET_ATLAS_HPP
#define ZEN_TEXTURES_PARSERS_SPRITESHEET_ATLAS_HPP

#include "../sprite_sheet_config.hpp"

#include "../../ecs/entity.hpp"

namespace Zen {

void ParseSpriteSheetFromAtlas (Entity texture, Entity frame, SpriteSheetConfig config);

}	// namespace Zen

#endif
