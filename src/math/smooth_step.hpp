/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_SMOOTHSTEP_HPP
#define ZEN_MATH_SMOOTHSTEP_HPP

namespace Zen {
namespace Math {

/**
 * Calculate a smooth interpolation percentage of `x` between `min` and `max`.
 *
 * The function receives the number `x` as an argument and returns 0 if `x` is less
 * than or equal to the left edge, 1 if `x` is greater than or equal to the right
 * edge, and smoothly interpolates, using a Hermite polynomial, between 0 and 1
 * otherwise.
 *
 * @since 0.0.0
 *
 * @param x The input value.
 * @param min The minimum value, also known as the 'left edge', assumed smaller
 * than the 'right edge'.
 * @param max The maximum value, also known as the 'right edge', assumed greater
 * than the 'left edge'.
 *
 * @return The percentage of interpolation, between 0 and 1.
 */
double SmoothStep (double x, double min, double max);

}	// namespace Math
}	// namespace Zen

#endif
