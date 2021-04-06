/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GEOM_TYPES_ELLIPSE_HPP
#define ZEN_GEOM_TYPES_ELLIPSE_HPP

#include "../shapes.hpp"

namespace Zen {

/**
 * Defines a Ellipse.
 *
 * @struct Ellipse
 * @since 0.0.0
 */
struct Ellipse
{
	/**
	 * Used for active type detection for the Shape union;
	 *
	 * @since 0.0.0
	 */
	const SHAPE type = SHAPE::ELLIPSE;

	double x = 0.,
		   y = 0.,
		   width = 0.,
		   height = 0.;

	Ellipse (double x = 0., double y = 0., double width = 0., double height = 0.)
		: x (x), y (y), width (width), height (height)
	{}

	Ellipse (const Ellipse& other)
		: x (other.x)
		, y (other.y)
		, width (other.width)
		, height (other.height)
	{}

	Ellipse (const Ellipse&& other)
		: x (other.x)
		, y (other.y)
		, width (other.width)
		, height (other.height)
	{}

	void operator = (const Ellipse& other)
	{
		x = other.x;
		y = other.y;
		width = other.width;
		height = other.height;
	}
};

}	// namespace Zen

#endif
