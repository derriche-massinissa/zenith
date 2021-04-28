/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "keyboard_manager.hpp"
#include "../input_manager.hpp"

namespace Zen {

extern InputManager g_input;

void KeyboardManager::boot ()
{
	enabled = true;
}

void KeyboardManager::onKeyDown (SDL_Event * const event_)
{
	if (!enabled)
		return;

	g_input.events.emit("keydown", event_);
}

void KeyboardManager::onKeyUp (SDL_Event * const event_)
{
	if (!enabled)
		return;

	g_input.events.emit("keyup", event_);
}

}	// namespace Zen
