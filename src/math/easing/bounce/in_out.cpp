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
namespace Bounce {

double InOut (double value)
{
    bool reverse = false;

    if (value < 0.5)
    {
        value = 1 - (value * 2);
        reverse = true;
    }
    else
    {
        value = (value * 2) - 1;
    }

    if (value < (1 / 2.75))
    {
        value = 7.5625 * value * value;
    }
    else if (value < (2 / 2.75))
    {
		value -= 1.5 / 2.75;
        value = 7.5625 * value * value + 0.75;
    }
    else if (value < 2.5 / 2.75)
    {
		value -= 2.25 / 2.75;
        value = 7.5625 * value * value + 0.9375;
    }
    else
    {
		value -= 2.625 / 2.75;
        value = 7.5625 * value * value + 0.984375;
    }

    if (reverse)
    {
        return (1 - value) * 0.5;
    }
    else
    {
        return value * 0.5 + 0.5;
    }
}

}	// namespace Bounce
}	// namespace Easing
}	// namespace Math
}	// namespace Zen
