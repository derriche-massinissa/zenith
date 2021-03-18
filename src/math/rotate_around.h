/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_ROTATEAROUND_H
#define ZEN_MATH_ROTATEAROUND_H

#include "vector2.h"

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
Math::Vector2 rotateAround (Math::Vector2 point, int x, int y, double angle);

}	// namespace Math
}	// namespace Zen

#endif
