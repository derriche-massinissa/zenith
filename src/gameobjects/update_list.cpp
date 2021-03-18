/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "update_list.h"

#include <algorithm>
#include <vector>
#include "../scene/scene.h"
#include "../event/event_emitter.h"

namespace Zen {
namespace GameObjects {

UpdateList::UpdateList (Scene* scene_)
	: scene (scene_)
{
	scene_->sys.events.on("start", &UpdateList::start, this);
}

UpdateList::~UpdateList ()
{
	shutdown();

	scene->sys.events.off("start", &UpdateList::start, this);
}

void UpdateList::start ()
{
	scene->sys.events.on("pre-update", &UpdateList::update, this);
	scene->sys.events.on("update", &UpdateList::sceneUpdate, this);
	scene->sys.events.once("shutdown", &UpdateList::shutdown, this);
}

void UpdateList::sceneUpdate (Uint32 time_, Uint32 delta_)
{
	for (const auto& obj_ : active)
	{
		if (obj_->active)
		{
			obj_->preUpdate(time_, delta_);
		}
	}
}

void UpdateList::shutdown (Data data_)
{
	scene->sys.events.off("pre-update", &UpdateList::update, this);
	scene->sys.events.off("update", &UpdateList::sceneUpdate, this);
	scene->sys.events.off("shutdown", &UpdateList::shutdown, this);
}

void UpdateList::add (GameObject* gameObject_)
{
	pending.emplace_back(gameObject_);

	toProcess++;
}

void UpdateList::remove (GameObject* gameObject_)
{
	destroy.emplace_back(gameObject_);

	toProcess++;
}

void UpdateList::removeAll ()
{
	for (const auto& obj_ : active)
	{
		destroy.emplace_back(obj_);

		toProcess++;
	}

	active.clear();
}

void UpdateList::update (Uint32 time_, Uint32 delta_)
{
	if (toProcess == 0)
		return;

	// Clear the destroy list
	for (const auto& obj_ : destroy)
	{
		auto it_ = std::find(active.begin(), active.end(), obj_);

		if (it_ != active.end())
		{
			active.erase(it_);
		}
	}
	destroy.clear();

	// Process the pending addition list
	for (auto& obj_ : pending)
	{
		if (!chechQueue ||
			(
				chechQueue &&
				std::find(active.begin(), active.end(), obj_) == active.end()
			)
		)
		{
			active.emplace_back(obj_);
		}
	}

	pending.clear();

	toProcess = 0;
}

std::vector<GameObject*> UpdateList::getActive ()
{
	return active;
}

int UpdateList::getLength ()
{
	return active.size();
}

}	// namespace Input
}	// namespace Zen
