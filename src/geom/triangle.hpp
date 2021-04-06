/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GEOM_TRIANGLE_HPP
#define ZEN_GEOM_TRIANGLE_HPP

#include "types/triangle.hpp"
#include "types/shape.hpp"

namespace Zen {

bool Contains (Triangle triangle, double x, double y);

bool TriangleContains (Shape shape, double x, double y);

}	// namespace Zen

#endif
