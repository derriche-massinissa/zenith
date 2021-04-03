/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Pointer Move Input Event.
 *
 * This event is dispatched by the InputPlugin belonging to a Scene if a pointer is
 * moved anywhere.
 *
 * Listen to this event from within a Scene using: `input.on("pointermove",
 * listener)`.
 *
 * The event hierarchy is as follows:
 *
 * 1. ZEN_EVENTS_GAMEOBJECT_POINTER_MOVE
 * 2. ZEN_EVENTS_GAMEOBJECT_MOVE
 * 3. ZEN_EVENTS_POINTER_MOVE
 *
 * With the top event being dispatched first and then flowing down the list. Note
 * that higher-up event handlers can stop the propagation of this event.
 *
 * @since 0.0.0
 *
 * @param pointer The Pointer responsible for triggering this event.
 * @param currentlyOver A vector containing all interactive Game Objects that the
 * pointer was over when the event was created.
 */
#define ZEN_INPUT_POINTER_MOVE "pointermove"
