/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "rotate_around.h"
#include <cmath>

namespace Zen {
namespace Math {

Math::Vector2 RotateAround (Math::Vector2 point, double x, double y, double angle)
{
	double c = std::cos(angle);
	double s = std::sin(angle);

	double tx = point.x - x;
	double ty = point.y - y;

	point.x = tx * c - ty * s + x;
	point.y = tx * s - ty * c + y;

	return point;
}

}	// namespace Math
}	// namespace Zen
