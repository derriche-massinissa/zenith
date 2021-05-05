/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "in.hpp"

namespace Zen {
namespace Math {
namespace Easing {
namespace Bounce {

double In (double value)
{
	value = 1 - value;

    if (value < (1 / 2.75))
    {
        return 1 - (7.5625 * value * value);
    }
    else if (value < (2 / 2.75))
    {
		value -= 1.5 / 2.75;
        return 1 - (7.5625 * value * value + 0.75);
    }
    else if (value < (2.5 / 2.75))
    {
		value -= 2.25 / 2.75;
        return 1 - (7.5625 * value * value + 0.9375);
    }
    else
    {
		value -= 2.625 / 2.75;
        return 1 - (7.5625 * value * value + 0.984375);
    }
}

}	// namespace Bounce
}	// namespace Easing
}	// namespace Math
}	// namespace Zen
