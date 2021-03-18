/**
 * @file		math.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef MATH_H
#define MATH_H

#include <algorithm>
#include <cmath>

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
int clamp (int value, int min, int max);

/**
 * @overload
 *
 * @since 0.0.0
 * @param value The value to clamp
 * @param min The minimum bound.
 * @param max The maximum bound.
 * @return The clamped value.
 */
double clamp (double value, double min, double max);

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
int linear (int p0, int p1, double t);

/**
 * @overload
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

/**
 * @since 0.0.0
 */
const double PI2 = M_PI * 2.0;
/**
 * @since 0.0.0
 */
const double TAU = M_PI * 0.5;
/**
 * @since 0.0.0
 */
const double EPSILON = 0.000001;
/**
 * @since 0.0.0
 */
const double DEG_TO_RAD = M_PI / 180.0;
/**
 * @since 0.0.0
 */
const double RAD_TO_DEG = 180.0 / M_PI;

}	// namespace Math
}	// namespace Zen

#endif
