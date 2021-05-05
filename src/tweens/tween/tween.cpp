/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "tween.hpp"
#include <algorithm>
#include <limits>
#include "../../systems/flip.hpp"
#include "../events/events.hpp"
#include "../easing.hpp"
#include "../../utils/safe_sub.hpp"

namespace Zen {

Tween::Tween (TweenConfig config_)
	: targets (config_.targets)
{
	if (config_.entries.empty())
	{
		// If no entries, make a base TweenData
		for (Entity target_ : config_.targets)
		{
			TweenData& data_ = data.emplace_back(TweenData{
					.target = target_,
					.index = 0,
					.from = config_.from,
					.to = config_.to,
					.action = config_.action,
					.duration = config_.duration,
					.delay = config_.delay,
					.yoyo = config_.yoyo,
					.hold = config_.hold,
					.repeat = config_.repeat,
					.repeatDelay = config_.repeatDelay,
					.flipX = config_.flipX,
					.flipY = config_.flipY
					});

			if (config_.ease != nullptr)
				data_.ease = config_.ease;
			else
				data_.ease = GetEaseFunction(config_.easing);
		}
	}
	else
	{
		for (Entity target_ : config_.targets)
		{
			// Loop through each entry in the config
			for (size_t i_ = 0; i_ < config_.entries.size(); i_++)
			{
				auto& entry_ = config_.entries[i_];

				// Create 1 TweenData per entry, per target
				TweenData& data_ = data.emplace_back(TweenData {
						.target = target_,
						.index = i_,
						.from = entry_.from,
						.to = entry_.to,
						.action = entry_.action,
						.getActiveValue = entry_.getActive,
						.getStartValue = entry_.getStart,
						.getEndValue = entry_.getEnd,
						.duration = entry_.duration,
						.delay = entry_.delay,
						.yoyo = entry_.yoyo,
						.hold = entry_.hold,
						.repeat = entry_.repeat,
						.repeatDelay = entry_.repeatDelay,
						.flipX = entry_.flipX,
						.flipY = entry_.flipY
						});

				if (entry_.ease != nullptr)
					data_.ease = entry_.ease;
				else
					data_.ease = GetEaseFunction(entry_.easing);
			}
		}
	}

	targets = config_.targets;
	completeDelay = config_.completeDelay;
	loop = config_.loop;
	loopDelay = config_.loopDelay;
	paused = config_.paused;

	startDelay = config_.delay;
	duration = config_.duration;

	// Callbacks
	callbacks.onActive = config_.onActive;
	callbacks.onComplete = config_.onComplete;
	callbacks.onLoop = config_.onLoop;
	callbacks.onRepeat = config_.onRepeat;
	callbacks.onStart = config_.onStart;
	callbacks.onStop = config_.onStop;
	callbacks.onUpdate = config_.onUpdate;
	callbacks.onYoyo = config_.onYoyo;
}

double Tween::getValue (int index)
{
	return data.at(index).current;
}

Tween& Tween::setTimeScale (double value)
{
	timeScale = std::clamp(value, 0., 1.);

	return *this;
}

double Tween::getTimeScale ()
{
	return timeScale;
}

bool Tween::isPlaying ()
{
	return (state == TWEEN::ACTIVE);
}

bool Tween::isPaused ()
{
	return (state == TWEEN::PAUSED);
}

bool Tween::hasTarget (Entity target_)
{
	for (auto& d_ : data)
	{
		if (d_.target == target_)
			return true;
	}

	return false;
}

Tween& Tween::updateTo (double value_, bool startToCurrent_)
{
	for (auto& d_ : data)
	{
		d_.end = value_;

		if (startToCurrent_)
			d_.start = d_.current;
	}

	return *this;
}

Tween& Tween::restart ()
{
	elapsed = 0;
	progress = 0;
	totalElapsed = 0;
	totalProgress = 0;

	if (state == TWEEN::ACTIVE)
	{
		return seek(0);
	}
	else if (state == TWEEN::PENDING_ADD)
	{
		return *this;
	}
	else
	{
		return play();
	}
}

void Tween::calcDuration ()
{
	Uint32 maxDuration_ = 0;
	Uint32 minDelay_ = std::numeric_limits<Uint32>::max();

	for (auto& data_ : data)
	{
		// Set t1 (duration + hold + yoyo)
		data_.t1 = data_.duration + data_.hold;
		if (data_.yoyo)
			data_.t1 += data_.duration;

		// Set t2 (repeatDelay + duration + hold + yoyo)
		data_.t2 = data_.t1 + data_.repeatDelay;

		// Total duration
		data_.totalDuration = data_.delay + data_.t1;

		if (data_.repeat == -1)
			data_.totalDuration = std::numeric_limits<Uint32>::max();
		else if (data_.repeat > 0)
			data_.totalDuration += data_.t2 * data_.repeat;

		// Get the longest TweenData from the Tween (Used to calculate the Tween TD)
		if (data_.totalDuration > maxDuration_)
			maxDuration_ = data_.totalDuration;

		if (data_.delay < minDelay_)
			minDelay_ = data_.delay;
	}

	// Exclude loop values

	// If duration has been set to 0 then we give it a super-low value so that it
	// always renders at least 1 frame, but no more, without causing divided by
	// zero errors elsewhere.
	duration = std::max(maxDuration_, static_cast<Uint32>(1));

	loopCounter = (loop == -1) ? std::numeric_limits<size_t>::max() : loop;

	if (loopCounter > 0)
		totalDuration = duration + completeDelay + ((duration + loopDelay)
				* loopCounter);
	else
		totalDuration = duration + completeDelay;

	// How long before this Tween starts playback?
	startDelay = minDelay_;
}

bool Tween::init ()
{
	if (paused)
	{
		state = TWEEN::PENDING_ADD;
		pausedState = TWEEN::INIT;

		return false;
	}

	for (auto& data_ : data)
	{

		if (data_.gen.delay != nullptr)
			data_.delay = data_.gen.delay(data_.target, 0, data_.index,
					targets.size());

		if (data_.gen.duration != nullptr)
			data_.duration = std::max(data_.gen.duration(data_.target, 0,
						data_.index, targets.size()), static_cast<Uint32>(1));

		if (data_.gen.hold != nullptr)
			data_.hold = data_.gen.hold(data_.target, 0, data_.index,
					targets.size());

		if (data_.gen.repeat != nullptr)
			data_.repeat = data_.gen.repeat(data_.target, 0, data_.index,
					targets.size());

		if (data_.gen.repeatDelay != nullptr)
			data_.repeatDelay = data_.gen.repeatDelay(data_.target, 0, data_.index,
					targets.size());
	}

	calcDuration();

	progress = 0;
	totalProgress = 0;
	elapsed = 0;
	totalElapsed = 0;

	state = TWEEN::INIT;

	return true;
}

void Tween::makeActive ()
{
	dispatchTweenEvent(ZEN_TWEENS_EVENTS_TWEEN_ACTIVE, callbacks.onActive);
}

void Tween::nextState ()
{
	if (loopCounter > 0)
	{
		elapsed = 0;
		progress = 0;
		loopCounter--;

		resetTweenData(true);

		if (loopDelay > 0)
		{
			countdown = loopDelay;
			state = TWEEN::LOOP_DELAY;
		}
		else
		{
			state = TWEEN::ACTIVE;
			dispatchTweenEvent(ZEN_TWEENS_EVENTS_TWEEN_LOOP, callbacks.onLoop);
		}
	}
	else if (completeDelay > 0)
	{
		state = TWEEN::COMPLETE_DELAY;
		countdown = completeDelay;
	}
	else
	{
		state = TWEEN::PENDING_REMOVE;
		dispatchTweenEvent(ZEN_TWEENS_EVENTS_TWEEN_COMPLETE, callbacks.onComplete);
	}
}

Tween& Tween::pause ()
{
	if (state == TWEEN::PAUSED)
		return *this;

	paused = true;
	pausedState = state;
	state = TWEEN::PAUSED;

	return *this;
}

Tween& Tween::play ()
{
	if (state == TWEEN::INIT)
	{
		resetTweenData(false);
		state = TWEEN::ACTIVE;

		return *this;
	}
	else if (state == TWEEN::ACTIVE ||
			(state == TWEEN::PENDING_ADD && pausedState == TWEEN::PENDING_ADD))
	{
		return *this;
	}

	/*
	if (parentIsTimeline)
	{
		resetTweenData(resetFromTimeline);

		if (calculatedOffset == 0)
		{
			state = TWEEN::ACTIVE;
		}
		else
		{
			countdown = calculatedOffset;
			state = TWEEN::OFFSET_DELAY;
		}
	}
	else */
	if (paused)
	{
		paused = false;
		makeActive();
	}
	else
	{
		resetTweenData(false);
		state = TWEEN::ACTIVE;
		makeActive();
	}

	return *this;
}

void Tween::resetTweenData (bool resetFromLoop)
{
	for (auto& data_ : data)
	{
		data_.progress = 0;
		data_.elapsed = 0;

		data_.repeatCounter = (data_.repeat == -1)
			? std::numeric_limits<int>::max()
			: data_.repeat;

		if (resetFromLoop)
		{
			data_.start = (data_.getStartValue != nullptr)
				? data_.getStartValue(data_.target, data_.start, data_.index,
						targets.size())
				: data_.from;

			data_.end = (data_.getEndValue != nullptr)
				? data_.getStartValue(data_.target, data_.end, data_.index,
						targets.size())
				: data_.to;

			data_.current = data_.start;

			data_.state = TWEEN::PLAYING_FORWARD;
		}
		else
		{
			data_.state = TWEEN::PENDING_RENDER;
		}

		if (data_.delay > 0)
		{
			data_.elapsed = data_.delay;
			data_.state = TWEEN::DELAY;
		}

		if (data_.getActiveValue != nullptr)
			data_.action(data_.target,
					data_.getActiveValue(data_.target, data_.start, 0, 0));
	}
}

Tween& Tween::resume ()
{
	if (state == TWEEN::PAUSED)
	{
		paused = false;
		state = pausedState;
	}
	else
	{
		play();
	}

	return *this;
}

Tween& Tween::seek (double toPosition, double delta)
{
	elapsed = 0;
	progress = 0;
	totalElapsed = 0;
	totalProgress = 0;

	for (auto& data_ : data)
	{
		data_.progress = 0;
		data_.elapsed = 0;

		data_.repeatCounter = (data_.repeat == -1)
			? std::numeric_limits<size_t>::max()
			: data_.repeat;

		if (data_.gen.delay != nullptr)
			data_.delay = data_.gen.delay(data_.target, 0, data_.index,
					targets.size());

		if (data_.gen.duration != nullptr)
			data_.duration = std::max(data_.gen.duration(data_.target, 0,
						data_.index, targets.size()), static_cast<Uint32>(1));

		if (data_.gen.hold != nullptr)
			data_.hold = data_.gen.hold(data_.target, 0, data_.index,
					targets.size());

		if (data_.gen.repeat != nullptr)
			data_.repeat = data_.gen.repeat(data_.target, 0, data_.index,
					targets.size());

		if (data_.gen.repeatDelay != nullptr)
			data_.repeatDelay = data_.gen.repeatDelay(data_.target, 0, data_.index,
					targets.size());

		data_.current = data_.start;
		data_.state = TWEEN::PLAYING_FORWARD;

		updateTweenData(&data_, 0);

		if (data_.delay > 0)
		{
			data_.elapsed = data_.delay;
			data_.state = TWEEN::DELAY;
		}
	}

	calcDuration();

	bool wasPaused = false;

	if (state == TWEEN::PAUSED)
	{
		wasPaused = true;
		state = TWEEN::ACTIVE;
	}

	isSeeking = true;

	do
	{
		update(0, delta);
	} while (totalProgress < toPosition);

	isSeeking = false;

	if (wasPaused)
		state = TWEEN::PAUSED;

	return *this;
}

Tween& Tween::complete (Uint32 delay)
{
	if (delay)
	{
		state = TWEEN::COMPLETE_DELAY;
		countdown = delay;
	}
	else
	{
		state = TWEEN::PENDING_REMOVE;
		dispatchTweenEvent(ZEN_TWEENS_EVENTS_TWEEN_COMPLETE, callbacks.onComplete);
	}

	return *this;
}

Tween& Tween::remove ()
{
	state = TWEEN::PENDING_REMOVE;
	emit("remove", this);

	return *this;
}

Tween& Tween::stop (double resetTo)
{
	if (state == TWEEN::ACTIVE)
	{
		if (resetTo >= 0)
			seek(resetTo);
	}

	dispatchTweenEvent(ZEN_TWEENS_EVENTS_TWEEN_STOP, callbacks.onStop);
	removeAllListeners();
	state = TWEEN::PENDING_REMOVE;

	return *this;
}

bool Tween::update ([[maybe_unused]] Uint32 time_, Uint32 delta_)
{
	if (state == TWEEN::PAUSED)
		return false;

	delta_ *= timeScale;

	elapsed += delta_;
	progress = std::min((1.*elapsed) / duration, 1.);

	totalElapsed += delta_;
	totalProgress = std::min((1.*totalElapsed) / totalDuration, 1.);

	switch (state)
	{
		case TWEEN::ACTIVE:
		{
			if (!hasStarted && !isSeeking)
			{
				startDelay = SafeSub(startDelay, delta_);
				if (startDelay == 0)
				{
					hasStarted = true;
					dispatchTweenEvent(ZEN_TWEENS_EVENTS_TWEEN_START, callbacks.onStart);
				}
			}

			bool stillRunning_ = false;

			for (auto& data_ : data)
			{
				if (updateTweenData(&data_, delta_))
					stillRunning_ = true;
			}

			// Anything still running? If not, we're done
			if (!stillRunning_)
				nextState();

		}	break;

		case TWEEN::LOOP_DELAY:
		{
			countdown = SafeSub(countdown, delta_);

			if (countdown == 0)
			{
				state = TWEEN::ACTIVE;
				dispatchTweenEvent(ZEN_TWEENS_EVENTS_TWEEN_LOOP, callbacks.onLoop);
			}

		}	break;

		case TWEEN::OFFSET_DELAY:
		{
			countdown = SafeSub(countdown, delta_);

			if (countdown == 0)
				state = TWEEN::ACTIVE;

		}	break;

		case TWEEN::COMPLETE_DELAY:
		{
			countdown = SafeSub(countdown, delta_);

			if (countdown == 0)
			{
				state = TWEEN::PENDING_REMOVE;
				dispatchTweenEvent(ZEN_TWEENS_EVENTS_TWEEN_COMPLETE,
						callbacks.onComplete);
			}

		}	break;

		default:
			break;
	}

	return (state == TWEEN::PENDING_REMOVE);
}

void Tween::dispatchTweenDataEvent (std::string event_,
		std::function<void(std::vector<Entity>)> callback_, TweenData& tweenData_)
{
	if (isSeeking)
		return;

	emit(event_, this, tweenData_.target, tweenData_.current, tweenData_.previous);

	if (callback_ != nullptr)
		callback_({ tweenData_.target });
}

void Tween::dispatchTweenEvent (std::string event_,
		std::function<void(std::vector<Entity>)> callback_)
{
	if (isSeeking)
		return;

	emit(event_, this, targets);

	if (callback_ != nullptr)
		callback_( targets );
}

TWEEN Tween::setStateFromEnd (TweenData *tweenData_, Uint32 diff_)
{
	if (tweenData_->yoyo)
	{
		// We've hit the end of a Playing Forward TweenData and we have a yoyo

		// Account for any extra time we got from the previous frame
		tweenData_->elapsed = diff_;
		tweenData_->progress = (1.*diff_) / tweenData_->duration;

		if (tweenData_->flipX)
			ToggleFlipX( tweenData_->target );

		if (tweenData_->flipY)
			ToggleFlipY( tweenData_->target );

		dispatchTweenDataEvent(ZEN_TWEENS_EVENTS_TWEEN_YOYO, callbacks.onYoyo,
				*tweenData_);

		tweenData_->start = (tweenData_->getStartValue != nullptr)
			? tweenData_->getStartValue(tweenData_->target, tweenData_->start,
					tweenData_->index, targets.size())
			: tweenData_->from;

		return TWEEN::PLAYING_BACKWARD;
	}
	else if (tweenData_->repeatCounter > 0)
	{
		// We've hit the end of a Playing Forward TweenData and we have a Repeat.
		// So we're going to go right back to the start to repeat it again.
		tweenData_->repeatCounter--;

		// Account for any extra time we got from the previous frame
		tweenData_->elapsed = diff_;
		tweenData_->progress = (1.*diff_) / tweenData_->duration;

		if (tweenData_->flipX)
			ToggleFlipX( tweenData_->target );

		if (tweenData_->flipY)
			ToggleFlipY( tweenData_->target );

		tweenData_->start = (tweenData_->getStartValue != nullptr)
			? tweenData_->getStartValue(tweenData_->target, tweenData_->start,
					tweenData_->index, targets.size())
			: tweenData_->from;

		tweenData_->end = (tweenData_->getEndValue != nullptr)
			? tweenData_->getEndValue(tweenData_->target, tweenData_->end,
					tweenData_->index, targets.size())
			: tweenData_->to;

		// Delay?
		if (tweenData_->repeatDelay > 0)
		{
			tweenData_->elapsed = tweenData_->repeatDelay - diff_;

			tweenData_->current = tweenData_->start;

			tweenData_->action(tweenData_->target, tweenData_->current);

			return TWEEN::REPEAT_DELAY;
		}
		else
		{
			dispatchTweenDataEvent(ZEN_TWEENS_EVENTS_TWEEN_REPEAT,
					callbacks.onRepeat, *tweenData_);

			return TWEEN::PLAYING_FORWARD;
		}
	}

	return TWEEN::COMPLETE;
}

TWEEN Tween::setStateFromStart (TweenData *tweenData_, Uint32 diff_)
{
	if (tweenData_->repeatCounter > 0)
	{
		tweenData_->repeatCounter--;

		// Account for any extra time we got from the previous frame
		tweenData_->elapsed = diff_;
		tweenData_->progress = (1.*diff_) / tweenData_->duration;

		if (tweenData_->flipX)
			ToggleFlipX( tweenData_->target );

		if (tweenData_->flipY)
			ToggleFlipY( tweenData_->target );

		tweenData_->end = (tweenData_->getEndValue != nullptr)
			? tweenData_->getEndValue(tweenData_->target, tweenData_->start,
					tweenData_->index, targets.size())
			: tweenData_->to;

		// Delay?
		if (tweenData_->repeatDelay > 0)
		{
			tweenData_->elapsed = tweenData_->repeatDelay - diff_;

			tweenData_->current = tweenData_->start;

			tweenData_->action(tweenData_->target, tweenData_->current);

			return TWEEN::REPEAT_DELAY;
		}
		else
		{
			dispatchTweenDataEvent(ZEN_TWEENS_EVENTS_TWEEN_REPEAT,
					callbacks.onRepeat, *tweenData_);

			return TWEEN::PLAYING_FORWARD;
		}
	}

	return TWEEN::COMPLETE;
}

bool Tween::updateTweenData (TweenData *tweenData_, Uint32 delta_)
{
	switch (tweenData_->state)
	{
		case TWEEN::PLAYING_FORWARD:
		case TWEEN::PLAYING_BACKWARD:
		{
			if (tweenData_->target == entt::null)
			{
				tweenData_->state = TWEEN::COMPLETE;
				break;
			}
			Uint32 elapsed_ = tweenData_->elapsed;
			Uint32 duration_ = tweenData_->duration;
			Uint32 diff_ = 0;

			elapsed_ += delta_;

			if (elapsed_ > duration_)
			{
				diff_ = elapsed_ - duration_;
				elapsed_ = duration_;
			}

			bool forward_ = (tweenData_->state == TWEEN::PLAYING_FORWARD);
			double progress_ = (1.*elapsed_) / duration_;

			tweenData_->elapsed = elapsed_;
			tweenData_->progress = progress_;
			tweenData_->previous = tweenData_->current;

			if (progress_ >= 1)
			{
				if (forward_)
				{
					tweenData_->current = tweenData_->end;
					tweenData_->action(tweenData_->target, tweenData_->end);

					if (tweenData_->hold > 0)
					{
						tweenData_->elapsed = tweenData_->hold - diff_;
						tweenData_->state = TWEEN::HOLD_DELAY;
					}
					else
					{
						tweenData_->state = setStateFromEnd(tweenData_, diff_);
					}
				}
				else
				{
					tweenData_->current = tweenData_->start;
					tweenData_->action(tweenData_->target, tweenData_->start);

					tweenData_->state = setStateFromStart(tweenData_, diff_);
				}
			}
			else
			{
				double v_ = (forward_)
					? tweenData_->ease(progress_)
					: tweenData_->ease(1 - progress_);

				tweenData_->current = tweenData_->start + ((tweenData_->end -
							tweenData_->start) * v_);

				tweenData_->action(tweenData_->target, tweenData_->current);
			}

			dispatchTweenDataEvent(ZEN_TWEENS_EVENTS_TWEEN_UPDATE,
					callbacks.onUpdate, *tweenData_);

		}	break;

		case TWEEN::DELAY:
		{
			tweenData_->elapsed = SafeSub(tweenData_->elapsed, delta_);

			if (tweenData_->elapsed == 0)
			{
				tweenData_->state = TWEEN::PENDING_RENDER;
			}
		}	break;

		case TWEEN::REPEAT_DELAY:
		{
			tweenData_->elapsed = SafeSub(tweenData_->elapsed, delta_);

			if (tweenData_->elapsed == 0)
			{
				tweenData_->state = TWEEN::PLAYING_FORWARD;

				dispatchTweenDataEvent(ZEN_TWEENS_EVENTS_TWEEN_REPEAT,
						callbacks.onRepeat, *tweenData_);
			}
		}	break;

		case TWEEN::HOLD_DELAY:
		{
			tweenData_->elapsed = SafeSub(tweenData_->elapsed, delta_);

			if (tweenData_->elapsed == 0)
			{
				tweenData_->state = setStateFromEnd(tweenData_,
						tweenData_->elapsed);
			}
		}	break;

		case TWEEN::PENDING_RENDER:
		{
			if (tweenData_->target != entt::null)
			{
				tweenData_->start = (tweenData_->getStartValue != nullptr)
					? tweenData_->getStartValue(tweenData_->target,
						tweenData_->start, tweenData_->index, targets.size())
					: tweenData_->from;

				tweenData_->end = (tweenData_->getEndValue != nullptr)
					? tweenData_->getEndValue(tweenData_->target,
						tweenData_->start, tweenData_->index, targets.size())
					: tweenData_->to;

				tweenData_->current = tweenData_->start;

				tweenData_->action(tweenData_->target, tweenData_->start);

				tweenData_->state = TWEEN::PLAYING_FORWARD;
			}
			else
			{
				tweenData_->state = TWEEN::COMPLETE;
			}
		}	break;

		default:
			break;
	}

	// Return `true` if this TweenData still playing, otherwise return `false`
	return (tweenData_->state != TWEEN::COMPLETE);
}

}	// namespace Zen
