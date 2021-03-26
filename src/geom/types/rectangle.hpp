/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GEOM_TYPES_RECTANGLE_HPP
#define ZEN_GEOM_TYPES_RECTANGLE_HPP

namespace Zen {

/**
 * Defines a Rectangle.
 *
 * @struct Rectangle
 * @since 0.0.0
 */
struct Rectangle
{
	double x = 0.,
		   y = 0.,
		   width = 0.,
		   height = 0.;
};

}	// namespace Zen

#endif
