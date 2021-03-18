/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_PARSERS_JSON_HASH_H
#define ZEN_TEXTURES_PARSERS_JSON_HASH_H

#include "nlohmann/json.hpp"

#include "../texture.fwd.h"

namespace Zen {
namespace Textures {

void parseJsonHash (Texture *texture, int sourceIndex, nlohmann::json json);

}	// namespace Textures
}	// namespace Zen

#endif
