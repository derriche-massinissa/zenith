/**
 * @file		event_listener.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include <string>
#include "../data.h"
#include <functional>

namespace Zen
{
	class Listener
	{
	public:
		Listener(std::string name, std::function<void(Data)> cb, bool flag);
		~Listener();

		/**
		 * Make the listener activate it's callback function.
		 *
		 * @since 0.0.0
		 * @param e A data object to pass to the callback.
		 * @return `once` member variable, `true` if one time listener, and
		 * needs to be removed, otherwise `false`.
		 */
		bool activate(Data e);

		std::function<void(Data)> getCallback ();

	private:
		/**
		 * A flag indicating if the listener is one timed. If true, it will be
		 * removed when activating only once.
		 *
		 * @since 0.0.0
		 */
		bool once;

		/**
		 * The event this listener is listening to.
		 *
		 * @since 0.0.0
		 */
		std::string eventName;

		/**
		 * The callback function to use if the corresponding event is emitted.
		 *
		 * @since 0.0.0
		 */
		std::function<void(Data)> callback;
	};
}

#endif
