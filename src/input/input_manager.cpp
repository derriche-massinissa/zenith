/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "input_manager.hpp"

namespace Zen {

void setWindowOver (InputEvent event_)
{
	isOver = true;

	events.emit(ZEN_INPUT_WINDOW_OVER, event);
}

void setWindowOut (InputEvent event_)
{
	isOut = true;

	events.emit(ZEN_INPUT_WINDOW_OUT, event);
}

void preRender (Uint32 time_, Uint32 delta_)
{
	auto& scenes_ = g_scene.getScenes(true, true);

	events_.emit("SYS_UPDATE");

	for (auto& scene_ : scenes_)
	{
		if (scene.sys.input && scene.sys.input.updatePoll(time_, delta_) && globalTopOnly)
		{
			//  If the Scene returns true, it means it captured some input that no other Scene should get, so we bail out
			return;
		}
	}
}

void InputManager::handleSDLEvents ()
{
	SDL_Event event_;

	while (SDL_PollEvent(&event_))
	{
		switch (event_.type)
		{
			// Mouse
			case SDL_MOUSEMOTION:
				emit("pointermove",
					event_.motion.x,		// Relative to window
					event_.motion.y,		// Relative to window
					event_.motion.xrel,		// Delta (Relative to last x)
					event_.motion.yrel);	// Delta (Relative to last y)
				break;

			case SDL_MOUSEBUTTONDOWN:
				emit("pointerdown",
					event_.button.x,
					event_.button.y,
					event_.button.clicks,	// 1 (single-click), 2 (double-click)...
					);
				break;

			case SDL_MOUSEBUTTONUP:
				emit("pointerup",
					event_.button.x,
					event_.button.y,
					event_.button.clicks,	// 1 (single-click), 2 (double-click)...
					);
				break;

			case SDL_MOUSEWHEEL:
				emit("wheel",
					event_.button.x,
					event_.button.y
					);
				break;

			case SDL_MOUSEWHEEL_NORMAL:
				break;

			case SDL_MOUSEWHEEL_FLIPPED:
				break;
		}
	}
}

}	// namespace Zen
