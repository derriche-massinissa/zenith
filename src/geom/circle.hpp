/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GEOM_CIRCLE_HPP
#define ZEN_GEOM_CIRCLE_HPP

#include "types/circle.hpp"
#include "types/shape.hpp"

namespace Zen {

bool Contains (Circle circle, double x, double y);

bool CircleContains (Shape shape, double x, double y);

}	// namespace Zen

#endif
