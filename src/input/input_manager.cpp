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
