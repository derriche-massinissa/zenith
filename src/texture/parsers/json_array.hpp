/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_PARSERS_JSONARRAY_HPP
#define ZEN_TEXTURES_PARSERS_JSONARRAY_HPP

#include "json/json.hpp"
#include "../../ecs/entity.hpp"

namespace Zen {

int ParseJsonArray (Entity texture, int sourceIndex, nlohmann::json json);

}	// namespace Zen

#endif
