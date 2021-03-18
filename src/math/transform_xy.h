/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_TRANSFORMXY_H
#define ZEN_MATH_TRANSFORMXY_H

#include "vector2.h"

namespace Zen {
namespace Math {

/**
 * Takes the `x` and `y` coordinates and transforms them into the same space as
 * defined by the position, rotation and scale values.
 *
 * @since 0.0.0
 *
 * @param x The x coordinate to be transformed.
 * @param y The y coordinate to be transformed.
 * @param positionX Horizontal position of the transform point.
 * @param positionY Vertical position of the transform point.
 * @param rotation Rotation of the transform point, in radians.
 * @param scaleX Horizontal scale of the transform point.
 * @param scaleY Vertical scale of the transform point.
 *
 * @return The translated point.
 */
Math::Vector2 transformXY (int x, int y, int positionX, int positionY, double rotation, double scaleX, double scaleY);

}	// namespace Math
}	// namespace Zen

#endif
