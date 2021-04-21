/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "list.hpp"

#include "../ecs/entity.hpp"
#include "../utils/vector/index_of.hpp"
#include "../utils/vector/remove.hpp"
#include "../utils/vector/contains.hpp"
#include "../math/random.hpp"
#include <algorithm>
#include <array>

namespace Zen {

template <typename T>
void List<T>::add (T item_, bool skipCallback_)
{
	if (unique && exists(item_))
		return;

	list.emplace_back(item_);

	if (!skipCallback_ && addCallback)
		addCallback(item_);
}

template <typename T>
void List<T>::addAt (T item_, std::size_t index_, bool skipCallback_)
{
	list.emplace(list.begin() + index_, item_);

	if (!skipCallback_ && addCallback)
		addCallback(item_);
}

template <typename T>
T List<T>::getAt (std::size_t index_)
{
	return list.at(index_);
}

template <typename T>
std::size_t List<T>::getIndex (T item_)
{
	return IndexOf(list, item_);
}

template <typename T>
void List<T>::sort (std::function<bool(T, T)> handler_)
{
	if (handler_ != nullptr)
		std::stable_sort(list.begin(), list.end(), handler_);
	else if (sortCallback != nullptr)
		std::stable_sort(list.begin(), list.end(), sortCallback);
}

template <typename T>
T List<T>::getRandom (std::size_t start_, std::size_t length_)
{
	return Math::Random.pick(list, start_, length_);
}

template <typename T>
void List<T>::swap (std::size_t index1_, std::size_t index2_)
{
	auto it1_ = list.begin() + index1_;
	auto it2_ = list.begin() + index2_;

	std::iter_swap(it1_, it2_);
}

template <typename T>
void List<T>::swap (T item1_, T item2_)
{
	auto it1_ = std::find(list.begin(), list.end(), item1_);
	auto it2_ = std::find(list.begin(), list.end(), item2_);

	std::iter_swap(it1_, it2_);
}

template <typename T>
void List<T>::moveTo (T item_, std::size_t index_)
{
	Remove(list, item_);
	list.emplace(list.begin() + index_, item_);
}

template <typename T>
void List<T>::remove (T item_, bool skipCallback_)
{
	Remove(list, item_);

	if (!skipCallback_ && removeCallback)
		removeCallback(item_);
}

template <typename T>
void List<T>::remove (std::vector<T> items_, bool skipCallback_)
{
	for (auto& item_ : items_)
		remove(item_, skipCallback_);
}

template <typename T>
void List<T>::removeAt (std::size_t index_, bool skipCallback_)
{
	T item_ = list.at(index_);
	Remove(list, item_);

	if (!skipCallback_ && removeCallback)
		removeCallback(item_);
}

template <typename T>
void List<T>::removeBetween (std::size_t start_, std::size_t end_, bool skipCallback_)
{
	std::vector<T> items_;
	items_.reserve(end_ - start_);
	items_.resize(end_ - start_);
	for (std::size_t i_ = start_; i_ < end_; i_++)
		items_[i_] = list[i_];

	list.erase(list.begin() + start_, list.begin() + end_);

	if (!skipCallback_ && removeCallback)
	{
		for (auto& item_ : items_)
			removeCallback(item_);
	}
}

template <typename T>
void List<T>::removeAll (bool skipCallback_)
{
	std::size_t i_ = list.size();
	while (i_--)
	{
		remove(list[i_], skipCallback_);
	}
}

template <typename T>
void List<T>::bringToTop (T item_)
{
	moveTo(item_, list.size() - 1);
}

template <typename T>
void List<T>::sendToBack (T item_)
{
	moveTo(item_, 0);
}

template <typename T>
void List<T>::moveUp (T item_)
{
	auto idx_ = IndexOf(list, item_);

	if (idx_ >= 0 && idx_ < (int)(list.size() - 1))
		std::iter_swap(list.begin() + idx_, list.begin() + idx_ + 1);
}

template <typename T>
void List<T>::moveDown (T item_)
{
	auto idx_ = IndexOf(list, item_);

	if (idx_ > 0)
		std::iter_swap(list.begin() + idx_, list.begin() + idx_ - 1);
}

template <typename T>
void List<T>::reverse ()
{
	std::reverse(list.begin(), list.end());
}

template <typename T>
void List<T>::shuffle ()
{
	Math::Random.shuffle(&list);
}

template <typename T>
void List<T>::replace (T oldItem_, T newItem_)
{
	if (!exists(newItem_))
		std::replace(list.begin(), list.end(), oldItem_, newItem_);
}

template <typename T>
bool List<T>::exists (T item_)
{
	return Contains(list, item_);
}

template <typename T>
void List<T>::each (std::function<void(T)> callback_)
{
	for (auto& item_ : list)
		callback_(item_);
}

// Explicit template instantiation
// For DisplayList and UpdateList
template class List<Entity>;

}	// namespace Zen
