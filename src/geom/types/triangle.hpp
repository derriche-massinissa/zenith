/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GEOM_TYPES_TRIANGLE_HPP
#define ZEN_GEOM_TYPES_TRIANGLE_HPP

#include "../shapes.hpp"

namespace Zen {

/**
 * Defines a Triangle.
 *
 * @struct Triangle
 * @since 0.0.0
 */
struct Triangle
{
	/**
	 * Used for active type detection for the Shape union;
	 *
	 * @since 0.0.0
	 */
	const SHAPE type = SHAPE::TRIANGLE;

	double x1 = 0.,
		   y1 = 0.,
		   x2 = 0.,
		   y2 = 0.,
		   x3 = 0.,
		   y3 = 0.;

	void operator = (const Triangle& other)
	{
		x1 = other.x1;
		y1 = other.y1;
		x2 = other.x2;
		y2 = other.y2;
		x3 = other.x3;
		y3 = other.y3;
	}
};

}	// namespace Zen

#endif
