/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_INPUT_KEYBOARD_MANAGER_HPP
#define ZEN_INPUT_KEYBOARD_MANAGER_HPP

#include <SDL2/SDL_events.h>
#include <vector>

namespace Zen {

class KeyboardManager
{
public:
	void boot ();

	void onKeyDown (SDL_Event * const event_);

	void onKeyUp (SDL_Event * const event_);

	bool enabled = false;
};

}	// namespace Zen

#endif
