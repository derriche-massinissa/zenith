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

namespace Zen {

/**
 *
 * @struct Shape
 * @since 0.0.0
 */
union Shape
{
	Rectangle rectangle;

	//Circle circle;

	//Ellipse ellipse;

	//Triangle triangle;

	Line line;

	//Point point;

	//Polygon polygon
};

}	// namespace Zen

#endif
