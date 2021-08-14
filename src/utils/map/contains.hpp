/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_UTILS_MAP_CONTAINS_HPP
#define ZEN_UTILS_MAP_CONTAINS_HPP

#include <map>

namespace Zen {

/**
 * Takes in a map and a key, and returns true if the key exists in the map.
 *
 * @since 0.0.0
 *
 * @param map A map.
 * @param key The key to check for.
 *
 * @return `true` if `key` exists in `map`. Otherwise `false`.
 *
 */
template <typename T, typename O>
bool Contains (std::map<T, O>& map, T key)
{
	return (map.find(key) != map.end());
}

#endif

}	// namespace Zen
