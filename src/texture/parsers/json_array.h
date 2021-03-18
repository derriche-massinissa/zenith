/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_PARSERS_JSON_ARRAY_H
#define ZEN_TEXTURES_PARSERS_JSON_ARRAY_H

#include "../texture.h"
#include "nlohmann/json.hpp"

namespace Zen {
namespace Textures {

void parseJsonArray (Texture *texture, int sourceIndex, nlohmann::json json);

}	// namespace Textures
}	// namespace Zen

#endif
