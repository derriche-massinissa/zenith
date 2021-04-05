/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_INTERPOLATION_SMOOTHSTEP_HPP
#define ZEN_MATH_INTERPOLATION_SMOOTHSTEP_HPP

namespace Zen {
namespace Math {

/**
 * A Smooth Step interpolation method.
 *
 * @since 0.0.0
 *
 * @param t - The percentage of interpolation, between 0 and 1.
 * @param min - The minimum value, also known as the 'left edge', assumed smaller
 * than the 'right edge'.
 * @param max - The maximum value, also known as the 'right edge', assumed greater
 * than the 'left edge'.
 *
 * @return The interpolated value.
 */
double SmoothStepInterpolation (double t, double min, double max);

}	// namespace Math
}	// namespace Zen

#endif
