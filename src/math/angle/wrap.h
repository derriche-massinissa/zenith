/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_ANGLE_WRAPDEGREES_H
#define ZEN_MATH_ANGLE_WRAPDEGREES_H

namespace Zen {
namespace Math {
namespace Angle {

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
double wrap (double angle);

}	// namespace Angle
}	// namespace Math
}	// namespace Zen

#endif
