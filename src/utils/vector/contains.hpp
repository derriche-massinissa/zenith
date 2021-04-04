/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_UTILS_VECTOR_CONTAINS_HPP
#define ZEN_UTILS_VECTOR_CONTAINS_HPP

#include <vector>
#include <algorithm>

namespace Zen {

/**
 * Takes in a vector and an item, and returns true if the item is cotained
 * inside the vector.
 *
 * @since 0.0.0
 *
 * @param vector A vector.
 * @param item A possible item of `vector`.
 *
 * @return `true` if `item` exists in `vector`. Otherwise `false`.
 */
template <typename T>
bool Contains (std::vector<T>& vector, T item)
{
	return std::find(vector.begin(), vector.end(), item) != vector.end();
}

/**
 * @overload
 * @since 0.0.0
 *
 * @param vector A vector.
 * @param item A possible item of `vector`.
 *
 * @return `true` if `item` exists in `vector`. Otherwise `false`.
 */
template <typename T>
bool Contains (std::vector<T>&& vector, T item)
{
	return Contains(vector, item);
}

}	// namespace Zen

#endif
