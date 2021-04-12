/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_INPUT_KEYBOARD_KEYS_KEY_HPP
#define ZEN_INPUT_KEYBOARD_KEYS_KEY_HPP

#include <SDL2/SDL_events.h>
#include "../../../event/event_emitter.hpp"

namespace Zen {

class Key : public EventEmitter
{
public:
	/**
	 * The keycode of this key.
	 *
	 * @since 0.0.0
	 */
	SDL_Keycode keyCode;

	/**
	 * Can this Key be processed?
	 *
	 * @since 0.0.0
	 */
	bool enabled = true;

	/**
	 * The "down" state of the key. This will remain `true` for as long as the
	 * keyboard thinks this key is held down.
	 *
	 * @since 0.0.0
	 */
	bool isDown = false;

	/**
	 * The "up" state of the key. This will remain `true` for as long as the
	 * keyboard thinks this key is up.
	 *
	 * @since 0.0.0
	 */
	bool isUp = true;

	/**
	 * The timestamp when the key was last pressed down.
	 *
	 * @since 0.0.0
	 */
	Uint32 timeDown = 0;

	/**
	 * The number of milliseconds this key was held down for in the previous
	 * down / up sequence.
	 * This value isn't updated every game step, only when the Key changes state.
	 * To get the current duration use the `getDuration` method.
	 *
	 * @since 0.0.0
	 */
	Uint32 duration = 0;

	/**
	 * The timestamp when the key was last released.
	 *
	 * @since 0.0.0
	 */
	Uint32 timeUp = 0;

	/**
	 * When a key is held down should it continuously fire the `down` event each
	 * time it repeats?
	 *
	 * By default it will emit the `down` event just once, but if you wish to
	 * receive the event for each repeat as well, enable this property.
	 *
	 * @since 0.0.0
	 */
	bool emitOnRepeat = false;

	/**
	 * If a key is held down this holds down the number of times the key has
	 * 'repeated'.
	 *
	 * @since 0.0.0
	 */
	unsigned int repeats = 0;

	/**
	 * True if the key has just been pressed (NOTE: requires to be reset, see
	 * justDown getter) isJustDown...?
	 *
	 * @since 0.0.0
	 */
	bool _justDown = false;

	/**
	 * True if the key has just been released (NOTE: requires to be reset, see
	 * justDown getter) isJustDown...?
	 *
	 * @since 0.0.0
	 */
	bool _justUp = false;

	/**
	 * Internal tick counter.
	 *
	 * @since 0.0.0
	 */
	int _tick = -1;

	/**
	 * The state of the modifier keys when this key was used.
	 *
	 * @since 0.0.0
	 */
	SDL_Keymod keyMod = KMOD_NONE;

	Key (SDL_Keycode keyCode, bool emitOnRepeat = false);

	/**
	 * The down state of the ALT key, if pressed at the same time as this key.
	 *
	 * @since 0.0.0
	 */
	bool altKey ();

	/**
	 * The down state of the left ALT key, if pressed at the same time as this key.
	 *
	 * @since 0.0.0
	 */
	bool lAltKey ();

	/**
	 * The down state of the right ALT key, if pressed at the same time as this key.
	 *
	 * @since 0.0.0
	 */
	bool rAltKey ();

	/**
	 * The down state of the CTRL key, if pressed at the same time as this key.
	 *
	 * @since 0.0.0
	 */
	bool ctrlKey ();

	/**
	 * The down state of the left CTRL key, if pressed at the same time as this key.
	 *
	 * @since 0.0.0
	 */
	bool lCtrlKey ();

	/**
	 * The down state of the right CTRL key, if pressed at the same time as this
	 * key.
	 *
	 * @since 0.0.0
	 */
	bool rCtrlKey ();

	/**
	 * The down state of the SHIFT key, if pressed at the same time as this key.
	 *
	 * @since 0.0.0
	 */
	bool shiftKey ();

	/**
	 * The down state of the left SHIFT key, if pressed at the same time as this
	 * key.
	 *
	 * @since 0.0.0
	 */
	bool lShiftKey ();

	/**
	 * The down state of the right SHIFT key, if pressed at the same time as this
	 * key.
	 *
	 * @since 0.0.0
	 */
	bool rShiftKey ();

	/**
	 * The down state of the Meta key, if pressed at the same time as this key.
	 *
	 * On Linux, the Meta Key is the Super Key.
	 * On a Mac it is the Command Key.
	 * On Windows, it is the Windows Key.
	 *
	 * @since 0.0.0
	 */
	bool metaKey ();

	/**
	 * The down state of the left Meta key, if pressed at the same time as this key.
	 *
	 * On Linux, the Meta Key is the Super Key.
	 * On a Mac it is the Command Key.
	 * On Windows, it is the Windows Key.
	 *
	 * @since 0.0.0
	 */
	bool lMetaKey ();

	/**
	 * The down state of the right Meta key, if pressed at the same time as this
	 * key.
	 *
	 * On Linux, the Meta Key is the Super Key.
	 * On a Mac it is the Command Key.
	 * On Windows, it is the Windows Key.
	 *
	 * @since 0.0.0
	 */
	bool rMetaKey ();

    /**
     * Processes the Key Down action for this Key.
     * Called automatically by the Keyboard Plugin.
     *
     * @since 0.0.0
     *
     * @param event The SDL Keyboard event.
     */
	void onDown (const SDL_Event * const event);

    /**
     * Processes the Key Up action for this Key.
     * Called automatically by the Keyboard Plugin.
     *
     * @since 0.0.0
     *
     * @param event The SDL Keyboard event.
     */
	void onUp (const SDL_Event * const event);

    /**
     * Resets this Key object back to its default un-pressed state.
     *
     * @since 0.0.0
     */
	void reset ();

    /**
     * Returns the duration, in ms, that the Key has been held down for.
     *
     * If the key is not currently down it will return zero.
     *
     * The get the duration the Key was held down for in the previous up-down cycle,
     * use the `Key::duration` property value instead.
     *
     * @since 0.0.0
     *
     * @return The duration, in ms, that the Key has been held down for if
	 * currently down.
     */
    Uint32 getDuration ();

	/**
	 * Returns `true` if the Key was pressed down within the `duration` value
	 * given, based on the current game clock time. Or `false` if it either isn't
	 * down, or was pressed down longer ago than the given duration.
	 *
	 * @since 0.0.0
	 *
	 * @param duration The duration, in ms, within which the key must have been
	 * pressed down.
	 *
	 * @return `true` if the Key was pressed down within `duration` ms ago,
	 * otherwise `false`.
	 */
	bool downDuration (Uint32 duration = 50);

	/**
	 * Returns `true` if the Key was released within the `duration` value given,
	 * based on the current game clock time. Or `false` if it either isn't
	 * up, or was released longer ago than the given duration.
	 *
	 * @since 0.0.0
	 *
	 * @param duration The duration, in ms, within which the key must have been
	 * released.
	 *
	 * @return `true` if the Key was released within `duration` ms ago, otherwise
	 * `false`.
	 */
	bool upDuration (Uint32 duration = 50);

	/**
	 * The justDown value allows you to test if this Key has just been pressed down
	 * or not.
	 *
	 * When you check this value it will return `true` if the Key is down,
	 * otherwise `false`.
	 *
	 * You can only call justDown once per key press. It will only return `true`
	 * once, until the Key is released and pressed down again.
	 * This allows you to use it in situations where you want to check if this key
	 * is down without using an event, such as in a core game loop.
	 *
	 * @since 0.0.0
	 *
	 * @return `true` if the Key was just pressed, otherwise `false`.
	 */
	bool justDown ();

	/**
	 * The justUp value allows you to test if this Key has just been released
	 * or not.
	 *
	 * When you check this value it will return `true` if the Key is up, otherwise
	 * `false`.
	 *
	 * You can only call justUp once per key release. It will only return `true`
	 * once, until the Key is pressed and released again.
	 * This allows you to use it in situations where you want to check if this key
	 * is up without using an event, such as in a core game loop.
	 *
	 * @since 0.0.0
	 *
	 * @return `true` if the Key was just released, otherwise `false`.
	 */
	bool justUp ();

    /**
     * Checks if the Key is currently being held down.
     *
     * The difference between this method and checking the `isDown` property
	 * directly is that you can provide a duration to this method. For example, if
	 * you wanted a key press to fire a bullet, but you only wanted it to be able
	 * to fire every 100ms, then you can call this method with a `duration` of 100
	 * and it will only return `true` every 100ms.
     *
     * @since 0.0.0
     *
     * @param duration The duration which must have elapsed before this Key is
	 * considered as being down.
     *
     * @return `true` if the Key is down within the duration specified, otherwise
	 * `false`.
     */
	bool checkDown (Uint32 duration = 0);

	bool operator == (const Key& rhs);
};

}	// namespace Zen

#endif
