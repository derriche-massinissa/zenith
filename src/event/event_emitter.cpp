/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "event_emitter.hpp"

namespace Zen {

std::vector<std::string> EventEmitter::getEventNames (Entity entity_)
{
	auto iterator_ = eventMap.find(entity_);
	if (iterator_ == eventMap.end())
		return {};

	std::vector<std::string> names_;

	for (const auto& it_ : eventMap[entity_])
		names_.emplace_back(it_.first);

	return names_;
}

int EventEmitter::getListenerCount (Entity entity_, std::string event_)
{
	auto iteratorE_ = eventMap.find(entity_);
	if (iteratorE_ == eventMap.end())
		return 0;

	auto iterator_ = eventMap[entity_].find(event_);
	int count_ = 0;

	if (iterator_ != eventMap[entity_].end())
		count_ = iterator_->second.size();

	return count_;
}

int EventEmitter::getListenerCount (std::string event_)
{
	return getListenerCount(entt::null, event_);
}

std::vector<ListenerBase*> EventEmitter::getListeners (Entity entity_, std::string event_)
{
	auto iteratorE_ = eventMap.find(entity_);
	if (iteratorE_ == eventMap.end())
		return {};

	std::vector<ListenerBase*> vec_;

	// Check if event exists
	auto iterator_ = eventMap[entity_].find(event_);
	if (iterator_ != eventMap[entity_].end())
	{
		for (auto& l_ : iterator_->second)
			vec_.push_back(l_.get());
	}

	return vec_;
}

std::vector<ListenerBase*> EventEmitter::getListeners (std::string event_)
{
	return getListeners(entt::null, event_);
}

void EventEmitter::removeListener (ListenerBase* listener_)
{
	if (listener_ == nullptr)
		return;

	auto entity_ = listener_->entity;

	auto iteratorE_ = eventMap.find(entity_);
	if (iteratorE_ == eventMap.end())
		return;

	// Check if event exists
	auto iterator_ = eventMap[entity_].find(listener_->event);
	if (iterator_ != eventMap[entity_].end())
	{
		// Find listener
		for (size_t i_ = 0; i_ < iterator_->second.size(); i_++)
		{
			if (listener_ == iterator_->second[i_].get())
			{
				iterator_->second.erase(iterator_->second.begin() + i_);
				listener_ = nullptr;
				break;
			}
		}
	}

	// Remove the event if no more listeners remain
	if (iterator_->second.empty())
		eventMap[entity_].erase(iterator_);

	// Remove the entity if no more events remain
	if (iteratorE_->second.empty())
		eventMap.erase(iteratorE_);
}

void EventEmitter::off (ListenerBase* listener_)
{
	removeListener(listener_);
}

void EventEmitter::removeAllListeners (Entity entity_, std::vector<std::string> eventNames_)
{
	auto iteratorE_ = eventMap.find(entity_);
	if (iteratorE_ == eventMap.end())
		return;

	// Check if events were sent
	if (eventNames_.empty())
	{
		// If no event was sent, remove all events and their listeners
		eventMap[entity_].clear();
	}
	else
	{
		// If events were sent, remove them with their listeners
		for (auto& event_ : eventNames_)
		{
			// Check if event exists
			auto iterator_ = eventMap[entity_].find(event_);
			if (iterator_ != eventMap[entity_].end())
				eventMap[entity_].erase(iterator_);
		}
	}
}

void EventEmitter::removeAllListeners (std::vector<std::string> eventNames_)
{
	removeAllListeners(entt::null, eventNames_);
}

void EventEmitter::removeAllListeners (Entity entity_, std::string eventName_)
{
	std::vector<std::string> events_ {eventName_};
	removeAllListeners(entity_, events_);
}

void EventEmitter::removeAllListeners (std::string eventName_)
{
	std::vector<std::string> events_ {eventName_};
	removeAllListeners(entt::null, events_);
}

void EventEmitter::clear ()
{
	eventMap.clear();
}

}	// namespace Zen
