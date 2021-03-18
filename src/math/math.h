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
 * Snap a value to nearest grid slice, using floor.
 *
 * Example: if you have an interval gap of `5` and a position of `12`... you will snap to `10`.
 * As will `14` snap to `10`... but `16` will snap to `15`.
 *
 * @since 0.0.0
 *
 * @param value The value to snap.
 * @param gap The interval gap of the grid.
 * @param start Optional starting offset for gap.
 * @param divide If `true` it will divide the snapped value by the gap before returning.
 *
 * @return The snapped value.
 */
double snapFloor (double value, double gap, double start = 0, bool divide = false);

/**
 * Snap a value to nearest grid slice, using ceil.
 *
 * Example: if you have an interval gap of `5` and a position of `12`... you will snap to `15`.
 * As will `14` snap to `15`... but `16` will snap to `20`.
 *
 * @since 0.0.0
 *
 * @param value The value to snap.
 * @param gap The interval gap of the grid.
 * @param start Optional starting offset for gap.
 * @param divide If `true` it will divide the snapped value by the gap before returning.
 *
 * @return  The snapped value.
 */
double snapCeil (double value, double gap, double start = 0, bool divide = false);

/**
 * Snap a value to nearest grid slice, using rounding.
 *
 * Example: if you have an interval gap of `5` and a position of `12`... you will 
 * snap to `10` whereas `14` will snap to `15`.
 *
 * @since 0.0.0
 *
 * @param value The value to snap.
 * @param gap The interval gap of the grid.
 * @param start Optional starting offset for gap.
 * @param divide If `true` it will divide the snapped value by the gap before returning.
 *
 * @return  The snapped value.
 */
double snapTo (double value, double gap, double start = 0, bool divide = false);

/**
 * Check whether the given values are fuzzily equal.
 *
 * Two numbers are fuzzily equal if their difference is less than `epsilon`.
 *
 * @since 0.0.0
 *
 * @param a The first value.
 * @param b The second value.
 * @param epsilon The epsilon.
 *
 * @return `true` if the values are fuzzily equal, otherwise `false`.
 */
bool fuzzyEqual (double a, double b, double epsilon = EPSILON);

/**
 * Calculate the fuzzy ceiling of the given value.
 *
 * @since 0.0.0
 *
 * @param value The value.
 * @param epsilon The epsilon.
 *
 * @return The fuzzy ceiling of the value.
 */
bool fuzzyCeil (double value, double epsilon = EPSILON);

/**
 * Calculate the fuzzy floor of the given value.
 *
 * @since 0.0.0
 *
 * @param value The value.
 * @param epsilon The epsilon.
 *
 * @return The fuzzy floor of the value.
 */
bool fuzzyFloor (double value, double epsilon = EPSILON);

/**
 * Check whether `a` is fuzzily greater than `b`.
 *
 * `a` is fuzzily greater than `b` if it is more than `b - epsilon`.
 *
 * @since 0.0.0
 *
 * @param a The first value.
 * @param b The second value.
 * @param epsilon The epsilon.
 *
 * @return `true` if `a` is fuzzily greater than than `b`, otherwise `false`.
 */
bool fuzzyGreaterThan (double a, double b, double epsilon = EPSILON);

/**
 * Check whether `a` is fuzzily less than `b`.
 *
 * `a` is fuzzily less than `b` if it is less than `b + epsilon`.
 *
 * @since 0.0.0
 *
 * @param a The first value.
 * @param b The second value.
 * @param epsilon The epsilon.
 *
 * @return `true` if `a` is fuzzily less than `b`, otherwise `false`.
 */
bool fuzzyLessThan (double a, double b, double epsilon = EPSILON);

/**
 * Convert the given angle from degrees, to the equivalent angle in radians.
 *
 * @since 0.0.0
 *
 * @param degrees The angle (in degrees) to convert to radians.
 *
 * @return The given angle converted to radians.
 */
double DegToRad (double degrees);

}	// namespace Math
}	// namespace Zen

#endif
