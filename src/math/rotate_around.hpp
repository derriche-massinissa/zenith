/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_ROTATEAROUND_HPP
#define ZEN_MATH_ROTATEAROUND_HPP

#include "types/vector2.hpp"

namespace Zen {
namespace Math {

/**
 * Rotate a `point` around `x` and `y` to the given `angle`, at the same distance.
 *
 * In polar notation, this maps a point from (r, t) to (r, angle), vs. the origin
 * (x, y).
 *
 * @since 0.0.0
 *
 * @param point The point to be rotated.
 * @param x The horizontal coordinate to rotate around.
 * @param y The vertical coordinate to rotate around.
 * @param angle The angle of rotation in radians.
 *
 * @return The given point.
 */
Math::Vector2 RotateAround (Math::Vector2 point, double x, double y, double angle);

}	// namespace Math
}	// namespace Zen

#endif
