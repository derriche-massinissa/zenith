/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "point.h"

namespace Zen {
namespace Geom {

Point::Point (int x_)
	: x(x_), y(x_)
{}

Point::Point (int x_, int y_)
	: x(x_), y(y_)
{}

Point& Point::setTo (int x_)
{
	x = x_;
	y = x_;

	return *this;
}

Point& Point::setTo (int x_, int y_)
{
	x = x_;
	y = y_;

	return *this;
}

}	// namespace Geom
}	// namespace Zen
