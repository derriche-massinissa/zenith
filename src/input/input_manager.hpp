/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_INPUT_INPUTMANAGER_HPP
#define ZEN_INPUT_INPUTMANAGER_HPP

#include "../event/event_emitter.hpp"
#include "pointer.hpp"
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_stdinc.h>
#include "../math/types/vector2.hpp"
#include "../components/transform_matrix.hpp"
#include "types/event.hpp"
#include "../core/config.fwd.hpp"
#include "const.hpp"

namespace Zen {

class InputManager
{
public:
	GameConfig* config;

	bool enabled = true;

	bool booted = false;

	EventEmitter events;

	bool isOver = true;

	/**
	 * The default cursor to be used when interacting with the game.
	 *
	 * See the setDefaultCursor method for more details.
	 *
	 * @since 0.0.0
	 */
	std::string defaultCursor;

	std::vector<Pointer> pointers;

	Pointer* mousePointer;

	Pointer* activePointer;

	bool globalTopOnly = true;

	Uint32 time = 0;

	Math::Vector2 tempPoint;

	std::vector<Entity> tempHitTest;

	Components::TransformMatrix tempMatrix;

	Components::TransformMatrix tempMatrix2;

	bool tempSkip = false;

	std::map<std::string, SDL_Cursor*> cursors;

	/**
	 * A pointer to the "pointerin" event listener, to later remove it.
	 *
	 * @since 0.0.0
	 */
	ListenerBase* lPointerIn = nullptr;

	/**
	 * A pointer to the "pointerout" event listener, to later remove it.
	 *
	 * @since 0.0.0
	 */
	ListenerBase* lPointerOut = nullptr;

	InputManager ();

	~InputManager ();

	void boot (GameConfig* cfg_);

	void setWindowOver ();

	void setWindowOut ();

	void preRender (Uint32 time_, Uint32 delta_);

	/**
	 * @since 0.0.0
	 */
	void createCursor ();

	/**
	 * Creates a new cursor from an image file.
	 *
	 * The LoaderPlugin should be used to add cursors from files, i.e:
	 *
	 * ```cpp
	 * load.cursor("hand", "../assets/dragon_hand.png");
	 * ```
	 *
	 * @since 0.0.0
	 *
	 * @param key The key to store the cursor with.
	 * @param path The path to the image file to use.
	 * @param hotX The X-axis location of the upper left corner of the cursor
	 * relative to the actual mouse position.
	 * @param hotY The Y-axis location of the upper left corner of the cursor
	 * relative to the actual mouse position.
	 */
	void createColorCursor (std::string key, std::string path, int hotX, int hotY);

	void createSystemCursor (std::string key, SDL_SystemCursor cursor);

	/**
	 * @since 0.0.0
	 */
	void showCursor (bool toggle);

	bool isCursorHidden ();

    /**
     * Tells the InputManager to set a custom cursor.
     *
     * This cursor will be the default cursor used when interacting with the game
	 * window.
     *
     * If an Interactive Object also sets a custom cursor, this is the cursor that
	 * is reset after its use.
     *
     * ```cpp
     * input.setDefaultCursor("sword");
     * ```
	 *
	 * Note that a suitable cursor needs to have been loaded first, just like
	 * images.
	 *
	 * ```cpp
	 * load.cursor("sword", "assets/cursors/sword.cur");
	 * load.cursor("arrow", "assets/cursors/arrow.png");
	 * load.cursor("magic", "assets/cursors/magic.gif");
	 * ```
     *
     * @since 0.0.0
     *
     * @param cursor The key of the cursor to be used when setting the default
	 * cursor.
     */
	void setDefaultCursor (std::string cursor);

    /**
     * Set the given cursor as the active cursor.
	 *
     * Also called by the InputPlugin when processing over and out events.
     * Tells the InputManager to set a custom cursor during its postUpdate step.
     *
     * @since 0.0.0
     *
     * @param cursor The key of the cursor to set.
     */
	void setCursor (std::string cursor);

    /**
     * Called by the InputPlugin when processing over and out events.
     *
     * Tells the InputManager to clear the custom cursor, if set, during its
	 * postUpdate step.
     *
     * @since 0.0.0
     *
     * @param interactiveObject The Interactive Object that called this method.
     */
	void resetCursor ();

	std::vector<Pointer*> addPointer (int quantity = 1);

	void updateInputPlugins (INPUT type, std::vector<Pointer*> pointers);

	void onTouchStart (InputEvent event);

	void onTouchMove (InputEvent event_);

	void onTouchEnd (InputEvent event_);

	void onTouchCancel (InputEvent event_);

	void onMouseDown (InputEvent event_);

	void onMouseMove (InputEvent event_);

	void onMouseUp (InputEvent event_);

	void onMouseWheel (InputEvent event_);

	void onPointerLockChange (bool locked_);

	bool inputCandidate (Entity entity_, Entity camera_);

	std::vector<Entity> hitTest (Pointer* pointer_, std::vector<Entity> gameObject_, Entity camera_);

	bool pointWithinHitArea (Entity gameObject_, double x_, double y_);

	void transformPointer (Pointer* pointer_, double windowX_, double windowY_, bool wasMove_);
};

}	// namespace Zen

#endif
