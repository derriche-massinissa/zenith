/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "mouse_manager.hpp"

#include <SDL2/SDL_mouse.h>
#include "../../window/window.hpp"
#include "../input_manager.hpp"

namespace Zen {

extern Window g_window;
extern InputManager g_input;

void MouseManager::boot ()
{
	enabled = true;
}

void MouseManager::lockPointer ()
{
	locked = true;

	// Hide pointer
	SDL_ShowCursor(0);

	// Center pointer
	SDL_WarpMouseInWindow(g_window.window, g_window.width() / 2, g_window.height() / 2);
}

void MouseManager::releasePointer ()
{
	locked = false;

	// Show pointer
	SDL_ShowCursor(1);
}

void MouseManager::onMouseMove (SDL_Event * const event_)
{
	tempEvent.reset();

	// Relative to window
	tempEvent.x = event_->motion.x;
	// Relative to window
	tempEvent.y = event_->motion.y;
	// Delta (Relative to last x)
	tempEvent.deltaX = event_->motion.xrel;
	// Delta (Relative to last y)
	tempEvent.deltaY = event_->motion.yrel;

	g_input.onMouseMove(tempEvent);
}

void MouseManager::onMouseUp (SDL_Event * const event_)
{
	tempEvent.reset();

	// Relative to window
	tempEvent.x = event_->button.x;

	// Relative to window
	tempEvent.y = event_->button.y;

	// 1 (single-click), 2 (double-click)...
	tempEvent.clicks = event_->button.clicks;

	tempEvent.timestamp = event_->button.timestamp;

	g_input.onMouseUp(tempEvent);
}

void MouseManager::onMouseDown (SDL_Event * const event_)
{
	tempEvent.reset();

	// Relative to window
	tempEvent.x = event_->button.x;

	// Relative to window
	tempEvent.y = event_->button.y;

	// 1 (single-click), 2 (double-click)...
	tempEvent.clicks = event_->button.clicks;

	tempEvent.timestamp = event_->button.timestamp;

	g_input.onMouseDown(tempEvent);
}

/*
void MouseManager::onMouseOver ()
{
	tempEvent.reset();

	tempEvent.timestamp = SDL_GetTicks();

	g_input.setWindowOver(tempEvent);
}

void MouseManager::onMouseOut ()
{
	tempEvent.reset();

	tempEvent.timestamp = SDL_GetTicks();

	g_input.setWindowOut(tempEvent);
}
*/

void MouseManager::onMouseWheel (SDL_Event * const event_)
{
	tempEvent.reset();

	// Horizontal(???) scroll ([+] right : [-] left)
	tempEvent.x = event_->button.x;
	// Vertical scroll ([+] up : [-] down)
	tempEvent.y = event_->button.y;

	g_input.onMouseWheel(tempEvent);
}

void MouseManager::onPointerLockChange ()
{
	g_input.onPointerLockChange(locked);
}

void MouseManager::update ()
{
	// Center pointer if locked
	if (locked)
	{
		SDL_WarpMouseInWindow(
			g_window.window,
			g_window.width() / 2,
			g_window.height() / 2
		);

		// TODO reset mouse pointer object in g_input too
	}
}

MouseManager g_mouse;

}	// namespace Zen
