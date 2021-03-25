/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_SYSTEMS_SOURCE_HPP
#define ZEN_TEXTURES_SYSTEMS_SOURCE_HPP

#include <string>
#include "../../ecs/entity.hpp"

namespace Zen {

Entity CreateTextureSource (Entity texture, std::string src, int index);

void DestroyTextureSource (Entity source);

}	// namespace Zen

#endif
