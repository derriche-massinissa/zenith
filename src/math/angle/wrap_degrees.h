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

/**
 * Wrap an angle in degrees.
 *
 * Wraps the angle to a value in the range of -180 to 180.
 *
 * @since 0.0.0
 *
 * @param angle The angle to wrap, in degrees.
 *
 * @return The wrapped angle, in degrees.
 */
double WrapDegrees (double angle);

}	// namespace Math
}	// namespace Zen

#endif
