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
#include <memory>

#include "../messages.h"
#include "../data.h"
#include "event_listener.h"

namespace Zen {
namespace Events {

/**
 * The event emitter is responsible for emitting, listening to and managing events in the whole game. Every event activity goes through it.
 *
 * A tipical use for this EventEmitter looks like this:
 *
 * ```c++
 * void function (int a, double b)
 * {
 *     std::cout << a * b << std::endl;	
 * }
 *
 * // ...
 *
 * events.on("game_step", &MyScene::function, this);
 *
 * events.emit("game_step", 7, 4.9);
 * ```
 *
 * Make sure that callback functions always have a return type of `void`!
 *
 * If you add multiple listeners for the same event, for example:
 * ```c++
 * events.on("game_step", &MyScene::function1, this);
 * events.on("game_step", &MyScene::function2, this);
 * events.on("game_step", &MyScene::function3, this);
 * ```
 * Be sure that all the functions take the same number and type of arguments!
 * Because when emitting an event, the given parameters are passed to all the 
 * listeners.
 *
 * @class EventEmitter
 * @since 0.0.0
 */
class EventEmitter
{
public:
	/**
	 * Add a listener for a given event.
	 *
	 * @since 0.0.0
	 *
	 * @param eventName - The event name.
	 * @param callback - The listener callback function.
	 * @param once If `true`, makes the listener one timed.
	 */
	template <typename... Args>
	void addListener (
			std::string eventName,
			void (T::* callback)(Args...),
			T *context,
			bool once
			)
	{
		// Bind function to the given context
		std::function<void(Args...)> boundCB = [context, callback]
			(Args... args) -> void
			{
				(context->*callback)(args...);
			}

		// Check if the event already has listeners (Exists in the event map)
		auto iterator = eventMap.find(eventName);

		if (iterator != eventMap.end())
		{
			// Add the listener to this event
			iterator->second.emplace_back(
					std::make_unique<Listener<Args...>> (eventName, boundCB, once)
					);
		}
		else
		{
			// Add the event to the event map
			eventMap.emplace(eventName, std::vector<ListenerBase> {
					std::make_unique<Listener<Args...>> (eventName, boundCB, once)
					});
		}
	}

	/**
	 * Add a listener for a given event.
	 *
	 * @since 0.0.0
	 *
	 * @param eventName The event name.
	 * @param callback The listener callback function.
	 * @param context The context to invoke the listener with.
	 */
	template <typename T, typename... Args>
	void on (std::string eventName, void (T::* callback)(Args...), T *context)
	{
		// Add the listener to this EventEmitter
		addListener(eventName, callback, context, false);
	}

	/**
	 * Add a one-time listener for a given event.
	 *
	 * @since 0.0.0
	 *
	 * @param eventName The event name.
	 * @param callback The listener callback function.
	 * @param context The context to invoke the listener with.
	 */
	template <typename T>
	void once (std::string eventName, void (T::* callback)(Args...), T *context)
	{
		// Add the listener to this EventEmitter
		addListener(eventName, callback, context, true);
	}

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
	template <typename... Args>
	bool emit (std::string eventName, Args&&... args)
	{
		// Check if there are listeners for this event
		bool foundEvent = false;
		auto iterator = eventMap.find(eventName);

		if (iterator != eventMap.end())
		{
			// Activate the listeners
			for (auto l = iterator->second.begin(); l != iterator->second.end();)
			{
				if ( static_cast<Listener<Args...>*>(l.get())->activate(args...) )
				{
					iterator->second.erase(l);
				}
				else
				{
					// ONLY INCREMENT IF NO ERASE OPERATION HAPPENED!!
					// ERASING NATURALLY MOVES LATER ELEMENTS TO THE LEFT!!!
					l++;
				}
			}

			foundEvent = true;
		}

		return foundEvent;
	}

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
	std::vector<ListenerBase*> getListeners (std::string event);

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
	template <typename T, typename... Args>
	void off (std::string eventName, void (T::* function)(Args...), T *context)
	{
		// Check if the event exists
		auto it = eventMap.find(eventName);
		if (it != eventMap.end())
		{
			// Bind the function to the given context
			std::function<void(Args...)> boundFunc = [context, function]
				(Args... args) -> void
				{
					(context->*function)(args...);
				}

			// Get a pointer to the target function
			auto target = boundFunc.target<void(*)(Args...)>();

			// Find Listener by comparing functor targets
			for (auto l = it->second.begin(); l != it->second.end(); l++)
			{
				auto listenerTarget = l->getCallback().target<void(*)(Args...)>();
				if (target == listenerTarget)
				{
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
	std::map<std::string, std::vector<std::unique_ptr<ListenerBase>>> eventMap;
};

}	// namespace Events
}	// namespace Zen

#endif
