/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_WRAP_HPP
#define ZEN_MATH_WRAP_HPP

namespace Zen {
namespace Math {

/**
 * Wrap the given `value` between `min` and `max.
 *
 * @since 0.0.0
 *
 * @param value The value to wrap.
 * @param min The minimum value.
 * @param max The maximum value.
 *
 * @return {number} The wrapped value.
 */
double Wrap (double value, double min, double max);

}	// namespace Math
}	// namespace Zen

#endif
