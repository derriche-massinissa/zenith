/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_COMPONENTS_BLENDMODE_HPP
#define ZEN_COMPONENTS_BLENDMODE_HPP

#include "../enums/blend_modes.hpp"

namespace Zen {
namespace Components {

struct BlendMode
{
	BLEND_MODE value = BLEND_MODE::BLEND;
};

}	// namespace Components
}	// namespace Zen

#endif
