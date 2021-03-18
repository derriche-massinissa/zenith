/**
 * @file		event_emitter.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "event_emitter.h"

namespace Zen {
namespace Events {

std::vector<std::string> EventEmitter::getEventNames ()
{
	std::vector<std::string> names;

	for (const auto& it : eventMap)
		names.emplace_back(it.first);

	return names;
}

int EventEmitter::getListenerCount (std::string event)
{
	auto iterator = eventMap.find(event);
	int count = 0;

	if (iterator != eventMap.end())
		count = iterator->second.size();

	return count;
}

std::vector<ListenerBase*> EventEmitter::getListeners (std::string event)
{
	std::vector<ListenerBase*> vec {};

	// Check if event exists
	auto iterator = eventMap.find(event);
	if (iterator != eventMap.end())
	{
		for (int i = 0; i < iterator->second.size(); i++)
			vec.emplace_back(iterator->second[i].get());
	}

	return vec;
}

void EventEmitter::removeListener (std::string event, ListenerBase* listener)
{
	// Check if event exists
	auto iterator = eventMap.find(event);
	if (iterator != eventMap.end())
	{
		// Find listener
		for (int l = 0; l < iterator->second.size(); l++)
		{
			if (listener == iterator->second[l].get())
			{
				iterator->second.erase(iterator->second.begin() + l);
				break;
			}
		}
	}
}

void EventEmitter::removeAllListeners (std::vector<std::string> eventNames)
{
	// Check if events were sent
	if (eventNames.empty())
	{
		// If no event was sent, remove all events and their listeners
		eventMap.clear();
	}
	else
	{
		// If events were sent, remove them with their listeners
		for (auto event : eventNames)
		{
			// Check if event exists
			auto iterator = eventMap.find(event);
			if (iterator != eventMap.end())
				eventMap.erase(iterator);
		}
	}
}

void EventEmitter::shutdown ()
{
	removeAllListeners();
}

}	// namespace Events
}	// namespace Zen
