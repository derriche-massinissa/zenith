/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_INPUT_CONST_HPP
#define ZEN_INPUT_CONST_HPP

namespace Zen {

enum class INPUT
{
    /**
     * The mouse pointer is being held down.
     *
     * @since 0.0.0
     */
    MOUSE_DOWN = 0,

    /**
     * The mouse pointer is being moved.
     *
     * @since 0.0.0
     */
    MOUSE_MOVE,

    /**
     * The mouse pointer is released.
     *
     * @since 0.0.0
     */
    MOUSE_UP,

    /**
     * A touch pointer has been started.
     *
     * @since 0.0.0
     */
    TOUCH_START,

    /**
     * A touch pointer has been started.
     *
     * @since 0.0.0
     */
    TOUCH_MOVE,

    /**
     * A touch pointer has been started.
     *
     * @since 0.0.0
     */
    TOUCH_END,

    /**
     * The pointer lock has changed.
     *
     * @since 0.0.0
     */
    POINTER_LOCK_CHANGE,

    /**
     * A touch pointer has been been cancelled by the browser.
     *
     * @since 0.0.0
     */
    TOUCH_CANCEL,

    /**
     * The mouse wheel changes.
     *
     * @since 0.0.0
     */
    MOUSE_WHEEL
};

}	// namespace Zen

#endif
