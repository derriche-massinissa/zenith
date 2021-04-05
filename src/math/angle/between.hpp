/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_ANGLE_BETWEEN_HPP
#define ZEN_MATH_ANGLE_BETWEEN_HPP

namespace Zen {
namespace Math {

/**
 * Find the angle of a segment from (x1, y1) -> (x2, y2).
 *
 * @since 0.0.0
 *
 * @param x1 The x coordinate of the first point.
 * @param y1 The y coordinate of the first point.
 * @param x2 The x coordinate of the second point.
 * @param y2 The y coordinate of the second point.
 *
 * @return The angle in radians.
 */
double AngleBetween (double x1, double y1, double x2, double y2);

}	// namespace Math
}	// namespace Zen

#endif
