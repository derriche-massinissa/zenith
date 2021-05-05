/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TWEENS_EASING_HPP
#define ZEN_TWEENS_EASING_HPP

#include <functional>
#include "tween/const.hpp"

namespace Zen {

std::function<double(double)> GetEaseFunction(TWEEN ease);

}	// namespace Zen

#endif
