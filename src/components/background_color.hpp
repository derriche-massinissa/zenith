/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_COMPONENTS_COLOR_HPP
#define ZEN_COMPONENTS_COLOR_HPP

#include "../ecs/entity.hpp"
#include "../display/types/color.hpp"

namespace Zen {
namespace Components {

struct BackgroundColor
{
	Color color;
};

}	// namespace Components
}	// namespace Zen

#endif
