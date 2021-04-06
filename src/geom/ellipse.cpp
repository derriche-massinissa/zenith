/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "ellipse.hpp"

namespace Zen {

bool Contains (Ellipse ellipse, double x, double y)
{
	if (ellipse.width <= 0 || ellipse.height <= 0)
		return false;

	// Normalize the coords to an ellipse with center 0,0 and a radius of 0.5
	double normx = ((x - ellipse.x) / ellipse.width);
	double normy = ((y - ellipse.y) / ellipse.height);

	normx *= normx;
	normy *= normy;

	return (normx + normy) < 0.25;
}

bool EllipseContains (Shape shape, double x, double y)
{
	return Contains(shape.ellipse, x, y);
}

}	// namespace Zen
