/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_CLAMP_H
#define ZEN_MATH_CLAMP_H

namespace Zen {
namespace Math {

/**
 * Force a value within the boundaries by clamping it to the range
 * `min` and `max`.
 *
 * @since 0.0.0
 * @param value The value to clamp
 * @param min The minimum bound.
 * @param max The maximum bound.
 * @return The clamped value.
 */
double clamp (double value, double min, double max);

}	// namespace Math
}	// namespace Zen

#endif
