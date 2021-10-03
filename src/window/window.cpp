/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "window.hpp"

#include <utility>
#include "../core/config.hpp"
#include "../utils/messages.hpp"

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Zen {

Window::~Window ()
{
	// close called from the Game object destructor 'cuz calling it here crashes the program. Fix later, pls...
	//close();
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

int Window::create (GameConfig *cfg)
{
	config = cfg;

	if (initSdl()) {
		return 1;
	} else if (initSdlImg()) {
		cleanup(WINDOW_CLEANUP::SDL);
		return 1;
	} else if (createWindow()) {
		cleanup(WINDOW_CLEANUP::IMG, WINDOW_CLEANUP::SDL);
		return 1;
	} else if (createContext()) {
		cleanup(window, WINDOW_CLEANUP::IMG, WINDOW_CLEANUP::SDL);
		return 1;
	} else {
		// Everything has gone well

		setPixelArt(config->pixelArt);

		if (config->minWidth || config->minHeight)
			setMinSize(config->minWidth, config->minHeight);

		if (config->maxWidth || config->maxHeight)
			setMaxSize(config->maxWidth, config->maxHeight);
	}

	return 0;
}

int Window::initSdl ()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		MessageError("Could not initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	return 0;
}

int Window::initSdlImg ()
{
	// Initialize SDL_image
	int imgFlags_ = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags_) & imgFlags_)) {
		MessageError("Could not initialize SDL_image: %s\n", IMG_GetError());
		return 1;
	}

	return 0;
}

int Window::initGL ()
{
	// Use OpenGL 3.3 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}

int Window::createWindow ()
{
	// Create a window
	window = SDL_CreateWindow(
			config->title.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			config->width,
			config->height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
			);
	if (window == nullptr) {
		MessageError("Window could not be created: %s\n", SDL_GetError());
		return 1;
	}

	return 0;
}

int Window::createContext ()
{
	// Create a renderer for the window
	context = SDL_GL_CreateContext(window);
	if (context == nullptr) {
		MessageError("OpenGL context could not be created: %s\n", SDL_GetError());
		return 1;
	}

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
		MessageError("Could not initialize GLEW: ", glewGetErrorString(glewError));

	// Use VSync
	if (SDL_GL_SetSwapInterval(1) < 0)
		MessageWarning("Could not enable VSync: ", SDL_GetError());

	if (setupGL()) {
		MessageError("Unable to setup OpenGL");

		return -1;
	}

	return 0;
}

int Window::setupGL ()
{
	// Depth testing
	glDisable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	// Stencil testing
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// Blending
	glEnable(GL_BLEND);

	// Face culling (Unneeded because this is a 2D framework)
	glDisable(GL_CULL_FACE);

	// Gamma correction (Default)
	//glEnable(GL_FRAMEBUFFER_SRGB);

	// Draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	return 0;
}

void Window::setPixelArt (bool pixelArt_)
{
	// Texture filtering
	if (pixelArt_) {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")) {
			MessageWarning("Nearest pixel sampling not enabled!");
		}
	} else {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			MessageWarning("Linear texture filtering not enabled!");
		}
	}
}

int Window::close ()
{
	// Do not forget to free resources in their respective managers before
	// running this!
	cleanup(context,
			window,
			WINDOW_CLEANUP::IMG,
			WINDOW_CLEANUP::SDL
		   );
	context = nullptr;
	window = nullptr;

	return 0;
}

template<typename T, typename... Args>
void Window::cleanup (T t_, Args&&... args_)
{
	cleanup(t_);
	cleanup(std::forward<Args>(args_)...);
}

// Specializations
template<>
void Window::cleanup<WINDOW_CLEANUP> (WINDOW_CLEANUP c_)
{
	switch (c_)
	{
		case WINDOW_CLEANUP::SDL:
			SDL_Quit();
			break;

		case WINDOW_CLEANUP::IMG:
			IMG_Quit();
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
void Window::cleanup<SDL_GLContext> (SDL_GLContext ctx_)
{
	if (!ctx_) return;

	SDL_GL_DeleteContext(ctx_);
}

void Window::handleSDLEvents (SDL_Event event_)
{
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
}

Window& Window::setTitle (const char* title_)
{
	SDL_SetWindowTitle(window, title_);

	return *this;
}

Window& Window::setFullscreen (bool flag_)
{
	constexpr int DISPLAY = 0;

	static int posX, posY;

	// Update the flag
	fullscreen = flag_;

	if (fullscreen) { // Go from windowed to fullscreen
		// Save position of window
		SDL_GetWindowPosition(window, &posX, &posY);

		// Get and save dimensions from real display
		SDL_DisplayMode mode;
		SDL_GetCurrentDisplayMode(DISPLAY, &mode);
		SDL_SetWindowFullscreen(window, SDL_TRUE);

		SDL_SetWindowDisplayMode(window, &mode);
	}
	else { // Go from fullscreen to windowed
		SDL_SetWindowFullscreen(window, SDL_FALSE);

		// Restore position of window
		SDL_SetWindowPosition(window, posX, posY);
	}

	return *this;
}

Window& Window::toggleFullscreen ()
{
	return setFullscreen(!fullscreen);
}

Window& Window::setMinSize (int width_, int height_)
{
	SDL_SetWindowMinimumSize(window, width_, height_);

	return *this;
}

Window& Window::setMaxSize (int width_, int height_)
{
	SDL_SetWindowMaximumSize(window, width_, height_);

	return *this;
}

bool Window::isMinimized ()
{
	return minimized;
}

bool Window::isFocused ()
{
	return focused;
}

bool Window::isPointerIn ()
{
	return pointerIn;
}

}	// namespace Zen
