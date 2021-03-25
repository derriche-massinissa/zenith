/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_FUZZY_EQUAL_H
#define ZEN_MATH_FUZZY_EQUAL_H

#include "../const.h"

namespace Zen {
namespace Math {

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
bool FuzzyEqual (double a, double b, double epsilon = EPSILON);

}	// namespace Math
}	// namespace Zen

#endif
