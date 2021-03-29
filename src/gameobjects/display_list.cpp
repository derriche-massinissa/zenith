/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "display_list.h"

#include <algorithm>
#include <iterator>

#include "../systems/depth.hpp"
#include "../event/event_emitter.hpp"

namespace Zen {
extern EventEmitter g_event;

namespace GameObjects {

void DisplayList::add (Entity gameObject_)
{
	list.emplace_back(gameObject_);

	g_event.emit(gameObject_, "removed-from-scene");

	/*
	 * TODO FIXME
	if (obj_->displayList)
		obj_->displayList->remove(obj_);
		*/

	//obj_->displayList = this;

	queueDepthSort();

	g_event.emit(gameObject_, "added-to-scene");
}

void DisplayList::remove (Entity gameObject_)
{
	for (auto it = list.begin(); it != list.end(); it++)
	{
		if (gameObject_ == *it)
		{
			list.erase(it);
			break;
		}
	}

	queueDepthSort();
}

void DisplayList::queueDepthSort ()
{
	sortChildrenFlag = true;
}

void DisplayList::depthSort ()
{
	if (sortChildrenFlag)
	{
		std::stable_sort(list.begin(), list.end(), sortByDepth);

		sortChildrenFlag = false;
	}
}

bool DisplayList::sortByDepth (
		const Entity childA,
		const Entity childB)
{
	return GetDepth(childA) < GetDepth(childB);
}

std::vector<Entity> DisplayList::getChildren ()
{
	std::vector<Entity> out_;

	for (const auto& obj_ : list)
	{
		out_.emplace_back(obj_);
	}

	return out_;
}

int DisplayList::getIndex (Entity child_)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (child_ == list[i])
		{
			return i;
		}
	}

	return -1;
}

}	// namespace Input
}	// namespace Zen
