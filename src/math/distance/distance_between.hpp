/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_DISTANCE_BETWEEN_HPP
#define ZEN_MATH_DISTANCE_BETWEEN_HPP

namespace Zen {
namespace Math {

/**
 * Calculate the distance between two sets of coordinates (points).
 *
 * @since 0.0.0
 *
 * @param x1 The x coordinate of the first point.
 * @param y1 The y coordinate of the first point.
 * @param x2 The x coordinate of the second point.
 * @param y2 The y coordinate of the second point.
 *
 * @return The distance between each point.
 */
double DistanceBetween (double x1, double y1, double x2, double y2);

}	// namespace Math
}	// namespace Zen

#endif
