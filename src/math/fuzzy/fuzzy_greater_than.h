/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_FUZZY_GREATER_THAN_H
#define ZEN_MATH_FUZZY_GREATER_THAN_H

#include "../const.h"

namespace Zen {
namespace Math {

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
bool FuzzyGreaterThan (double a, double b, double epsilon = EPSILON);

}	// namespace Math
}	// namespace Zen

#endif
