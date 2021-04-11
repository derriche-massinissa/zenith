/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "event_emitter.hpp"

namespace Zen {

std::vector<std::string> EventEmitter::getEventNames ()
{
	std::vector<std::string> names_;

	for (const auto& it_ : eventMap)
		names_.emplace_back(it_.first);

	return names_;
}

int EventEmitter::getListenerCount (std::string event_)
{
	auto iterator_ = eventMap.find(event_);
	int count_ = 0;

	if (iterator_ != eventMap.end())
		count_ = iterator_->second.size();

	return count_;
}

std::vector<ListenerBase*> EventEmitter::getListeners (std::string event_)
{
	std::vector<ListenerBase*> vec_ {};

	// Check if event exists
	auto iterator_ = eventMap.find(event_);
	if (iterator_ != eventMap.end())
	{
		for (size_t i_ = 0; i_ < iterator_->second.size(); i_++)
			vec_.emplace_back(iterator_->second[i_].get());
	}

	return vec_;
}

void EventEmitter::removeListener (ListenerBase*& listener_)
{
	if (listener_ == nullptr) return;

	// Check if event exists
	auto iterator_ = eventMap.find(listener_->event);
	if (iterator_ != eventMap.end())
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
}

void EventEmitter::off (ListenerBase*& listener_)
{
	removeListener(listener_);
}

void EventEmitter::removeAllListeners (std::vector<std::string> eventNames_)
{
	// Check if events were sent
	if (eventNames_.empty())
	{
		// If no event was sent, remove all events and their listeners
		eventMap.clear();
	}
	else
	{
		// If events were sent, remove them with their listeners
		for (auto& event_ : eventNames_)
		{
			// Check if event exists
			auto iterator_ = eventMap.find(event_);
			if (iterator_ != eventMap.end())
				eventMap.erase(iterator_);
		}
	}
}

void EventEmitter::removeAllListeners (std::string eventName_)
{
	std::vector<std::string> events_ {eventName_};

	removeAllListeners(events_);
}

void EventEmitter::shutdown ()
{
	eventMap.clear();
}

/*
std::vector<std::string> EventEmitter::getEventNames (Entity entity_)
{
	std::vector<std::string> names_;

	if (globalEventMap.find(entity_) != globalEventMap.end())
	{
		for (const auto& it_ : globalEventMap[entity_])
			names_.emplace_back(it_.first);
	}

	return names_;
}

int EventEmitter::getListenerCount (Entity entity_, std::string event_)
{
	if (globalEventMap.find(entity_) == globalEventMap.end())
		return 0;

	auto iterator_ = globalEventMap[entity_].find(event_);
	int count_ = 0;

	if (iterator_ != globalEventMap[entity_].end())
		count_ = iterator_->second.size();

	return count_;
}

std::vector<ListenerBase*> EventEmitter::getListeners (Entity entity_, std::string event_)
{
	std::vector<ListenerBase*> vec_ {};

	if (globalEventMap.find(entity_) == globalEventMap.end())
		return vec_;

	// Check if event exists
	auto iterator_ = globalEventMap[entity_].find(event_);
	if (iterator_ != globalEventMap[entity_].end())
	{
		for (size_t i_ = 0; i_ < iterator_->second.size(); i_++)
			vec_.emplace_back(iterator_->second[i_].get());
	}

	return vec_;
}

void EventEmitter::removeListener (Entity entity_, std::string event_, ListenerBase* listener_)
{
	if (globalEventMap.find(entity_) == globalEventMap.end())
		return;

	// Check if event exists
	auto iterator_ = globalEventMap[entity_].find(event_);
	if (iterator_ != globalEventMap[entity_].end())
	{
		// Find listener
		for (size_t l_ = 0; l_ < iterator_->second.size(); l_++)
		{
			if (listener_ == iterator_->second[l_].get())
			{
				iterator_->second.erase(iterator_->second.begin() + l_);
				break;
			}
		}
	}
}

void EventEmitter::removeAllListeners (Entity entity_, std::vector<std::string> eventNames_)
{
	if (globalEventMap.find(entity_) == globalEventMap.end())
		return;

	// Check if events were sent
	if (eventNames_.empty())
	{
		// If no event was sent, remove all events and their listeners
		globalEventMap[entity_].clear();
	}
	else
	{
		// If events were sent, remove them with their listeners
		for (auto& event_ : eventNames_)
		{
			// Check if event exists
			auto iterator_ = globalEventMap[entity_].find(event_);
			if (iterator_ != globalEventMap[entity_].end())
				globalEventMap[entity_].erase(iterator_);
		}
	}
}

void EventEmitter::removeAllListeners (Entity entity_, std::string eventName_)
{
	std::vector<std::string> events_ {eventName_};

	removeAllListeners(entity_, events_);
}
*/

}	// namespace Zen
