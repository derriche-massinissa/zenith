/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "display_list.h"

#include <algorithm>

#include "gameobject.h"

namespace Zen {
namespace GameObjects {

void DisplayList::add (std::unique_ptr<GameObject>& gameObject_)
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

void remove (GameObject* gameObject_)
{
	for (auto it = list.begin(); it != list.end(); it++)
	{
		if (gameObject_ == it->get())
		{
			list.erase(it);
			break;
		}
	}
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

static bool DisplayList::sortByDepth (
		std::unique_ptr<GameObject> childA,
		std::unique_ptr<GameObject> childB)
{
	return childA->getDepth() < childB->getDepth();
}

std::vector<GameObject*> DisplayList::getChildren ()
{
	std::vector<GameObject*> out;

	for (const auto& obj : list)
	{
		out.emplace_back(list.get());
	}

	return out;
}

}	// namespace Input
}	// namespace Zen
