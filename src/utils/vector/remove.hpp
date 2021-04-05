/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_UTILS_VECTOR_REMOVE_HPP
#define ZEN_UTILS_VECTOR_REMOVE_HPP

#include <vector>
#include <algorithm>

namespace Zen {

/**
 * Takes in a vector and an item, and if the item is in the vector, removes it.
 *
 * @since 0.0.0
 *
 * @param vector A vector.
 * @param item A possible item of `vector`.
 */
template <typename T>
void Remove (std::vector<T>& vector, T item)
{
	vector.erase(
		std::remove(vector.begin(), vector.end(), item),
		vector.end()
	);
}

/**
 * @overload
 * @since 0.0.0
 *
 * @param vector A vector.
 * @param item A possible item of `vector`.
 */
template <typename T>
bool Remove (std::vector<T>&& vector, T item)
{
	return Remove(vector, item);
}

}	// namespace Zen

#endif
