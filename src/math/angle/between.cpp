/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "between.hpp"

#include <cmath>

namespace Zen {
namespace Math {

double AngleBetween (double x1, double y1, double x2, double y2)
{
	return std::atan2(y2 - y1, x2 - x1);
}

}	// namespace Math
}	// namespace Zen
