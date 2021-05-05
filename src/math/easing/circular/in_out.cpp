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
namespace Circular {

double InOut (double value)
{
	value *= 2;

    if (value < 1)
    {
        return -0.5 * (std::sqrt(1 - value * value) - 1);
    }
    else
    {
		value -= 2;
        return 0.5 * (std::sqrt(1 - value * value) + 1);
    }
}

}	// namespace Circular
}	// namespace Easing
}	// namespace Math
}	// namespace Zen
