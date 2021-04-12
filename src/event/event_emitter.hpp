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
#include "../ecs/entity.hpp"
#include "../utils/assert.hpp"
#include "../utils/messages.hpp"
#include "../utils/pretty_type.hpp"
#include "../utils/lambda/lambda_to_functor.hpp"

// Macro to test if any added listener is compatible with the already added
// callbacks, by having the same signature
#ifndef NDEBUG
#define TEST_EVENT() \
	do \
	{ \
		/* Check if the event is already listened to */ \
		auto it_ = eventMap.find(eventName_); \
		if ( it_ != eventMap.end() ) \
		{ \
			/* Try to cast the first listener to this signature */ \
			auto l_ = dynamic_cast<Listener<Args...>*>(it_->second.at(0).get()); \
			if (l_ == nullptr) \
			{ \
				MessageError("Added an incompatible listener to an already existing event of different signature!"); \
				MessageError("Event is: ", eventName_); \
				MessageError("Function signature is: ", PRETTY_TYPE<decltype(callback_)>()); \
				MessageError("Parameter types are: "); \
				((std::cout << " - " << PRETTY_TYPE<Args>() << std::endl), ...); \
			} \
		} \
	} while (false)
#else
#define TEST_EVENT() do {} while (false)
#endif

#ifndef NDEBUG
#define TEST_EVENT_LAMBDA() \
	do \
	{ \
		/* Check if the event is already listened to */ \
		auto it_ = eventMap.find(eventName_); \
		if ( it_ != eventMap.end() ) \
		{ \
			/* Try to cast the first listener to this signature */ \
			auto l_ = dynamic_cast<decltype(functor_)*>(it_->second.at(0).get()); \
			if (l_ == nullptr) \
			{ \
				MessageError("Added an incompatible listener to an already existing event of different signature!"); \
				MessageError("Event is: ", eventName_); \
				MessageError("Function signature is: ", PRETTY_TYPE<decltype(callback_)>()); \
				MessageError("Callback function is a Lambda."); \
			} \
		} \
	} while (false)
#else
#define TEST_EVENT_LAMBDA() do {} while (false)
#endif

namespace Zen {

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
 * Because when emitting an event, the given parameters are passed to all _the_
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
	 * Add a listener for a given event, using a functor as a callback.
	 *
	 * @since 0.0.0
	 *
	 * @tparam Args The parameter types the functor takes.
	 *
	 * @param eventName_ The event name.
	 * @param callback_ The listener callback function.
	 * @param once_ If `true`, makes the listener one timed.
	 *
	 * @return A pointer to the created event listener.
	 */
	template <typename... Args>
	auto addListener (
			bool once_,
			std::string eventName_,
			const std::function<void(Args...)>& callback_
			)
	{
		TEST_EVENT();

		eventMap[eventName_].emplace_back(
				std::make_unique<Listener<Args...>> (eventName_, callback_, once_)
				);

		return eventMap[eventName_].back().get();
	}

	/**
	 * Add a listener for a given event, using a lambda function as a callback.
	 *
	 * This takes an L-Value reference.
	 *
	 * @since 0.0.0
	 *
	 * @tparam Lambda The type of the lambda callback function.
	 *
	 * @param eventName_ The event name.
	 * @param callback_ The listener callback function.
	 * @param once_ If `true`, makes the listener one timed.
	 *
	 * @return A pointer to the created event listener.
	 */
	template <typename Lambda>
	auto addListener (bool once_, std::string eventName_, const Lambda& callback_)
	{
		typename lambda_to_functor<Lambda>::type functor_ = callback_;

		TEST_EVENT_LAMBDA();

		return addListener(once_, eventName_, functor_);
	}

	/**
	 * Add a listener for a given event, using a lambda function as a callback.
	 *
	 * This takes an R-Value reference.
	 *
	 * @since 0.0.0
	 *
	 * @tparam Lambda The type of the lambda callback function.
	 *
	 * @param eventName_ The event name.
	 * @param callback_ The listener callback function.
	 * @param once_ If `true`, makes the listener one timed.
	 *
	 * @return A pointer to the created event listener.
	 */
	template <typename Lambda>
	auto addListener (bool once_, std::string eventName_, const Lambda&& callback_)
	{
		typename lambda_to_functor<Lambda>::type functor_ = callback_;

		TEST_EVENT_LAMBDA();

		return addListener(once_, eventName_, functor_);
	}

	/**
	 * Add a listener for a given event, using a free function as a callback.
	 *
	 * @since 0.0.0
	 *
	 * @tparam Args The parameter types the free function takes.
	 *
	 * @param eventName_ The event name.
	 * @param callback_ The listener callback function.
	 * @param once_ If `true`, makes the listener one timed.
	 *
	 * @return A pointer to the created event listener.
	 */
	template <typename... Args>
	auto addListener (bool once_, std::string eventName_, void (*callback_)(Args...))
	{
		TEST_EVENT();

		std::function<void(Args...)> functor_ = callback_;

		eventMap[eventName_].emplace_back(
				std::make_unique<Listener<Args...>> (eventName_, functor_, once_)
				);

		return eventMap[eventName_].back().get();
	}

	/**
	 * Add a listener for a given event, using a member function as a callback.
	 *
	 * @since 0.0.0
	 *
	 * @tparam T The type of the context of the member function.
	 * @tparam Args The parameter types the member function takes.
	 *
	 * @param eventName_ The event name.
	 * @param callback_ The listener callback function.
	 * @param context_ The context to invoke the listener with.
	 * @param once_ If `true`, makes the listener one timed.
	 *
	 * @return A pointer to the created event listener.
	 */
	template <typename T, typename... Args>
	auto addListener (
			bool once_,
			std::string eventName_,
			void (T::* callback_)(Args...),
			T *context_
			)
	{
		TEST_EVENT();

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

		return eventMap[eventName_].back().get();
	}

	/**
	 * Add a listener for a given event.
	 *
	 * @since 0.0.0
	 *
	 * @tparam Args The parameter types to pass to `addListener`.
	 *
	 * @param args The parameters to pass to `addListener`.
	 *
	 * @return A pointer to the created event listener.
	 */
	template <typename... Args>
	auto on (Args&&... args_)
	{
		// Add the listener to this EventEmitter
		return addListener(false, std::forward<Args>(args_)...);
	}

	/**
	 * Add a one-time listener for a given event.
	 *
	 * @since 0.0.0
	 *
	 * @tparam Args The parameter types to pass to `addListener`.
	 *
	 * @param args The parameters to pass to `addListener`.
	 *
	 * @return A pointer to the created event listener.
	 */
	template <typename... Args>
	auto once (Args&&... args_)
	{
		// Add the listener to this EventEmitter
		return addListener(true, std::forward<Args>(args_)...);
	}

	/**
	 * Calls each of the listeners registered for a given event.
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
				static_cast<Listener<Args...>*>(l_->get())->callback(args_...);

				if (l_->get()->once)
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

			// Remove the event if no more listeners remain
			if (iterator_->second.empty())
			{
				eventMap.erase(iterator_);
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
	 * Remove a listener.
	 *
	 * @since 0.0.0
	 *
	 * @param listener_ A pointer to the listener to remove.
	 */
	void removeListener (ListenerBase*& listener_);

	/**
	 * Remove a listener.
	 *
	 * @since 0.0.0
	 *
	 * @param listener_ A pointer to the listener to remove.
	 */
	void off (ListenerBase*& listener_);

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

	// GLOBAL VERSION METHODS
	// TODO Remove globals

	/*
	template <typename T, typename... Args>
	void addListener (Entity entity_, std::string eventName_, void (T::* callback_)(Args...), T *context_, bool once_)
	{
#ifndef NDEBUG
		// Check if the event is already listened to
		auto iter_ = globalEventMap.find(entity_);
		if ( iter_ != globalEventMap.end() )
		{
			auto iter2_ = globalEventMap[entity_].find(eventName_);
			if ( iter2_ != globalEventMap[entity_].end() )
			{
				auto l_ = dynamic_cast<Listener<Args...>*>(iter2_->second.at(0).get());

				if (l_ == nullptr)
				{
					MessageError("Added an incompatible listener to an already existing event of different signature!");

					MessageError("Event is: ", eventName_);

					MessageError("Function signature is: ", PRETTY_TYPE<decltype(callback_)>());

					MessageError("Parameter types are: ");
					((std::cout << " - " << PRETTY_TYPE<Args>() << std::endl), ...);
				}
			}
		}
#endif

		// Bind function to the given context
		std::function<void(Args...)> boundCB_ = [context_, callback_]
			(Args... args_) -> void
			{
				(context_->*callback_)(args_...);
			};

		// Add the event to the event map
		globalEventMap[entity_][eventName_].emplace_back(
				std::make_unique<Listener<Args...>> (eventName_, boundCB_, once_)
				);
	}

	template <typename T, typename... Args>
	void on (Entity entity_, std::string eventName_, void (T::* callback_)(Args...), T *context_)
	{
		// Add the listener to this EventEmitter
		addListener(entity_, eventName_, callback_, context_, false);
	}

	template <typename T, typename... Args>
	void once (Entity entity_, std::string eventName_, void (T::* callback_)(Args...), T *context_)
	{
		// Add the listener to this EventEmitter
		addListener(entity_, eventName_, callback_, context_, true);
	}

	template <typename... Args>
	bool emit (Entity entity_, std::string eventName_, Args&&... args_)
	{
		// Is there any entity listening to events?
		auto iterator_ = globalEventMap.find(entity_);
		if (iterator_ == globalEventMap.end())
			return false;

		// Is the entity listening to the given event?
		auto iterator2_ = globalEventMap[entity_].find(eventName_);
		if (iterator2_ == globalEventMap[entity_].end())
			return false;

		// Activate the listeners
		for (auto l_ = iterator2_->second.begin(); l_ != iterator2_->second.end();)
		{
			if ( static_cast<Listener<Args...>*>(l_->get())->activate(args_...) )
			{
				iterator2_->second.erase(l_);
			}
			else
			{
				// ONLY INCREMENT IF NO ERASE OPERATION HAPPENED!!
				// ERASING NATURALLY MOVES LATER ELEMENTS TO THE LEFT!!!
				l_++;
			}
		}

		// Remove the event if no more listeners remain
		if (iterator2_->second.empty())
		{
			globalEventMap[entity_].erase(iterator2_);
		}

		// Remove the entity if no more events
		if (iterator_->second.empty())
		{
			globalEventMap.erase(iterator_);
		}

		return true;
	}

	std::vector<std::string> getEventNames (Entity entity_);

	int getListenerCount (Entity entity_, std::string event_);

	std::vector<ListenerBase*> getListeners (Entity entity_, std::string event_);

	void removeListener (Entity entity_, std::string event_, ListenerBase* listener_);

	template <typename T, typename... Args>
	void off (Entity entity_, std::string eventName_, void (T::* function_)(Args...), T *context_)
	{
		// Check if the entity is listening
		auto it_ = globalEventMap.find(entity_);
		if (it_ == globalEventMap.end())
			return;

		// Check if the event exists
		auto it2_ = globalEventMap[entity_].find(eventName_);
		if (it2_ == globalEventMap[entity_].end())
			return;

		// Bind the function to the given context
		std::function<void(Args...)> boundFunc_ = [context_, function_]
			(Args... args_) -> void
			{
				(context_->*function_)(args_...);
			};

		// Get a pointer to the target function
		auto target_ = boundFunc_.template target<void(*)(Args...)>();

		// Find Listener by comparing functor targets
		for (auto l_ = it2_->second.begin(); l_ != it2_->second.end(); l_++)
		{
			auto listenerTarget_ = static_cast<Listener<Args...>*>(l_->get())->getCallback().template target<void(*)(Args...)>();

			if (target_ == listenerTarget_)
			{
				it2_->second.erase(l_);
				break;
			}
		}
	}

	void removeAllListeners (Entity entity_, std::vector<std::string> eventNames_ = {});

	void removeAllListeners (Entity entity_, std::string eventName_);
	*/
};

}	// namespace Zen

#endif
