/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_EASING_LINEAR_LINEAR
#define ZEN_MATH_EASING_LINEAR_LINEAR

namespace Zen {
namespace Math {
namespace Easing {
namespace Linear {

/**
 * Linear easing (no variation).
 *
 * @since 0.0.0
 *
 * @param value The value to ease. Between 0 and 1.
 *
 * @return The eased value. Between 0 and 1.
 */
double Linear (double value);

}	// namespace Linear
}	// namespace Easing
}	// namespace Math
}	// namespace Zen

#endif
