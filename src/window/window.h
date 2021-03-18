/**
 * @file		window.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <utility>

#include "../messages.h"
#include "../core/game_config.h"
#include "../const.h"

// Forward declaration
#include "../core/game.fwd.h"

namespace Zen {

/**
 * A wrapper for the SDL_Window and it's renderer.
 *
 * @class Window
 * @since 0.0.0
 *
 * @param g The Game instance.
 */
class Window
{
public:
	Window (Game& g);
	~Window ();

	/**
	 * A reference to the Game instance this Window belongs to.
	 *
	 * @since 0.0.0
	 */
	Game& game;

	/**
	 * The game config object.
	 *
	 * @since 0.0.0
	 */
	GameConfig config;

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
	int create (GameConfig cfg);

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
	void handleSDLEvents ();

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
	void setPixelArt (bool pixelArt);

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
	 * - CLEANUP (Enum Class)
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
		void cleanup (T t, Args&&... args);
};

/**
 * The "CLEANUP" specialized cleanup method.
 *
 * The method declaration is outside the class because template
 * specialization cannot happen inside a class scope.
 *
 * @since 0.0.0
 * @sa void cleanup (T t, Args&&... args)
 */
template<>
void Window::cleanup<CLEANUP> (CLEANUP c);

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
void Window::cleanup<SDL_Window*> (SDL_Window *win);

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
void Window::cleanup<SDL_Renderer*> (SDL_Renderer *ren);

}	// namespace Zen

// Declarations of the forward declared elements
#include "../core/game.h"

#endif
