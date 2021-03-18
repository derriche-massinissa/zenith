/**
 * @file		event_emitter.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "event_emitter.h"

Zen::EventEmitter::EventEmitter ()
{}

Zen::EventEmitter::~EventEmitter ()
{}

Zen::Listener*
Zen::EventEmitter::addListener (
		std::string eventName,
		std::function<void(Zen::Data)> callback,
		bool flag
		)
{
	// Check if the event already has listeners (Exists in the event map)
	auto iterator = eventMap.find(eventName);

	if (iterator != eventMap.end()) {
		// Add the listener to this event
		iterator->second.emplace_back(eventName, callback, flag);
	} else {
		// Add the event to the map
		eventMap.emplace(eventName, std::vector<Listener> {Listener (eventName, callback, flag)});
		iterator = eventMap.find(eventName);
	}

	return &iterator->second.back();
}
/*
Zen::Listener*
Zen::EventEmitter::on (
		std::string eventName,
		std::function<void(Zen::Data)> callback
		)
{
	return addListener(eventName, callback, false);
}

Zen::Listener*
Zen::EventEmitter::once (
		std::string eventName,
		std::function<void(Zen::Data)> callback
		)
{
	return addListener(eventName, callback, true);
}
*/

bool
Zen::EventEmitter::emit (
		std::string eventName,
		Zen::Data event
		)
{
	// Check if there are listeners for this event
	bool foundEvent = false;
	auto iterator = eventMap.find(eventName);
	if (iterator != eventMap.end()) {
		// Activate the listeners
		for (auto l = iterator->second.begin(); l != iterator->second.end();) {
			if (l->activate(event)) {
				iterator->second.erase(l);
			} else {
				// ONLY INCREMENT IF NO ERASE OPERATION HAPPENED!!
				// ERASING NATURALLY MOVES LATER ELEMENTS TO THE LEFT!!!
				l++;
			}
		}
		foundEvent = true;
	}

	return foundEvent;
}

bool
Zen::EventEmitter::emit (
		std::string eventName
		)
{
	return emit(eventName, Data {});
}

std::vector<std::string>
Zen::EventEmitter::getEventNames ()
{
	std::vector<std::string> names;
	for (const auto& it : eventMap)
		names.emplace_back(it.first);

	return names;
}

int
Zen::EventEmitter::getListenerCount (
		std::string event
		)
{
	std::map<std::string, std::vector<Zen::Listener>>::iterator iterator = eventMap.find(event);
	size_t count = 0;

	if (iterator != eventMap.end())
		count = iterator->second.size();

	return count;
}

std::vector<Zen::Listener*>
Zen::EventEmitter::getListeners (
		std::string event
		)
{
	std::vector<Listener*> vec {};

	// Check if event exists
	auto iterator = eventMap.find(event);
	if (iterator != eventMap.end()) {
		for (int i = 0; i < iterator->second.size(); i++) {
			vec.emplace_back(&iterator->second.at(i));
		}
	}

	return vec;
}

void
Zen::EventEmitter::removeListener (
		std::string event,
		Zen::Listener* listener
		)
{
	// Check if event exists
	auto iterator = eventMap.find(event);
	if (iterator != eventMap.end()) {
		// Find listener
		for (int l = 0; l < iterator->second.size(); l++) {
			if (listener == &iterator->second.at(l)) {
				iterator->second.erase(iterator->second.begin() + l);
				break;
			}
		}
	}
}

void
Zen::EventEmitter::removeAllListeners (
		std::vector<std::string> eventNames
		)
{
	// Check if events were sent
	if (eventNames.empty()) {
		// If no event was sent, remove all events and their listeners
		eventMap.clear();
	} else {
		// If events were sent, remove them with their listeners
		for (auto event : eventNames) {
			// Check if event exists
			auto iterator = eventMap.find(event);
			if (iterator != eventMap.end())
				eventMap.erase(iterator);
		}
	}
}

void
Zen::EventEmitter::shutdown ()
{
	removeAllListeners();
}
