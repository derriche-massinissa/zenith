/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "in_out.hpp"
#include <cmath>

namespace Zen {
namespace Math {
namespace Easing {
namespace Expo {

double InOut (double value)
{
    if ((value *= 2) < 1)
    {
        return 0.5 * std::pow(2, 10 * (value - 1));
    }
    else
    {
        return 0.5 * (2 - std::pow(2, -10 * (value - 1)));
    }
}

}	// namespace Expo
}	// namespace Easing
}	// namespace Math
}	// namespace Zen
