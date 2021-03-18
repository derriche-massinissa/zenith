/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "window.h"

namespace Zen {

Window::Window (Game& game_)
	: game(game_)
{}

Window::~Window ()
{
	removeAllListeners();

	close();
}

int Window::width()
{
	int width_ = 0;

	SDL_GetRendererOutputSize(renderer, &width_, nullptr);

	return width_;
}

int Window::height()
{
	int height_ = 0;

	SDL_GetRendererOutputSize(renderer, nullptr, &height_);

	return height_;
}

int Window::create (Core::GameConfig config_)
{
	config = config_;

	if (initSdl()) {
		return 1;
	} else if (initSdlImg()) {
		cleanup(CLEANUP::SDL);
		return 1;
	} else if (initSdlMixer()) {
		cleanup(CLEANUP::IMG, CLEANUP::SDL);
		return 1;
	} else if (initSdlTtf()) {
		cleanup(CLEANUP::MIX, CLEANUP::IMG, CLEANUP::SDL);
		return 1;
	} else if (createWindow()) {
		cleanup(CLEANUP::TTF, CLEANUP::MIX, CLEANUP::IMG, CLEANUP::SDL);
		return 1;
	} else if (createRenderer()) {
		cleanup(window_, CLEANUP::TTF, CLEANUP::MIX, CLEANUP::IMG, CLEANUP::SDL);
		return 1;
	} else {
		// Everything has gone well

		setPixelArt(config.pixelArt);

		if (config.minWidth || config.minHeight)
			setMinSize(config.minWidth, config.minHeight);

		if (config.maxWidth || config.maxHeight)
			setMaxSize(config.maxWidth, config.maxHeight);
	}

	return 0;
}

int Window::initSdl ()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		messageError("Could not initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	return 0;
}

int Window::initSdlImg ()
{
	// Initialize SDL_image
	int imgFlags_ = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags_) & imgFlags_)) {
		messageError("Could not initialize SDL_image: %s\n", IMG_GetError());
		return 1;
	}

	return 0;
}

int Window::initSdlMixer ()
{
	// Initialize SDL_mixer
	// TODO Set these constants from the config object
	const int frequency_ = 44100;
	Uint16 format_ = MIX_DEFAULT_FORMAT;
	int channels_ = 2;
	int chunksize_ = 2048;
	if (Mix_OpenAudio(frequency_, format_, channels_, chunksize_) < 0) {
		messageError("Could not initialize SDL_mixer: %s\n", Mix_GetError());
		return 1;
	}

	return 0;
}

int Window::initSdlTtf ()
{
	// Initialize SDL_ttf
	if (TTF_Init() != 0) {
		messageError("Could not initialize SDL_ttf: %s\n", TTF_GetError());
		return 1;
	}

	return 0;
}

int Window::createWindow ()
{
	// Create a window
	window = SDL_CreateWindow(
			config.title.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			config.width,
			config.height,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
			);
	if (window == nullptr) {
		messageError("Window could not be created: %s\n", SDL_GetError());
		return 1;
	}

	return 0;
}

int Window::createRenderer ()
{
	// Create a renderer for the window
	renderer = SDL_CreateRenderer(
			window,
			-1,
			SDL_RENDERER_ACCELERATED |
			SDL_RENDERER_PRESENTVSYNC
			);
	if (renderer == nullptr) {
		messageError("Renderer could not be created: %s\n", SDL_GetError());
		return 1;
	}

	// Initialize the render color
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);

	return 0;
}

void Window::setPixelArt (bool pixelArt_)
{
	// Texture filtering
	if (pixelArt_) {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")) {
			messageWarning("Nearest pixel sampling not enabled!");
		}
	} else {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			messageWarning("Linear texture filtering not enabled!");
		}
	}
}

int Window::close ()
{
	// Do not forget to free resources in their respective managers before
	// running this!
	cleanup(
			renderer,
			window,
			CLEANUP::TTF,
			CLEANUP::MIX,
			CLEANUP::IMG,
			CLEANUP::SDL
		   );
	renderer = nullptr;
	window = nullptr;

	return 0;
}

// Clean up template
//template<typename T>
//void Window::cleanup (T t)
//{
//	return;
//}

	template<typename T, typename... Args>
void Window::cleanup (T t_, Args&&... args_)
{
	cleanup(t_);
	cleanup(std::forward<Args>(args_)...);
}

// Specializations
template<>
void Window::cleanup<CLEANUP> (CLEANUP c_)
{
	switch (c_)
	{
		case CLEANUP::SDL:
			SDL_Quit();
			break;

		case CLEANUP::IMG:
			IMG_Quit();
			break;

		case CLEANUP::MIX:
			Mix_Quit();
			break;

		case CLEANUP::TTF:
			TTF_Quit();
			break;
	}
}

template<>
void Window::cleanup<SDL_Window*> (SDL_Window *win_)
{
	if (!win_) return;

	SDL_DestroyWindow(win_);
}

template<>
void Window::cleanup<SDL_Renderer*> (SDL_Renderer *ren_)
{
	if (!ren_) return;

	SDL_DestroyRenderer(ren_);
}

void Window::handleSDLEvents ()
{
	SDL_Event event_;

	while (SDL_PollEvent(&event_))
	{
		switch (event_.type)
		{
			case SDL_QUIT:
				emit("quit");
				break;

			case SDL_WINDOWEVENT:
				switch (event_.window.event)
				{
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						emit("resize", event_.window.data1, event_.window.data2);
						break;

					case SDL_WINDOWEVENT_EXPOSED:
						emit("expose");
						break;

					case SDL_WINDOWEVENT_ENTER:
						pointerIn = true;
						emit("pointerin");
						break;

					case SDL_WINDOWEVENT_LEAVE:
						pointerIn = false;
						emit("pointerout");
						break;

					case SDL_WINDOWEVENT_FOCUS_GAINED:
						focused = true;
						emit("focus");
						break;

					case SDL_WINDOWEVENT_FOCUS_LOST:
						focused = false;
						emit("blur");
						break;

					case SDL_WINDOWEVENT_MINIMIZED:
						minimized = true;
						emit("minimize");
						break;

					case SDL_WINDOWEVENT_MAXIMIZED:
						minimized = false;
						emit("maximize");
						break;

					case SDL_WINDOWEVENT_RESTORED:
						minimized = false;
						emit("restore");
						break;
				}
				break;
		}
	}
}

Window& setTitle (std::string title_)
{
	SDL_SetWindowTitle(window, title_.c_str());

	return *this;
}

Window& setFullscreen (bool flag_ = true)
{
	if (flag_)
		SDL_SetWindowFullscreen(window, SDL_TRUE);
	else
		SDL_SetWindowFullscreen(window, SDL_FALSE);

	return *this;
}

Window& Window::setMinSize (int width_, int height_);
{
	SDL_SetWindowMinimumSize(window, width_, height_);

	return *this;
}

Window& Window::setMaxSize (int width_, int height_)
{
	SDL_SetWindowMaximumSize(window, width_, height_);

	return *this;
}

bool isMinimized ()
{
	return minimized;
}

bool isFocused ()
{
	return focused;
}

bool isPointerIn ()
{
	return pointerIn;
}

}	// namespace Zen
