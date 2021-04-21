/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "display_list.hpp"

#include "../systems/depth.hpp"
#include "../event/event_emitter.hpp"
#include "../utils/vector/index_of.hpp"

namespace Zen {

DisplayList::DisplayList ()
{
	addCallback = [this] ([[maybe_unused]] Entity gameObject) {
		queueDepthSort();
	};

	removeCallback = [this] ([[maybe_unused]] Entity gameObject) {
		queueDepthSort();
	};

	sortCallback = [] (Entity childA,  Entity childB) {
		return sortByDepth(childA, childB);
	};
}

void DisplayList::queueDepthSort ()
{
	sortChildrenFlag = true;
}

void DisplayList::depthSort ()
{
	if (sortChildrenFlag) {
		std::stable_sort(list.begin(), list.end(), sortByDepth);

		sortChildrenFlag = false;
	}
}

bool DisplayList::sortByDepth (Entity childA, Entity childB)
{
	return GetDepth(childA) < GetDepth(childB);
}

std::vector<Entity> DisplayList::getChildren ()
{
	return list;
}

int DisplayList::getIndex (Entity child_)
{
	return IndexOf(list, child_);
}

void DisplayList::setDepth (Entity entity_, int depth_)
{
	SetDepth(entity_, depth_);

	queueDepthSort();
}

}	// namespace Zen
