/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "event_emitter.h"

namespace Zen {
namespace Events {

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
		for (int i_ = 0; i_ < iterator_->second.size(); i_++)
			vec_.emplace_back(iterator_->second[i_].get());
	}

	return vec_;
}

void EventEmitter::removeListener (std::string event_, ListenerBase* listener_)
{
	// Check if event exists
	auto iterator_ = eventMap.find(event_);
	if (iterator_ != eventMap.end())
	{
		// Find listener
		for (int l_ = 0; l_ < iterator_->second.size(); l_++)
		{
			if (listener_ == iterator_->second[l_].get())
			{
				iterator_->second.erase(iterator_->second.begin() + l_);
				break;
			}
		}
	}
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
	removeAllListeners();
}

}	// namespace Events
}	// namespace Zen
