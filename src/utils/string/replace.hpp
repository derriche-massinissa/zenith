/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_UTILS_STRING_REPLACE_HPP
#define ZEN_UTILS_STRING_REPLACE_HPP

#include <string>

namespace Zen {

/**
 * Takes in three strings `s`, `a` and `b`. Searches `s` for any instance of
 * `a` and replaces it with `b`.
 *
 * @since 0.0.0
 *
 * @param s A string.
 * @param a Substring to find and replace.
 * @param b String to replace `a` with.
 *
 * @return The modified `s` string.
 *
 */
std::string Replace (std::string s, std::string a, std::string b);

#endif

}	// namespace Zen
