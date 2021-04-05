/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "distance_between.hpp"
#include <cmath>

namespace Zen {
namespace Math {

double DistanceBetween (double x1, double y1, double x2, double y2)
{
	double dx = x1 - x2;
	double dy = y1 - y2;

	return std::sqrt(dx * dx + dy * dy);
}

}	// namespace Math
}	// namespace Zen
