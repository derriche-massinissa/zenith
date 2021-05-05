/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "stepped.hpp"
#include <cmath>

namespace Zen {
namespace Math {
namespace Easing {
namespace Stepped {

double Stepped (double value, unsigned int steps)
{
	if (value <= 0)
		return 0;
	else if (value >= 1)
		return 1;
	else
        return (std::trunc(steps * value) + 1) * (1. / steps);
}

}	// namespace Stepped
}	// namespace Easing
}	// namespace Math
}	// namespace Zen
