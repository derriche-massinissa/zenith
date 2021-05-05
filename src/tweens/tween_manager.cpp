/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
*/

#include "tween_manager.hpp"

#include <algorithm>
#include "types/tween_config.hpp"
#include "../ecs/entity.hpp"
#include "../scene/scene.hpp"

namespace Zen {

extern entt::registry g_registry;

TweenManager::TweenManager (Scene *scene)
	: scene (scene)
{
	scene->sys.events.on("start", &TweenManager::start, this);
}

void TweenManager::start ()
{
	scene->sys.events.on("pre-update", &TweenManager::preUpdate, this);
	scene->sys.events.on("update", &TweenManager::update, this);

	timeScale = 1.;
}

Tween* TweenManager::add (TweenConfig config_)
{
	Tween* tween_ = toAdd.emplace_back(std::make_shared<Tween>(config_)).get();
	toProcess++;

	tween_->on("remove", &TweenManager::remove, this);

	return tween_;
}

void TweenManager::preUpdate ([[maybe_unused]] Uint32 time_, [[maybe_unused]] Uint32 delta_)
{
	if (toProcess == 0)
		return;

	bool removed_;
	std::shared_ptr<Tween> tween_;

	// Clear the removal list
	for (auto& tween_ : toRemove)
	{
		removed_ = false;

		// Remove from the active list
		for (auto tweenIt_ = active.begin(); tweenIt_ != active.end(); tweenIt_++)
		{
			if (*tweenIt_ == tween_)
			{
				removed_ = true;
				active.erase(tweenIt_);
				break;
			}
		}

		if (removed_)
			break;

		// Not in active list, is it in prending instead?
		for (auto tweenIt_ = pending.begin(); tweenIt_ != pending.end(); tweenIt_++)
		{
			if (*tweenIt_ == tween_)
			{
				pending.erase(tweenIt_);
				break;
			}
		}
	}
	toRemove.clear();

	// Process the addition list
	// This stops callbacks and out of sync events from populating the active array
	// mid-way during the update
	for (auto& tween_ : toAdd)
	{
		if (tween_->state == TWEEN::PENDING_ADD)
		{
			// Return true if the Tween should be started right away, otherwise false
			if (tween_->init())
			{
				tween_->play();
				active.emplace_back(tween_);
			}
			else
			{
				pending.emplace_back(tween_);
			}
		}
	}
	toAdd.clear();

	toProcess = 0;
}

void TweenManager::update (Uint32 time_, Uint32 delta_)
{
	// Scale the delta
	delta_ *= timeScale;

	for (auto& tween_ : active)
	{
		// If Tween::update returns 'true' then it means it has completed,
		// so add it to the removal list
		if (tween_->update(time_, delta_))
		{
			toRemove.emplace_back(tween_);
			toProcess++;
		}
	}
}

void TweenManager::remove (Tween *tween_)
{
	// Remove from toAdd
	for (auto it_ = toAdd.begin(); it_ != toAdd.end(); it_++)
	{
		if (it_->get() == tween_)
		{
			toAdd.erase(it_);
			break;
		}
	}

	// Remove from pending
	for (auto it_ = pending.begin(); it_ != pending.end(); it_++)
	{
		if (it_->get() == tween_)
		{
			pending.erase(it_);
			break;
		}
	}

	// Remove from active
	for (auto it_ = active.begin(); it_ != active.end(); it_++)
	{
		if (it_->get() == tween_)
		{
			active.erase(it_);
			break;
		}
	}

	// Remove from toRemove
	for (auto it_ = toRemove.begin(); it_ != toRemove.end(); it_++)
	{
		if (it_->get() == tween_)
		{
			toRemove.erase(it_);
			break;
		}
	}
}

static bool has (std::deque<std::shared_ptr<Tween>>& deque, Tween* tween)
{
	for (auto& ptr : deque)
	{
		if (ptr.get() == tween)
			return true;
	}

	return false;
}

void TweenManager::makeActive (Tween *tween_)
{
	if (has(toAdd, tween_) || has(active, tween_))
		return;

	toAdd.emplace_back(tween_);

	// Remove from pending if present
	for (auto it_ = pending.begin(); it_ != pending.end(); it_++)
	{
		if (it_->get() == tween_)
		{
			pending.erase(it_);
			break;
		}
	}

	tween_->state = TWEEN::PENDING_ADD;

	toProcess++;
}

std::vector<Tween*> TweenManager::getTweensOf (Entity target_, bool includePending_)
{
	std::vector<Tween*> output;

	for (auto& tween_ : active)
	{
		if (tween_->hasTarget(target_))
		{
			output.push_back(tween_.get());
		}
	}

	if (includePending_)
	{
		for (auto& tween_ : pending)
		{
			if (tween_->hasTarget(target_))
			{
				output.push_back(tween_.get());
			}
		}
	}

	return output;
}

bool TweenManager::isTweening (Entity entity_)
{
	for (auto& tween_ : active)
	{
		if (tween_->hasTarget(entity_) && tween_->isPlaying())
			return true;
	}

	return false;
}

void TweenManager::killAll ()
{
	for (auto& tween_ : active)
	{
		tween_->stop();
	}
}

void TweenManager::killTweensOf (Entity target_)
{
	for (Tween* tween_ : getTweensOf(target_))
		tween_->stop();
}

void TweenManager::pauseAll ()
{
	for (auto tween_ : active)
		tween_->pause();
}

void TweenManager::resumeAll ()
{
	for (auto tween_ : active)
		tween_->resume();
}

void TweenManager::setGlobalTimeScale (double value)
{
	timeScale = std::clamp(value, 0., 1.);
}

}	// namespace Zen
