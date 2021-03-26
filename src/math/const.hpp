/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_CONST_HPP
#define ZEN_MATH_CONST_HPP

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

}	// namespace Math
}	// namespace Zen

#endif
