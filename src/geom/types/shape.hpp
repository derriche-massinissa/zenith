/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GEOM_TYPES_SHAPE_HPP
#define ZEN_GEOM_TYPES_SHAPE_HPP

#include "rectangle.hpp"
#include "line.hpp"

// TODO initialize like this:
// Shape myShape { .ellipse = {.x = 0} };
// `.shape.type` will be initialized by the activated structure thanks to the
// union's Common Initial Sequence

namespace Zen {

/**
 *
 * @struct Shape
 * @since 0.0.0
 */
union Shape
{
	struct {
		SHAPE type;
	} shape;

	Rectangle rectangle;

	Circle circle;

	Ellipse ellipse;

	Triangle triangle;

	Line line;

	//Point point;

	//Polygon polygon
};

}	// namespace Zen

#endif
