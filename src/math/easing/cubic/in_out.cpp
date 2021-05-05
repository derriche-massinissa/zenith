/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "in_out.hpp"

namespace Zen {
namespace Math {
namespace Easing {
namespace Cubic {

double InOut (double value)
{
	value *= 2;
    if (value < 1)
    {
        return 0.5 * value * value * value;
    }
    else
    {
		value -= 2;
        return 0.5 * (value * value * value + 2);
    }
}

}	// namespace Cubic
}	// namespace Easing
}	// namespace Math
}	// namespace Zen
