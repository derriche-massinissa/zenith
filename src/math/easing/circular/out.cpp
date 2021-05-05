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
namespace Circular {

double Out (double value)
{
	value--;
	return std::sqrt(1 - value * value);
}

}	// namespace Circular
}	// namespace Easing
}	// namespace Math
}	// namespace Zen
