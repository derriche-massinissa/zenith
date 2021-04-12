/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_INPUT_MOUSE_MANAGER_HPP
#define ZEN_INPUT_MOUSE_MANAGER_HPP

#include "../types/event.hpp"
#include <SDL2/SDL_events.h>

namespace Zen {

class MouseManager
{
public:
	bool enabled = false;

	bool locked = false;

	InputEvent tempEvent;

	void boot ();

	void lockPointer ();

	void releasePointer ();

	void onMouseMove (SDL_Event * const event_);

	void onMouseUp (SDL_Event * const event_);

	void onMouseDown (SDL_Event * const event_);

	/*
	void onMouseOver ();

	void onMouseOut ();
	*/

	void onMouseWheel (SDL_Event * const event_);

	void onPointerLockChange ();

	void update ();
};

}	// namespace Zen

#endif
