/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_INPUT_KEYBOARD_COMBO_COMBOKEY_HPP
#define ZEN_INPUT_KEYBOARD_COMBO_COMBOKEY_HPP

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <string>
#include <vector>
#include "../types/key_combo_config.hpp"
#include "../../../event/event_emitter.hpp"
#include "../keyboard_plugin.fwd.hpp"

namespace Zen {

/**
 * A KeyCombo will listen for a specific string of keys from the Keyboard, and when
 * it receives them it will emit a `keycombomatch` event from the Keyboard Manager,
 * as well as from itself.
 *
 * The keys to be listened for can be defined as:
 *
 * A string (i.e. "ATARI")
 * An array of either key codes or strings
 *
 * For example, to listen for the Konami code (up, up, down, down, left, right,
 * left, right, b, a, enter) you could do one of the following:
 *
 * ```cpp
 * input.keyboard.createCombo(
 *     { SDLK_UP, SDLK_UP, SDLK_DOWN, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_LEFT,
 * SDLK_RIGHT, SDLK_b, SDLK_a, SDLK_RETURN },
 *     { .resetOnMatch = true }
 * );
 *
 * KeyCombo* combo = input.keyboard.createCombo(
 *     { "Up", "Up", "Down", "Down", "Left", "Right", "Left", "Right", "B", "A",
 * "Return" },
 *     { .resetOnMatch = true }
 * );
 *
 * this.input.keyboard.on("keycombomatch", [] (??? *event) {
 *     std::cout << "Konami Code entered!" << std::endl;
 * });
 *
 * combo->on("keycombomatch", [] () {
 *     std::cout << "Konami Code entered!" << std::endl;
 * });
 * ```
 *
 * Or, to listen for the user entering the word ZENITH:
 *
 * ```cpp
 * input.keyboard.createCombo("ZENITH");
 * ```
 *
 * Note that the above only works with keys having a single character as a name,
 * like "A", "Z", "$", ".", ";"...
 * To send something like "Space" for the Space Bar you have to send an array of
 * strings instead like shown previously.
 *
 * @class KeyCombo
 * @since 0.0.0
 *
 * @param keyboardPlugin A reference to the Keyboard Plugin.
 * @param keys The keys that comprise this combo.
 * @param config A Key Combo configuration object.
 */
class KeyCombo : public EventEmitter
{
public:
	/**
	 * A flag that controls if this KeyCombo is actively processing keys or not.
	 *
	 * @since 0.0.0
	 */
	bool enabled = true;

	/**
	 * An array of the keycodes that comprise this combo.
	 *
	 * @since 0.0.0
	 */
	std::vector<SDL_Keycode> keyCodes;

	/**
	 * The current index of the key code the combo is waiting for.
	 *
	 * @since 0.0.0
	 */
	unsigned int index = 0;

	/**
	 * The length of this combo (in key codes).
	 *
	 * @since 0.0.0
	 */
	unsigned int size = 0;

	/**
	 * The time the previous key in the combo was matched.
	 *
	 * @since 0.0.0
	 */
	Uint32 timeLastMatched = 0;

	/**
	 * Has this Key Combo been matched yet?
	 *
	 * @since 0.0.0
	 */
	bool matched = false;

	/**
	 * The time the entire combo was matched.
	 *
	 * @since 0.0.0
	 */
	Uint32 timeMatched = 0;

	/**
	 * If the wrong key is pressed, reset this combo?
	 *
	 * @since 0.0.0
	 */
	bool resetOnWrongKey = true;

	/**
	 * The max delay in ms between each key press. Above this the combo is reset.
	 * 0 means disabled.
	 *
	 * @since 0.0.0
	 */
	Uint32 maxKeyDelay = 0;

	/**
	 * If previously matched and the first key is pressed again, will it reset?
	 *
	 * @since 0.0.0
	 */
	bool resetOnMatch = false;

	/**
	 * If the combo is matched, should it be deleted?
	 *
	 * @since 0.0.0
	 */
	bool deleteOnMatch = false;

	/**
	 * Construct the KeyCombo using a string of key names (Single character).
	 *
	 * @since 0.0.0
	 */
	KeyCombo (const char * keys_, KeyComboConfig config_ = {});

	/**
	 * Construct the KeyCombo using a vector of key names.
	 *
	 * @since 0.0.0
	 */
	KeyCombo (std::vector<const char*> keys_, KeyComboConfig config_ = {});

	/**
	 * Construct the KeyCombo using a vector of keycodes.
	 *
	 * @since 0.0.0
	 */
	KeyCombo (std::vector<SDL_Keycode> keys_, KeyComboConfig config_ = {});

    /**
     * How far complete is this combo? A value between 0 and 1.
     *
     * @since 0.0.0
     */
	double getProgress ();

	/**
	 * Process a key down event.
	 *
	 * @since 0.0.0
	 *
	 * @param event A pointer to the SDL keyboard event.
	 *
	 * @return `true` if it should to be deleted, `false` if it should be kept.
	 */
	bool onKeyDown (KeyboardPlugin* kbPlugin_, const SDL_Event * const event_);

private:
	/**
	 * Used internally by the KeyCombo class.
	 *
	 * @since 0.0.0
	 *
	 * @param event The native Keyboard Event.
	 *
	 * @return `true` if the combo was matched, otherwise `false`.
	 */
	bool process (const SDL_Event * const event_);

	/**
	 * Used internally by the KeyCombo class.
	 * Return `true` if it reached the end of the combo, `false` if not.
	 *
	 * @since 0.0.0
	 *
	 * @param event The native Keyboard Event.
	 *
	 * @return `true` if it reached the end of the combo, `false` if not.
	 */
	bool advance (const SDL_Event * const event_);

	/**
	 * Used internally by the KeyCombo class.
	 *
	 * @since 0.0.0
	 */
	void reset ();
};

}	// namespace Zen

#endif
