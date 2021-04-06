/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GEOM_ELLIPSE_HPP
#define ZEN_GEOM_ELLIPSE_HPP

#include "types/ellipse.hpp"
#include "types/shape.hpp"

namespace Zen {

bool Contains (Ellipse ellipse, double x, double y);

bool EllipseContains (Shape shape, double x, double y);

}	// namespace Zen

#endif
