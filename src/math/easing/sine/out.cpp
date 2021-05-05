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
namespace Sine {

double Out (double value)
{
	if (value == 0)
		return 0;
	else if (value == 1)
		return 1;
	else
		return std::cos(value * M_PI / 2);
}

}	// namespace Sine
}	// namespace Easing
}	// namespace Math
}	// namespace Zen
