/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GEOM_TYPES_CIRCLE_HPP
#define ZEN_GEOM_TYPES_CIRCLE_HPP

#include "../shapes.hpp"

namespace Zen {

/**
 * Defines a Circle.
 *
 * @struct Circle
 * @since 0.0.0
 */
struct Circle
{
	/**
	 * Used for active type detection for the Shape union;
	 *
	 * @since 0.0.0
	 */
	const SHAPE type = SHAPE::CIRCLE;

	double x = 0.,
		   y = 0.,
		   radius = 0.;

	Circle (double x = 0., double y = 0., double radius = 0.)
		: x (x), y (y), radius (radius)
	{}

	Circle (const Circle& other)
		: x (other.x)
		, y (other.y)
		, radius (other.radius)
	{}

	Circle (const Circle&& other)
		: x (other.x)
		, y (other.y)
		, radius (other.radius)
	{}

	void operator = (const Circle& other)
	{
		x = other.x;
		y = other.y;
		radius = other.radius;
	}
};

}	// namespace Zen

#endif
