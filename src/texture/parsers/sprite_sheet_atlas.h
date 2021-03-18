/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_PARSERS_SPRITESHEET_ATLAS_H
#define ZEN_TEXTURES_PARSERS_SPRITESHEET_ATLAS_H

#include <cmath>

#include "../sprite_sheet_config.h"

#include "../texture.fwd.h"
#include "../frame.fwd.h"

namespace Zen {
namespace Textures {

void parseSpriteSheetFromAtlas (Texture *texture, Frame *frame, SpriteSheetConfig config);

}	// namespace Textures
}	// namespace Zen

#endif
