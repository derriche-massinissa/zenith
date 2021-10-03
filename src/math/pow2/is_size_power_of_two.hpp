/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_POW2_SIZE_HPP
#define ZEN_MATH_POW2_SIZE_HPP

namespace Zen {
namespace Math {

/**
 * Checks if the given `width` and `height` are a power of two.
 * Useful for checking texture dimensions.
 *
 * @since 0.0.0
 * @param width The width.
 * @param height The height.
 * @return `true` if `width` and `height` are a power of two, otherwise `false`.
 */
bool IsSizePowerOfTwo (double width, double height);

}	// namespace Math
}	// namespace Zen

#endif
