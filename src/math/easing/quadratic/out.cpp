/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "out.hpp"

namespace Zen {
namespace Math {
namespace Easing {
namespace Quadratic {

double Out (double value)
{
	return value * (2 - value);
}

}	// namespace Quadratic
}	// namespace Easing
}	// namespace Math
}	// namespace Zen
