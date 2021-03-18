/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_PARSERS_SPRITESHEET_H
#define ZEN_TEXTURES_PARSERS_SPRITESHEET_H

#include <cmath>
#include "../texture.h"
#include "../sprite_sheet_config.h"

namespace Zen {
namespace Textures {

void parserSpriteSheet (Texture *texture, int sourceIndex, int x, int y, int height, int width, SpriteSheetConfig config);

}	// namespace Textures
}	// namespace Zen

#endif
