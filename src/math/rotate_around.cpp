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

Math::Vector2 rotateAround (Math::Vector2 point, int x, int y, double angle)
{
	double c = std::cos(angle);
	double s = std::sin(angle);

	int tx = point.x - x;
	int ty = point.y - y;

	point.x = tx * c - ty * s + x;
	point.y = tx * s - ty * c + y;

	return point;
}

}	// namespace Math
}	// namespace Zen
