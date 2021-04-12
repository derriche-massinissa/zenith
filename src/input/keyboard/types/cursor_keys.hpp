/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_INPUT_KEYBOARD_TYPES_CURSORKEYS_HPP
#define ZEN_INPUT_KEYBOARD_TYPES_CURSORKEYS_HPP

#include "../keys/key.hpp"

namespace Zen {

/**
 * @struct CursorKeys
 * @since 0.0.0
 */
struct CursorKeys
{
	/**
	 * A key object mapping to the UP arrow key.
	 *
	 * @since 0.0.0
	 */
	Key* up = nullptr;

	/**
	 * A key object mapping to the DOWN arrow key.
	 *
	 * @since 0.0.0
	 */
	Key* down = nullptr;

	/**
	 * A key object mapping to the LEFT arrow key.
	 *
	 * @since 0.0.0
	 */
	Key* left = nullptr;

	/**
	 * A key object mapping to the RIGHT arrow key.
	 *
	 * @since 0.0.0
	 */
	Key* right = nullptr;

	/**
	 * A key object mapping to the SPACE BAR key.
	 *
	 * @since 0.0.0
	 */
	Key* space = nullptr;

	/**
	 * A key object mapping to the LEFT SHIFT key.
	 *
	 * @since 0.0.0
	 */
	Key* lshift = nullptr;

	/**
	 * A key object mapping to the RIGHT SHIFT key.
	 *
	 * @since 0.0.0
	 */
	Key* rshift = nullptr;
};

}	// namespace Zen

#endif
