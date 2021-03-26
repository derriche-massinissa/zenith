/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_ANGLE_WRAPRADIANS_HPP
#define ZEN_MATH_ANGLE_WRAPRADIANS_HPP

namespace Zen {
namespace Math {

/**
 * Wrap an angle.
 *
 * Wraps the angle to a value in the range of -PI to PI.
 *
 * @since 0.0.0
 *
 * @param angle The angle to wrap, in radians.
 *
 * @return The wrapped angle, in radians.
 */
double WrapRadians (double radians);

}	// namespace Math
}	// namespace Zen

#endif
