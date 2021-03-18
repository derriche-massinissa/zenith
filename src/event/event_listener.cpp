/**
 * @file		event_listener.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "event_listener.h"
#include <functional>

Zen::Listener::Listener (
	std::string name,
	std::function<void(Data)> cb,
	bool flag)
{
	eventName = name;
	callback = cb;
	once = flag;
}

Zen::Listener::~Listener ()
{}

bool
Zen::Listener::activate (
	Zen::Data e
	)
{
	callback(e);
	return once;
}

std::function<void(Data)>
Zen::Listener::getCallback ()
{
	return callback;
}
