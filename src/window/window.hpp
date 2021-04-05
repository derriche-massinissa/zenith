/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_WINDOW_HPP
#define ZEN_WINDOW_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "const.hpp"
#include "../event/event_emitter.hpp"
#include "../core/config.fwd.hpp"

/**
 * @todo "What I learned" list!
 * @todo source files (cpp) are compiled individualy!!!!!!
 *
 * @todo move all necessary includes in cpp files!
 * @todo make as much use of forward dclarations as possible
 */
namespace Zen {

/**
 * A wrapper for the SDL_Window and its renderer.
 *
 * @class Window
 * @since 0.0.0
 */
class Window : public EventEmitter
{
public:
	/**
	 * @since 0.0.0
	 */
	~Window ();

	/**
	 * A reference to the Game instance this Window belongs to.
	 *
	 * @since 0.0.0
	 */
	GameConfig *config;

	/** 
	 * A reference to the SDL window.
	 *
	 * @since 0.0.0
	 */
	SDL_Window *window = nullptr;

	/** 
	 * A reference to the SDL window.
	 *
	 * @since 0.0.0
	 */
	SDL_Renderer *renderer = nullptr;

	/**
	 * Getter for the window's width.
	 *
	 * @return The width of the window in pixels.
	 */
	int width ();

	/**
	 * Getter for the window's width.
	 *
	 * @return The width of the window in pixels.
	 */
	int height ();

	/**
	 * This method is responsible for starting the window, its
	 * renderer, and other SDL2 specific features.
	 *
	 * @since 0.0.0
	 *
	 * @return 0 if no problem occured, 1 otherwise.
	 */
	int create (GameConfig *cfg);

	/**
	 * This method is responsible for destroying the renderer and the
	 * window, as well as quiting SDL and its other extensions.
	 *
	 * @since 0.0.0
	 *
	 * @return 0 if no problem occured, 1 otherwise.
	 */
	int close ();

	/**
	 * An event handler for all SDL Events, be it window events, keyboard events,
	 * mouse/pointer events...
	 *
	 * @since 0.0.0
	 */
	void handleSDLEvents (SDL_Event event_);

	/**
	 * This method initializes SDL2.
	 *
	 * @since 0.0.0
	 *
	 * @return 1 if an error occured, 0 otherwise.
	 */
	int initSdl ();

	/**
	 * This method initializes SDL_image.
	 *
	 * @since 0.0.0
	 *
	 * @return 1 if an error occured, 0 otherwise.
	 */
	int initSdlImg ();

	/**
	 * This method initializes SDL_mixer.
	 *
	 * @since 0.0.0
	 *
	 * @return 1 if an error occured, 0 otherwise.
	 */
	int initSdlMixer ();

	/**
	 * This method initializes SDL_ttf.
	 *
	 * @since 0.0.0
	 *
	 * @return 1 if an error occured, 0 otherwise.
	 */
	int initSdlTtf ();

	/**
	 * This method creates a SDL_window.
	 *
	 * @since 0.0.0
	 *
	 * @return 1 if an error occured, 0 otherwise.
	 */
	int createWindow ();

	/**
	 * This method creates a renderer for the window.
	 *
	 * @since 0.0.0
	 *
	 * @return 1 if an error occured, 0 otherwise.
	 */
	int createRenderer ();

	/**
	 * This method sets the render scale quality hint, to prevent pixel art
	 * from becoming blurred when scaled. It will remain crisp.
	 *
	 * @since 0.0.0
	 *
	 * @param pixelArt The value to set `pixelArt` to.
	 */
	void setPixelArt (bool pixelArt_);

	/**
	 * @since 0.0.0
	 * @sa void cleanup (T t, Args&&... args)
	 */
	//template<typename T>
	//void cleanup (T t);

	/**
	 * The clean up method takes in any number of elements to free, destroy
	 * or quit them.
	 *
	 * It is a veriadic template, so it can take in any number of parameter of
	 * any of the following types:
	 * - WINDOW_CLEANUP (Enum Class)
	 * - SDL_Window
	 * - SDL_Renderer
	 *
	 * It will then take appropriate actions depending on the type of each
	 * parameter.
	 *
	 * @since 0.0.0
	 * @param t Template parameter.
	 * @param args Template parameter pack.
	 */
	template<typename T, typename... Args>
		void cleanup (T t_, Args&&... args_);

	/**
	 * Sets the window's title.
	 *
	 * @since 0.0.0
	 *
	 * @param title The new window's title.
	 *
	 * @return This window instance.
	 */
	Window& setTitle (const char* title_);

	/**
	 * Make the window go fullscreen or into windowed mode.
	 *
	 * @since 0.0.0
	 *
	 * @param flag `true` to go fullscreen, `false` to remove it.
	 *
	 * @return This window instance.
	 */
	Window& setFullscreen (bool flag_ = true);

	/**
	 * Sets the minimum size this window can be resized to. Use with no parameter
	 * to remove the limit.
	 *
	 * Note that the user's window manager needs to support and use the
	 * necessary window hints for this to work.
	 *
	 * @since 0.0.0
	 *
	 * @param width The minimum width the window can be resized to.
	 * @param height The minimum height the window can be resized to.
	 *
	 * @return This window instance.
	 */
	Window& setMinSize (int width_ = 0, int height_ = 0);

	/**
	 * Sets the maximum size this window can be resized to. Use with no parameter
	 * to remove the limit.
	 *
	 * Note that the user's window manager needs to support and use the
	 * necessary window hints for this to work.
	 *
	 * @since 0.0.0
	 *
	 * @param width The maximum width the window can be resized to.
	 * @param height The maximum height the window can be resized to.
	 *
	 * @return This window instance.
	 */
	Window& setMaxSize (int width_ = 0, int height_ = 0);

	/**
	 * Is the window minimized?
	 *
	 * @since 0.0.0
	 *
	 * @return `true` if the window is minimized, otherwise `false`.
	 */
	bool isMinimized ();

	/**
	 * Is the window focused?
	 *
	 * @since 0.0.0
	 *
	 * @return `true` if the window is focused, otherwise `false`.
	 */
	bool isFocused ();
	
	/**
	 * Is the pointer inside the window?
	 *
	 * @since 0.0.0
	 *
	 * @return `true` if the pointer is in the window, otherwise `false`.
	 */
	bool isPointerIn ();

	/**
	 * Is the window minimized?
	 *
	 * @since 0.0.0
	 */
	bool minimized = false;

	/**
	 * Is the window focused?
	 *
	 * @since 0.0.0
	 */
	bool focused = false;

	/**
	 * Is the pointer inside the window?
	 *
	 * @since 0.0.0
	 */
	bool pointerIn = false;
};

/**
 * The "WINDOW_CLEANUP" specialized cleanup method.
 *
 * The method declaration is outside the class because template
 * specialization cannot happen inside a class scope.
 *
 * @since 0.0.0
 * @sa void cleanup (T t, Args&&... args)
 */
template<>
void Window::cleanup<WINDOW_CLEANUP> (WINDOW_CLEANUP c_);

/**
 * The "SDL_Window" specialized cleanup method.
 *
 * The method declaration is outside the class because template
 * specialization cannot happen inside a class scope.
 *
 * @since 0.0.0
 * @sa void cleanup (T t, Args&&... args)
 */
template<>
void Window::cleanup<SDL_Window*> (SDL_Window *win_);

/**
 * The "SDL_Renderer" specialized cleanup method.
 *
 * The method declaration is outside the class because template
 * specialization cannot happen inside a class scope.
 *
 * @since 0.0.0
 * @sa void cleanup (T t, Args&&... args)
 */
template<>
void Window::cleanup<SDL_Renderer*> (SDL_Renderer *ren_);

}	// namespace Zen

#endif
