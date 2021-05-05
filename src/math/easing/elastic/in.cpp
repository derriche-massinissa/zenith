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
namespace Elastic {

double In (double value, double amplitude, double period)
{
    if (value == 0)
    {
        return 0;
    }
    else if (value == 1)
    {
        return 1;
    }
    else
    {
        double s = period / 4;

        if (amplitude < 1)
            amplitude = 1;
        else
            s = period * std::asin(1 / amplitude) / (2 * M_PI);

		value--;

        return -(amplitude * std::pow(2, 10 * value) * std::sin((value - s)
					* (2 * M_PI) / period));
    }
}

}	// namespace Elastic
}	// namespace Easing
}	// namespace Math
}	// namespace Zen
