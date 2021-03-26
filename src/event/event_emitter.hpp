/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_EVENTS_EVENT_EMITTER_HPP
#define ZEN_EVENTS_EVENT_EMITTER_HPP

#include <functional>
#include <string>
#include <utility>
#include <vector>
#include <queue>
#include <map>
#include <memory>

#include "event_listener.hpp"

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
private:
	/**
	 * A map of event names and all of their listeners. 
	 *
	 * Each event name is unique in the list. If a listener is added for an event that already exists in the list, it is added to the list of listeners of said event name.
	 *
	 * @since 0.0.0
	 */
	std::map<std::string, std::vector<std::unique_ptr<ListenerBase>>> eventMap;

public:
	/**
	 * Add a listener for a given event.
	 *
	 * @since 0.0.0
	 *
	 * @param eventName_ The event name.
	 * @param callback_ The listener callback function.
	 * @param context_ The context to invoke the listener with.
	 * @param once_ If `true`, makes the listener one timed.
	 */
	template <typename T, typename... Args>
	void addListener (
			std::string eventName_,
			void (T::* callback_)(Args...),
			T *context_,
			bool once_
			)
	{
		// Bind function to the given context
		std::function<void(Args...)> boundCB_ = [context_, callback_]
			(Args... args_) -> void
			{
				(context_->*callback_)(args_...);
			};

		// Add the event to the event map
		eventMap[eventName_].emplace_back(
				std::make_unique<Listener<Args...>> (eventName_, boundCB_, once_)
				);

		/**
		 * @todo Delete this block in src/event/event_emitter.h
		// Check if the event already has listeners (Exists in the event map)
		auto iterator_ = eventMap.find(eventName_);

		if (iterator_ != eventMap.end())
		{
			// Add the listener to this event
			iterator_->second.emplace_back(
					std::make_unique<Listener<Args...>> (eventName_, boundCB_, once_)
					);
		}
		else
		{
			// Add the event to the event map
			eventMap.emplace(eventName_, std::vector<std::unique_ptr<ListenerBase>> {
					std::make_unique<Listener<Args...>> (eventName_, boundCB_, once_)
					});
		}
		*/
	}

	/**
	 * Add a listener for a given event.
	 *
	 * @since 0.0.0
	 *
	 * @param eventName_ The event name.
	 * @param callback_ The listener callback function.
	 * @param context_ The context to invoke the listener with.
	 */
	template <typename T, typename... Args>
	void on (std::string eventName_, void (T::* callback_)(Args...), T *context_)
	{
		// Add the listener to this EventEmitter
		addListener(eventName_, callback_, context_, false);
	}

	/**
	 * Add a one-time listener for a given event.
	 *
	 * @since 0.0.0
	 *
	 * @param eventName_ The event name.
	 * @param callback_ The listener callback function.
	 * @param context_ The context to invoke the listener with.
	 */
	template <typename T, typename... Args>
	void once (std::string eventName_, void (T::* callback_)(Args...), T *context_)
	{
		// Add the listener to this EventEmitter
		addListener(eventName_, callback_, context_, true);
	}

	/**
	 * Calls each of the listeners registered for a given event.
	 *
	 * Note that the event isn't executed immediatly, but added to the
	 * internal event queue to be polled in a future game step.
	 *
	 * @since 0.0.0
	 *
	 * @param eventName_ The event name.
	 * @param args_ The arguments to pass to the listener function.
	 *
	 * @return `true` if the event had listeners, else `false`.
	 */
	template <typename... Args>
	bool emit (std::string eventName_, Args&&... args_)
	{
		// Check if there are listeners for this event
		bool foundEvent_ = false;
		auto iterator_ = eventMap.find(eventName_);

		if (iterator_ != eventMap.end())
		{
			// Activate the listeners
			for (auto l_ = iterator_->second.begin(); l_ != iterator_->second.end();)
			{
				if ( static_cast<Listener<Args...>*>(l_->get())->activate(args_...) )
				{
					iterator_->second.erase(l_);
				}
				else
				{
					// ONLY INCREMENT IF NO ERASE OPERATION HAPPENED!!
					// ERASING NATURALLY MOVES LATER ELEMENTS TO THE LEFT!!!
					l_++;
				}
			}

			foundEvent_ = true;
		}

		return foundEvent_;
	}

	/**
	 * Return a vector listing the events for which the emitter has
	 * registered listeners.
	 *
	 * @since 0.0.0
	 *
	 * @return List of events.
	 */
	std::vector<std::string> getEventNames ();

	/**
	 * Return the number of listeners listening to a given event.
	 *
	 * @since 0.0.0
	 *
	 * @param event_ The event name.
	 *
	 * @return The number of listeners.
	 */
	int getListenerCount (std::string event_);

	/**
	 * Return the listeners registered for a given event.
	 *
	 * @since 0.0.0
	 *
	 * @param event_ The event name.
	 *
	 * @return A vector of references to the registered listeners.
	 */
	std::vector<ListenerBase*> getListeners (std::string event_);

	/**
	 * Remove a listener from a given event.
	 *
	 * @since 0.0.0
	 *
	 * @param event_ The event name.
	 *
	 * @param listener_ Only remove this particular listener.
	 */
	void removeListener (std::string event_, ListenerBase* listener_);

	/**
	 * Remove the listeners of a given event.
	 *
	 * This method takes a method and a context, unlike the removeListener()
	 * method that takes a pointer to a listener.
	 *
	 * @since 0.0.0
	 *
	 * @param event_ The event name.
	 * @param function_ Only remove the listener that has this function as a callback.
	 * @param context_ Only remove the listener that has this context as a callback context.
	 */
	template <typename T, typename... Args>
	void off (std::string eventName_, void (T::* function_)(Args...), T *context_)
	{
		// Check if the event exists
		auto it_ = eventMap.find(eventName_);
		if (it_ != eventMap.end())
		{
			// Bind the function to the given context
			std::function<void(Args...)> boundFunc_ = [context_, function_]
				(Args... args_) -> void
				{
					(context_->*function_)(args_...);
				};

			// Get a pointer to the target function
			auto target_ = boundFunc_.template target<void(*)(Args...)>();

			// Find Listener by comparing functor targets
			for (auto l_ = it_->second.begin(); l_ != it_->second.end(); l_++)
			{
				auto listenerTarget_ = static_cast<Listener<Args...>*>(l_->get())->getCallback().template target<void(*)(Args...)>();

				if (target_ == listenerTarget_)
				{
					it_->second.erase(l_);
					break;
				}
			}
		}
	}

	/**
	 * Remove all listeners, or those of the specified events.
	 *
	 * @since 0.0.0
	 *
	 * @param eventNames_ A vector of event names.
	 */
	void removeAllListeners (std::vector<std::string> eventNames_ = {});

	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param eventNames_ A vector of event names.
	 */
	void removeAllListeners (std::string eventName_);

	/**
	 * Removes all listeners.
	 *
	 * @since 0.0.0
	 */
	void shutdown ();
};

}	// namespace Events
}	// namespace Zen

#endif
