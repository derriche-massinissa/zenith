/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_EASING_STEPPED_STEPPED_HPP
#define ZEN_MATH_EASING_STEPPED_STEPPED_HPP

namespace Zen {
namespace Math {
namespace Easing {
namespace Stepped {

/**
 * Stepped easing.
 *
 * @since 0.0.0
 *
 * @param value The value to be eased.
 * @param steps The number of steps in the ease.
 *
 * @return The eased value.
 */
double Stepped (double value, unsigned int steps = 1);

}	// namespace Stepped
}	// namespace Easing
}	// namespace Math
}	// namespace Zen

#endif
