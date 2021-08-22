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
#include <deque>
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
		/* Check if the entity is already listened to */ \
		auto itEnt_ = eventMap.find(entity_); \
		if ( itEnt_ != eventMap.end() ) \
		{ \
			/* Check if the event is already listened to */ \
			auto it_ = eventMap[entity_].find(eventName_); \
			if ( it_ != eventMap[entity_].end() ) \
			{ \
				/* Try to cast the first listener to this signature */ \
				auto l_ = dynamic_cast<Listener<Args...>*>(it_->second.at(0).get()); \
				if (l_ == nullptr) \
				{ \
					MessageError("Added an incompatible listener to an already \
							existing event of different signature!"); \
					if (entity_ != entt::null) \
						MessageError("Entity is: ", static_cast<std::uint64_t>(entity_)); \
					MessageError("Event is: ", eventName_); \
					MessageError("Function signature is: ", \
						PRETTY_TYPE<decltype(callback_)>()); \
					MessageError("Parameter types are: "); \
					((std::cout << " - " << PRETTY_TYPE<Args>() << std::endl), ...); \
				} \
			} \
		} \
	} while (false)
#else
#define TEST_EVENT() do {} while (false)
#endif

namespace Zen {

/**
 * The event emitter is responsible for emitting, listening to and managing
 * events.
 *
 * You can add four types of callbacks.
 * 1. Member functions:
 * ```c++
 * void MyScene::function (int a, double b)
 * {
 *     std::cout << a * b << std::endl;
 * }
 *
 * // ...
 *
 * MyScene scene;
 * events.on("game_step", &MyScene::function, scene);
 * events.emit("game_step", 7, 4.9);
 * ```
 * 2. Free functions:
 * ```cpp
 * void function ()
 * {
 *     std::cout << "Hello World!" << std::endl;
 * }
 *
 * // ...
 *
 * events.on.("game_step", function);
 * events.emit("game_step");
 * ```
 * 3. Lambda functions:
 * ```cpp
 * events.on.("game_step", [] (int x) {
 *     std::cout << (x * 7) << std::endl;
 * });
 * events.emit("game_step", 47);
 * ```
 * 4. Functors:
 * ```cpp
 * std::function<void()> func = freeFunction;
 * events.on("game_step", func);
 * events.emit("game_step");
 * ```
 *
 * Make sure that callback functions always have a return type of `void`!
 *
 * If you add multiple listeners for the same event, for example:
 * ```c++
 * events.on("game_step", &MyScene::function1, this);
 * events.on("game_step", function2);
 * events.on("game_step", [] () -> void { return; });
 * ```
 * Be sure that all the functions have the same number and type of parameters!
 * Because when emitting an event, the given arguments are passed to _all_ of
 * the listeners.
 *
 * You can also specify a scope, to only use the event with the given entity.
 * ```cpp
 * Entity player = newCharacter();
 * Entity enemy = newCharacter();
 *
 * events.on(player, "levelup", [] (int level) {
 *     std::cout << "Congrats!" << std::endl;
 * });
 * events.on(enemy, "levelup", [] (int level) {
 *     std::cout << "Oh no..." << std::endl;
 * });
 *
 * events.emit("levelup", 76);				// Nothing happens
 * events.emit(player, "levelup", 76);		// "Congrats!"
 * events.emit(enemy, "levelup", 76);		// "Oh no..."
 * ```
 *
 * @class EventEmitter
 * @since 0.0.0
 */
class EventEmitter
{
private:
	/**
	 * A map of entities, event names and all of their listeners.
	 *
	 * Each event name is unique in the list. If a listener is added for an
	 * event that already exists in the list, it is added to the list of
	 * listeners of said event name.
	 *
	 * *NOTE*: Changed the vector to deque because the callback list can change
	 * during the execution of an event, which makes the address of the unique
	 * pointer to the listener invalid.
	 *
	 * @since 0.0.0
	 */
	std::map<
		Entity,
		std::map<
			std::string,
			std::deque<std::shared_ptr<ListenerBase>>
		>
	> eventMap;

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
			Entity entity_,
			std::string eventName_,
			const std::function<void(Args...)>& callback_
			)
	{
		TEST_EVENT();

		eventMap[entity_][eventName_].emplace_back(
				std::make_unique<Listener<Args...>> (eventName_, callback_, once_, entity_)
				);

		return eventMap[entity_][eventName_].back().get();
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
	auto addListener (
			bool once_,
			Entity entity_,
			std::string eventName_,
			const Lambda& callback_
			)
	{
		typename lambda_to_functor<Lambda>::type functor_ = callback_;

		return addListener(once_, entity_, eventName_, functor_);
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
	auto addListener (
			bool once_,
			Entity entity_,
			std::string eventName_,
			const Lambda&& callback_
			)
	{
		typename lambda_to_functor<Lambda>::type functor_ = callback_;

		return addListener(once_, entity_, eventName_, functor_);
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
	auto addListener (
			bool once_,
			Entity entity_,
			std::string eventName_,
			void (*callback_)(Args...)
			)
	{
		TEST_EVENT();

		std::function<void(Args...)> functor_ = callback_;

		eventMap[entity_][eventName_].emplace_back(
				std::make_unique<Listener<Args...>> (eventName_, functor_, once_, entity_)
				);

		return eventMap[entity_][eventName_].back().get();
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
			Entity entity_,
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
		eventMap[entity_][eventName_].emplace_back(
				std::make_unique<Listener<Args...>> (eventName_, boundCB_, once_, entity_)
				);

		return eventMap[entity_][eventName_].back().get();
	}

	/**
	 * Add a listener for a given event, for the given entity.
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
	auto on (Entity entity_, std::string eventName_, Args&&... args_)
	{
		// Add the listener to this EventEmitter
		return addListener(false, entity_, eventName_, std::forward<Args>(args_)...);
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
	auto on (std::string eventName_, Args&&... args_)
	{
		// Add the listener to this EventEmitter
		return addListener(false, entt::null, eventName_, std::forward<Args>(args_)...);
	}

	/**
	 * Add a one-time listener for a given event, for the given entity.
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
	auto once (Entity entity_, std::string eventName_, Args&&... args_)
	{
		// Add the listener to this EventEmitter
		return addListener(true, entity_, eventName_, std::forward<Args>(args_)...);
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
	auto once (std::string eventName_, Args&&... args_)
	{
		// Add the listener to this EventEmitter
		return addListener(true, entt::null, eventName_, std::forward<Args>(args_)...);
	}

	/**
	 * Calls each of the listeners registered for a given event, for the given
	 * entity.
	 *
	 * @since 0.0.0
	 *
	 * @param eventName_ The event name.
	 * @param args_ The arguments to pass to the listener function.
	 *
	 * @return `true` if the event had listeners, else `false`.
	 */
	template <typename... Args>
	bool emit (Entity entity_, std::string eventName_, Args&&... args_)
	{
		// Check if there are listeners for this entity
		auto iteratorEnt_ = eventMap.find(entity_);
		if (iteratorEnt_ == eventMap.end())
			return false;

		// Check if there are listeners for this event
		auto iterator_ = eventMap[entity_].find(eventName_);
		if (iterator_ == eventMap[entity_].end())
			return false;

		// Create a copy of the callbacks list to make it safe to modify the
		// original list
		std::deque<std::shared_ptr<ListenerBase>> callbacks = iterator_->second;

		// Iterator over the original list
		auto ol_ = iterator_->second.begin();

		// Activate the listeners
		for (auto l_ = callbacks.begin(); l_ != callbacks.end(); l_++)
		{
			static_cast<Listener<Args...>*>(l_->get())->callback(args_...);

			if (l_->get()->once)
			{
				iterator_->second.erase(ol_);
			}
			else
			{
				// ONLY INCREMENT IF NO ERASE OPERATION HAPPENED!!
				// ERASING NATURALLY MOVES LATER ELEMENTS HIGHER!!!
				ol_++;
			}
		}

		// Have the map been emptied during the event emission?
		if (!eventMap.empty() &&
			eventMap.contains(entity_) &&
			eventMap[entity_].contains(eventName_) )
		{
			// Remove the event if no more listeners remain
			if (iterator_->second.empty())
				eventMap[entity_].erase(iterator_);

			// Remove the entity if no more events remain
			if (iteratorEnt_->second.empty())
				eventMap.erase(iteratorEnt_);
		}

		return true;
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
		// Cast `entt::null` to `Entity` to remove ambiguity (Otherwise it could
		// be converted to std::string)
		return emit((Entity)entt::null, eventName_, std::forward<Args>(args_)...);
	}

	/**
	 * Return a vector listing the events for which the emitter has
	 * registered listeners.
	 *
	 * @since 0.0.0
	 *
	 * @return List of events.
	 */
	std::vector<std::string> getEventNames (Entity entity = entt::null);

	/**
	 * Return the number of listeners listening to a given event, for the given
	 * entity.
	 *
	 * @since 0.0.0
	 *
	 * @param event_ The event name.
	 *
	 * @return The number of listeners.
	 */
	int getListenerCount (Entity entity, std::string event);

	/**
	 * Return the number of listeners listening to a given event.
	 *
	 * @since 0.0.0
	 *
	 * @param event_ The event name.
	 *
	 * @return The number of listeners.
	 */
	int getListenerCount (std::string event);

	/**
	 * Return the listeners registered for a given event, for the given entity.
	 *
	 * @since 0.0.0
	 *
	 * @param event_ The event name.
	 *
	 * @return A vector of references to the registered listeners.
	 */
	std::vector<ListenerBase*> getListeners (Entity entity, std::string event);

	/**
	 * Return the listeners registered for a given event.
	 *
	 * @since 0.0.0
	 *
	 * @param event_ The event name.
	 *
	 * @return A vector of references to the registered listeners.
	 */
	std::vector<ListenerBase*> getListeners (std::string event);

	/**
	 * Remove a listener.
	 *
	 * @since 0.0.0
	 *
	 * @param listener_ A pointer to the listener to remove.
	 */
	void removeListener (ListenerBase* listener);

	/**
	 * Remove a listener.
	 *
	 * @since 0.0.0
	 *
	 * @param listener_ A pointer to the listener to remove.
	 */
	void off (ListenerBase* listener);

	/**
	 * Remove all listeners, or those of the specified events, for the given
	 * entity.
	 *
	 * @since 0.0.0
	 *
	 * @param eventNames_ A vector of event names.
	 */
	void removeAllListeners (Entity entity, std::vector<std::string> eventNames = {});

	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param eventNames_ A vector of event names.
	 */
	void removeAllListeners (Entity entity, std::string eventName);

	/**
	 * Remove all listeners, or those of the specified events.
	 *
	 * @since 0.0.0
	 *
	 * @param eventNames_ A vector of event names.
	 */
	void removeAllListeners (std::vector<std::string> eventNames = {});

	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param eventNames_ A vector of event names.
	 */
	void removeAllListeners (std::string eventName);

	/**
	 * Removes __ALL__ the listeners from __ALL__ the events for every entity
	 * registered in this EventEmitter.
	 *
	 * @since 0.0.0
	 */
	void clear ();
};

}	// namespace Zen

#endif
