/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "out.hpp"
#include <cmath>

namespace Zen {
namespace Math {
namespace Easing {
namespace Expo {

double Out (double value)
{
    return 1 - std::pow(2, -10 * value);
}

}	// namespace Expo
}	// namespace Easing
}	// namespace Math
}	// namespace Zen
