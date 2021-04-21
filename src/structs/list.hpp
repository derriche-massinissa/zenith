/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_STRUCTS_LIST_HPP
#define ZEN_STRUCTS_LIST_HPP

#include <vector>
#include <functional>

namespace Zen {

template <typename T>
class List
{
public:
	/**
	 * The items that belong to this collection.
	 *
	 * @since 0.0.0
	 */
	std::vector<T> list;

	/**
	 * A callback that is invoked every time an item is added to this list.
	 *
	 * @since 0.0.0
	 */
	std::function<void(T)> addCallback = nullptr;

	/**
	 * A callback that is invoked every time an item is removed from this list.
	 *
	 * @since 0.0.0
	 */
	std::function<void(T)> removeCallback = nullptr;

	/**
	 * A function used to sort items.
	 *
	 * @since 0.0.0
	 */
	std::function<bool(T, T)> sortCallback = nullptr;

	/**
	 * Adds the given item to the end of this vector. Each item is unique.
	 *
	 * @since 0.0.0
	 *
	 * @param item The item to add.
	 * @param skipCallback Skip calling the List::addCallback if the item is added
	 * successfully.
	 */
	void add (T item, bool skipCallback = false);

	/**
	 * Adds an item to the vector at the specified index. Each item is unique.
	 *
	 * @since 0.0.0
	 *
	 * @param item The item to add.
	 * @param index The index in the vector in which the item will be added.
	 * @param skipCallback Skip calling the List::addCallback if the item is added
	 * successfully.
	 */
	void addAt (T item, std::size_t index, bool skipCallback = false);

	/**
	 * Retrieves the item at the given index.
	 *
	 * @since 0.0.0
	 *
	 * @param index The index in the vector in which the item will be added.
	 */
	T getAt (std::size_t index);

	/**
	 * Locates an item within the List and returns its index.
	 *
	 * @since 0.0.0
	 *
	 * @param item The item to locate.
	 */
	std::size_t getIndex (T item);

	/**
	 * Sort the contents of this List so the items are in order based using the
	 * given handler function, or List::sortCallback if previously given.
	 *
	 * @since 0.0.0
	 *
	 * @param handler The function to use to sort the items of this List.
	 */
	void sort (std::function<bool(T, T)> handler = nullptr);

	/**
	 * Returns a random item from this List.
	 *
	 * @since 0.0.0
	 *
	 * @param start The offset from the front of the List.
	 * @param length Restriction on the number of values to select from. 0 for
	 * "until the end".
	 *
	 * @return A random item of this List.
	 */
	T getRandom (std::size_t start = 0, std::size_t length = 0);

	/**
	 * Swaps the positions of two items in the List, at the given positions.
	 *
	 * @since 0.0.0
	 *
	 * @param item1 Index of the first item to swap.
	 * @param item2 Index of the second item to swap.
	 */
	void swap (std::size_t index1, std::size_t index2);

	/**
	 * Swaps the positions of two items in the List.
	 *
	 * @since 0.0.0
	 *
	 * @param item1 The first item to swap.
	 * @param item2 The second item to swap.
	 */
	void swap (T item1, T item2);

	/**
	 * Moves an item to the given index.
	 *
	 * @since 0.0.0
	 *
	 * @param item The item to move
	 * @param index The position to move the item to.
	 */
	void moveTo (T item, std::size_t index);

	/**
	 * Removes the given item from this List.
	 *
	 * @param item The item to remove.
	 * @param skipCallback Skip calling List::removeCallback.
	 */
	void remove (T item, bool skipCallback = false);

	/**
	 * Removes the given items from this List.
	 *
	 * @param items The items to remove.
	 * @param skipCallback Skip calling List::removeCallback.
	 */
	void remove (std::vector<T> items, bool skipCallback = false);

	/**
	 * Removes the given item from this List.
	 *
	 * @param index The index of the item to remove.
	 * @param skipCallback Skip calling List::removeCallback.
	 */
	void removeAt (std::size_t index, bool skipCallback = false);

	/**
	 * Removes the given items within the given range from this List.
	 *
	 * @param start The index to start removing from.
	 * @param end The index to stop removing from. The item at this position won't
	 * be removed.
	 * @param skipCallback Skip calling List::removeCallback.
	 */
	void removeBetween (std::size_t start, std::size_t end, bool skipCallback = false);

	/**
	 * Removes all the items from this List.
	 *
	 * @since 0.0.0
	 */
	void removeAll (bool skipCallback = false);

	/**
	 * Brings the given item to the top of this List.
	 *
	 * @since 0.0.0
	 *
	 * @param child The item to bring to the top of the List.
	 */
	void bringToTop (T item);

	/**
	 * Sends the given item to the bottom of this List.
	 *
	 * @since 0.0.0
	 *
	 * @param child The item to send to the bottom of the List.
	 */
	void sendToBack (T item);

	/**
	 * Moves the given item up one place unless it's already at the top.
	 *
	 * @since 0.0.0
	 *
	 * @param child The item to move up.
	 */
	void moveUp (T item);

	/**
	 * Moves the given item down one place unless it's already at the bottom.
	 *
	 * @since 0.0.0
	 *
	 * @param child The item to move down.
	 */
	void moveDown (T item);

	/**
	 * Reverses the order of all items in this List.
	 *
	 * @since 0.0.0
	 */
	void reverse ();

	/**
	 * Shuffles the items in this List.
	 *
	 * @since 0.0.0
	 */
	void shuffle ();

	/**
	 * Replaces an item of this List with the given newItem. `newItem` cannot be
	 * a member of this List.
	 *
	 * @since 0.0.0
	 *
	 * @param oldItem The item in this List that will be replaced.
	 * @param newItem The item to be inserted into this List.
	 */
	void replace (T oldItem, T newItem);

	/**
	 * Checks if the given item already exists within this List.
	 *
	 * @since 0.0.0
	 *
	 * @param item The item to check for the existence of.
	 *
	 * @return `true` if the item is found in this List. `false` otherwise.
	 */
	bool exists (T item);

	/**
	 * Passes all children to the given callback.
	 *
	 * @since 0.0.0
	 *
	 * @param callback The function to call.
	 */
	void each (std::function<void(T)> callback);
};

}	// namespace Zen

#endif
