/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "circle.hpp"

#include "../math/distance/distance_between.hpp"

namespace Zen {

bool Contains (Circle circle, double x, double y)
{
	if (circle.radius <= 0)
		return false;

	return Math::DistanceBetween(circle.x, circle.y, x, y) <= circle.radius;
}

bool CircleContains (Shape shape, double x, double y)
{
	return Contains(shape.circle, x, y);
}

}	// namespace Zen
