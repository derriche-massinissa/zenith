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
#include "../event/event_emitter.hpp"

namespace Zen {
namespace GameObjects {

UpdateList::UpdateList (Scene* scene_)
	: scene (scene_)
{
	lStart = scene_->sys.events.on("start", &UpdateList::start, this);
}

UpdateList::~UpdateList ()
{
	shutdown();

	scene->sys.events.off(lStart);
}

void UpdateList::start ()
{
	lPreUpdate = scene->sys.events.on("pre-update", &UpdateList::update, this);
	lUpdate = scene->sys.events.on("update", &UpdateList::sceneUpdate, this);
	lShutdown = scene->sys.events.once("shutdown", &UpdateList::shutdown, this);
}

void UpdateList::sceneUpdate ([[maybe_unused]] Uint32 time_, [[maybe_unused]] Uint32 delta_)
{
	/*
	 * TODO entity update
	for (const auto& obj_ : active)
	{
		if (obj_->active)
		{
			obj_->preUpdate(time_, delta_);
		}
	}
	*/
}

void UpdateList::shutdown ()
{
	scene->sys.events.off(lPreUpdate);
	scene->sys.events.off(lUpdate);
	scene->sys.events.off(lShutdown);
}

void UpdateList::add (Entity gameObject_)
{
	pending.emplace_back(gameObject_);

	toProcess++;
}

void UpdateList::remove (Entity gameObject_)
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

void UpdateList::update ([[maybe_unused]] Uint32 time_, [[maybe_unused]] Uint32 delta_)
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

std::vector<Entity> UpdateList::getActive ()
{
	return active;
}

int UpdateList::getLength ()
{
	return active.size();
}

}	// namespace Input
}	// namespace Zen
