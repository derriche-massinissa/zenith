/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_UTILS_SAFESUB_HPP
#define ZEN_UTILS_SAFESUB_HPP

namespace Zen {

/**
 * This function performs a safe substration operation for unsigned integers to
 * avoid value wrapping.
 *
 * @since 0.0.0
 */
template <typename T, typename O>
T SafeSub (T lhs, O rhs)
{
	if (lhs <= rhs)
		lhs = 0;
	else
		lhs -= rhs;

	return lhs;
}

}	// namespace Zen

#endif
