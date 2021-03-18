/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "display_list.h"

#include <algorithm>
#include <iterator>

#include "gameobject.h"

namespace Zen {
namespace GameObjects {

void DisplayList::add (std::unique_ptr<GameObject> gameObject_)
{
	GameObject* obj_ = gameObject_.get();

	list.emplace_back(std::move(gameObject_));

	obj_->emit("removed-from-scene");

	if (obj_->displayList)
		obj_->displayList->remove(obj_);

	obj_->displayList = this;

	queueDepthSort();

	obj_->emit("added-to-scene");
}

void DisplayList::remove (GameObject* gameObject_)
{
	for (auto it = list.begin(); it != list.end(); it++)
	{
		if (gameObject_ == it->get())
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
		const std::unique_ptr<GameObject>& childA,
		const std::unique_ptr<GameObject>& childB)
{
	return childA->getDepth() < childB->getDepth();
}

std::vector<GameObject*> DisplayList::getChildren ()
{
	std::vector<GameObject*> out_;

	for (const auto& obj_ : list)
	{
		out_.emplace_back(obj_.get());
	}

	return out_;
}

int DisplayList::getIndex (GameObject* child_)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (child_ == list[i].get())
		{
			return i;
		}
	}

	return -1;
}

}	// namespace Input
}	// namespace Zen
