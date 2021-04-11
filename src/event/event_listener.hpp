/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_EVENTS_EVENT_LISTENER_HPP
#define ZEN_EVENTS_EVENT_LISTENER_HPP

#include <string>
#include <functional>

namespace Zen {

/**
 * The interface used for containers to access stored Listener elements.
 *
 * @class ListenerBase
 * @since 0.0.0
 */
class ListenerBase
{
public:
	ListenerBase (std::string event_, bool once_)
		: event (event_), once (once_)
	{}

	virtual ~ListenerBase () = default;

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
public:
	/**
	 * @since 0.0.0
	 *
	 * @param event_ The event to listen for.
	 * @param cb_ The callback function to activate when an event happens.
	 * @param flag_ Whether this listener is one timed or not.
	 */
	Listener (std::string event_, std::function<void(Args...)> cb_, bool flag_)
		: ListenerBase(event_, flag_)
		, callback(cb_)
	{}

	/**
	 * The callback function to use if the corresponding event is emitted.
	 *
	 * @since 0.0.0
	 */
	std::function<void(Args...)> callback;
};

}	// namespace Zen

#endif
