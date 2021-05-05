/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "in.hpp"
#include <cmath>

namespace Zen {
namespace Math {
namespace Easing {
namespace Expo {

double In (double value)
{
    return std::pow(2, 10 * (value - 1)) - 0.001;
}

}	// namespace Expo
}	// namespace Easing
}	// namespace Math
}	// namespace Zen
