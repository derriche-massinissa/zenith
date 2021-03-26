/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GEOM_TYPES_LINE_HPP
#define ZEN_GEOM_TYPES_LINE_HPP

namespace Zen {

/**
 * Defines a Line segment, a part of a line between two endpoints.
 *
 * @struct Line
 * @since 0.0.0
 */
struct Line
{
	double x1 = 0.,
		   y1 = 0.,
		   x2 = 0.,
		   y2 = 0.;
};

}	// namespace Zen

#endif
