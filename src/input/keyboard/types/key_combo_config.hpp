/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_INPUT_KEYBOARD_TYPES_COMBOCONFIG_HPP
#define ZEN_INPUT_KEYBOARD_TYPES_COMBOCONFIG_HPP

#include <SDL2/SDL_stdinc.h>

namespace Zen {

/**
 * @struct KeyComboConfig
 * @since 0.0.0
 */
struct KeyComboConfig
{
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
};

}	// namespace Zen

#endif
