/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_LINEAR_H
#define ZEN_MATH_LINEAR_H

namespace Zen {
namespace Math {

/**
 * Calculates a linear (interpolation) value over t.
 *
 * @since 0.0.0
 *
 * @param p0 The first point.
 * @param p1 The second point.
 * @param t The percentage between p0 and p1 to return, represented as a number between 0 and 1.
 *
 * @return The step t% of the way between p0 and p1.
 */
double linear (double p0, double p1, double t);

}	// namespace Math
}	// namespace Zen

#endif
