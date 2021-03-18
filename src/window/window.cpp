/**
 * @file		window.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "window.h"

namespace Zen {

Window::Window (Game& g)
	: game(g)
{}

Window::~Window ()
{
	close();
}

int Window::width()
{
	int width = 0;

	SDL_GetRendererOutputSize(renderer_, &width, nullptr);

	return width;
}

int Window::height()
{
	int height = 0;

	SDL_GetRendererOutputSize(renderer_, nullptr, &height);

	return height;
}

int Window::create (GameConfig cfg)
{
	config = cfg;

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
		setPixelArt(config.pixelArt);
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
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		messageError("Could not initialize SDL_image: %s\n", IMG_GetError());
		return 1;
	}

	return 0;
}

int Window::initSdlMixer ()
{
	// Initialize SDL_mixer
	// TODO Set these constants from the config object
	const int frequency = 44100;
	Uint16 format = MIX_DEFAULT_FORMAT;
	int channels = 2;
	int chunksize = 2048;
	if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0) {
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
	window_ = SDL_CreateWindow(
			config.title.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			config.width,
			config.height,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
			);
	if (window_ == nullptr) {
		messageError("Window could not be created: %s\n", SDL_GetError());
		return 1;
	}

	return 0;
}

int Window::createRenderer ()
{
	// Create a renderer for the window
	renderer_ = SDL_CreateRenderer(
			window_,
			-1,
			SDL_RENDERER_ACCELERATED |
			SDL_RENDERER_PRESENTVSYNC
			);
	if (renderer_ == nullptr) {
		messageError("Renderer could not be created: %s\n", SDL_GetError());
		return 1;
	}

	// Initialize the render color
	SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0xff);

	return 0;
}

void Window::setPixelArt (bool pixelArt)
{
	// Texture filtering
	if (pixelArt) {
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
			renderer_,
			window_,
			CLEANUP::TTF,
			CLEANUP::MIX,
			CLEANUP::IMG,
			CLEANUP::SDL
		   );
	renderer_ = nullptr;
	window_ = nullptr;

	return 0;
}

// Clean up template
//template<typename T>
//void Window::cleanup (T t)
//{
//	return;
//}

	template<typename T, typename... Args>
void Window::cleanup (T t, Args&&... args)
{
	cleanup(t);
	cleanup(std::forward<Args>(args)...);
}

// Specializations
template<>
void Window::cleanup<CLEANUP> (CLEANUP c)
{
	switch (c) {
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
		default:
			break;
	}
}

template<>
void Window::cleanup<SDL_Window*> (SDL_Window *win)
{
	if (!win) return;
	SDL_DestroyWindow(win);
}

template<>
void Window::cleanup<SDL_Renderer*> (SDL_Renderer *ren)
{
	if (!ren) return;
	SDL_DestroyRenderer(ren);
}

void Window::handleSDLEvents ()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				emit("quit");
				break;

			case SDL_WINDOWEVENT:
				switch (e.window.event)
				{
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						emit("resize", e.window.data1, e.window.data2);
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

Window& setTitle (std::string title)
{
	SDL_SetWindowTitle(window_, title.c_str());

	return *this;
}

Window& setFullscreen (bool flag = true)
{
	if (flag)
		SDL_SetWindowFullscreen(window_, SDL_TRUE);
	else
		SDL_SetWindowFullscreen(window_, SDL_FALSE);

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
