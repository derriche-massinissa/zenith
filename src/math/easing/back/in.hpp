/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_EASING_BACK_IN
#define ZEN_MATH_EASING_BACK_IN

namespace Zen {
namespace Math {
namespace Easing {
namespace Back {

/**
 * Back ease-in.
 *
 * @since 0.0.0
 *
 * @param value The value to be tweened.
 * @param overshoot The overshoot amount.
 *
 * @return The tweened value.
 */
double In (double value, double overshoot = 1.70158);

}	// namespace Back
}	// namespace Easing
}	// namespace Math
}	// namespace Zen

#endif
