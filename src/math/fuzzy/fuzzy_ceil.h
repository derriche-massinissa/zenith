/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_FUZZY_CEIL_H
#define ZEN_MATH_FUZZY_CEIL_H

#include "../const.h"

namespace Zen {
namespace Math {

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
bool FuzzyCeil (double value, double epsilon = EPSILON);

}	// namespace Math
}	// namespace Zen

#endif
