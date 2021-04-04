/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_UTILS_VECTOR_INDEXOF_HPP
#define ZEN_UTILS_VECTOR_INDEXOF_HPP

#include <vector>
#include <algorithm>

namespace Zen {

/**
 * Takes in a vector and an item, and returns the index of the item inside the
 * vector, or the first instance of it, or -1 if the item isn't present in the
 * vector.
 *
 * @since 0.0.0
 *
 * @param vector A vector.
 * @param item A possible item of `vector`.
 *
 * @return The index of item inside `vector` or -1 if not contained in it.
 */
template <typename T>
int IndexOf (std::vector<T>& vector, T item)
{
	std::vector<T>::iterator itr = std::find(vector.begin(), vector.end(), item);

	if (itr == vector.end())
		return -1;

	return std::distance(vector.begin(), itr);
}

/**
 * @overload
 * @since 0.0.0
 *
 * @param vector A vector.
 * @param item A possible item of `vector`.
 *
 * @return The index of item inside `vector` or -1 if not contained in it.
 */
template <typename T>
int IndexOf (std::vector<T>&& vector, T item)
{
	return IndexOf(vector, item);
}

}	// namespace Zen

#endif
