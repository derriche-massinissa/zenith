/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_UTILS_MAP_EMPLACE_HPP
#define ZEN_UTILS_MAP_EMPLACE_HPP

#include <map>

namespace Zen {

/**
 * Takes in a map, a key and one or more values, and emplaces a new item in the
 * map at the given key with the given values passed to it (or its constructor).
 *
 * @since 0.0.0
 *
 * @param map A pointer to a map to emplace a new item in.
 * @param key The key to emplace a new item at.
 * @param args The arguments to use to construct the new item.
 *
 * @return A pointer to the emplaced item, or null if it couldn't be added.
 */
template <typename T, typename O, typename...Args>
O* Emplace (std::map<T, O>* map, T key, Args&&...args)
{
	auto o = map->emplace(
		std::piecewise_construct,
		std::forward_as_tuple(key),
		std::forward_as_tuple(std::forward<Args...>(args)...)
	);
	
	if (o.second)
		return nullptr;
	else
		return &o.first->second;
}

}	// namespace Zen

#endif
