/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "game.hpp"

#include "../window/window.hpp"
#include "../input/mouse/mouse_manager.hpp"
#include "../input/keyboard/keyboard_manager.hpp"

namespace Zen {

extern Window g_window;
extern MouseManager g_mouse;
extern KeyboardManager g_keyboard;

void Game::handleSDLEvents ()
{
	SDL_Event event_;

	while (SDL_PollEvent(&event_))
	{
		switch (event_.type)
		{
			// Misc
			case SDL_QUIT:
				shutdown();
				break;

			// Window
			case SDL_WINDOWEVENT:
				g_window.handleSDLEvents(event_);
				break;

			// Mouse
			case SDL_MOUSEMOTION:
				if (g_mouse.enabled)
					g_mouse.onMouseMove(&event_);
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (g_mouse.enabled)
					g_mouse.onMouseDown(&event_);
				break;

			case SDL_MOUSEBUTTONUP:
				if (g_mouse.enabled)
					g_mouse.onMouseUp(&event_);
				break;

			case SDL_MOUSEWHEEL:
				if (g_mouse.enabled)
					g_mouse.onMouseWheel(&event_);
				break;

			// Keyboard
			case SDL_KEYDOWN:
				if (g_keyboard.enabled)
					g_keyboard.onKeyDown(&event_);
				break;

			case SDL_KEYUP:
				if (g_keyboard.enabled)
					g_keyboard.onKeyUp(&event_);
				break;
		}
	}
}

}	// namespace Zen
