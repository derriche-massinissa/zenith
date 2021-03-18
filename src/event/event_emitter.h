/**
 * @file		event_emitter.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef EVENT_EMITTER_H
#define EVENT_EMITTER_H

#include <functional>
#include <string>
#include <utility>
#include <vector>
#include <queue>
#include <map>

#include "../messages.h"
#include "../data.h"
#include "event_listener.h"

namespace Zen {
	/**
	 * @class EventEmitter
	 *
	 * The event emitter is responsible for emitting, listening to and managing events in the whole game. Every event activity goes through it.
	 *
	 * @since 0.0.0
	 */
	class EventEmitter
	{
	public:
		EventEmitter();
		~EventEmitter();

		/**
		 * Add a listener for a given event.
		 *
		 * @since 0.0.0
		 * @param eventName - The event name.
		 * @param callback - The listener callback function.
		 * @param once If `true`, makes the listener one timed.
		 * @return A reference to the event listener.
		 */
		Listener* addListener (
				std::string eventName,
				std::function<void(Data)> callback,
				bool once
				);

		/**
		 * Add a listener for a given event.
		 *
		 * @since 0.0.0
		 *
		 * @param eventName The event name.
		 * @param callback The listener callback function.
		 * @param context The context to invoke the listener with.
		 *
		 * @return A reference to the event listener.
		 */
		template <typename T>
		Listener* on (
			std::string eventName,
			void (T::* callback)(Data)
			T *context
			)
		{
			return addListener(eventName, std::bind(callback, context, std::placeholders::_1), false);
		}
		/*Listener* on (
				std::string eventName,
				std::function<void(Data)> callback
				);*/

		/**
		 * Add a one-time listener for a given event.
		 *
		 * @since 0.0.0
		 *
		 * @param eventName The event name.
		 * @param callback The listener callback function.
		 * @param context The context to invoke the listener with.
		 *
		 * @return A reference to the event listener.
		 */
		template <typename T>
		Listener* once (
			std::string eventName,
			void (T::* callback)(Data)
			T *context
			)
		{
			return addListener(eventName, std::bind(callback, context, std::placeholders::_1), true);
		}
		/*Listener* once (
				std::string eventName,
				std::function<void(Data)> callback
				);*/

		/**
		 * Calls each of the listeners registered for a given event.
		 *
		 * Note that the event isn't executed immediatly, but added to the
		 * internal event queue to be polled in a future game step.
		 *
		 * @since 0.0.0
		 * @param eventName - The event name.
		 * @param event - The event data.
		 * @return `true` if the event had listeners, else `false`.
		 */
		bool emit (std::string eventName, Data event);

		/**
		 * @overload
		 *
		 * @since 0.0.0
		 * @param eventName - The event name.
		 * @return `true` if the event had listeners, else `false`.
		 */
		bool emit (std::string eventName);

		/**
		 * Return a vector listing the events for which the emitter has
		 * registered listeners.
		 *
		 * @since 0.0.0
		 * @return List of events.
		 */
		std::vector<std::string> getEventNames ();

		/**
		 * Return the number of listeners listening to a given event.
		 *
		 * @since 0.0.0
		 * @param event - The event name.
		 * @return The number of listeners.
		 */
		int getListenerCount (std::string event);

		/**
		 * Return the listeners registered for a given event.
		 *
		 * @since 0.0.0
		 * @param event - The event name.
		 * @return A vector of references to the registered listeners.
		 */
		std::vector<Listener*> getListeners (std::string event);

		/**
		 * Remove a listener from a given event.
		 *
		 * @since 0.0.0
		 * @param event - The event name.
		 * @param listener - Only remove this particular listener.
		 */
		void removeListener (std::string event, Listener* listener);

		/**
		 * Remove the listeners of a given event.
		 *
		 * This method takes a method and a context, unlike the `removeListener()` method that takes a pointer to a listener.
		 *
		 * @since 0.0.0
		 * @param event - The event name.
		 * @param function - Only remove the listener that has this function as a callback.
		 * @param context - Only remove the listener that has this context as a callback context.
		 */
		template <typename T>
		void off (std::string event, void (T::* func)(Data), T *context)
		{
			// Bind the function to the given context
			std::function<void(Data)> function = std::bind(func, context, std::placeholders::_1);

			// Get a pointer to the target function
			auto target = function.target<void(*)(Data)>();

			// Check if the event exists
			auto it = eventMap.find(event);
			if (it != eventMap.end()) {
				// Find Listener by comparing functor targets
				for (auto l = it->second.begin(); l != it->second.end(); l++) {
					auto listenerTarget = l->getCallback().target<void(*)(Data)>();
					if (target == listenerTarget) {
						it->second.erase(l);
						break;
					}
				}
			}
		}

		/**
		 * Remove all listeners, or those of the specified events.
		 *
		 * @since 0.0.0
		 * @param eventNames - A vector of event names.
		 */
		void removeAllListeners (std::vector<std::string> eventNames = {});

		/**
		 * Removes all listeners.
		 *
		 * @since 0.0.0
		 */
		void shutdown ();

	private:
		/**
		 * A map of event names and all of their listeners. 
		 *
		 * Each event name is unique in the list. If a listener is added for an event that already exists in the list, it is added to the list of listeners of said event name.
		 *
		 * @since 0.0.0
		 */
		std::map<std::string, std::vector<Listener>> eventMap;
	};
}
#endif
