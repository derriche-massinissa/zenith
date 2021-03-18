/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_EVENTS_EVENT_LISTENER_H
#define ZEN_EVENTS_EVENT_LISTENER_H

#include <string>
#include <functional>

namespace Zen {
namespace Events {

/**
 * The interface used for containers to access stored Listener elements.
 *
 * @class ListenerBase
 * @since 0.0.0
 */
class ListenerBase
{
protected:
	/**
	 * The event this listener is listening to.
	 *
	 * @since 0.0.0
	 */
	std::string event;

	/**
	 * A flag indicating if the listener is one timed. If true, it will be
	 * removed when activating only once.
	 *
	 * @since 0.0.0
	 */
	bool once;

public:
	ListenerBase (std::string event_, bool once_)
		: event (event_), once (once_)
	{}
};

/**
 * The listener is responsible for holding a callback function, and activate
 * it when the event listened to is emitted.
 *
 * @class Listener
 * @since 0.0.0
 */
template <typename... Args>
class Listener : public ListenerBase
{
private:
	/**
	 * The callback function to use if the corresponding event is emitted.
	 *
	 * @since 0.0.0
	 */
	std::function<void(Args...)> callback;

public:
	/**
	 * @since 0.0.0
	 *
	 * @param event_ The event to listen for.
	 * @param cb_ The callback function to activate when an event happens.
	 * @param flag_ Whether this listener is one timed or not.
	 */
	Listener (std::string event_, std::function<void(Args...)> cb_, bool flag_)
		: callback(cb_), ListenerBase(event_, flag_)
	{}

	/**
	 * Make the listener activate it's callback function.
	 *
	 * @since 0.0.0
	 *
	 * @param args The parameters to pass to the callback function
	 *
	 * @return Listener::once member variable, `true` if one time listener, and
	 * needs to be removed, otherwise `false`.
	 */
	bool activate(Args... args_)
	{
		callback(args_...);

		return once;
	}

	/**
	 * Returns the callback functor of this Listener.
	 *
	 * @return The callback of this Listener.
	 */
	std::function<void(Args...)> getCallback ()
	{
		return callback;
	}
};

}	// namespace Events
}	// namespace Zen

#endif
