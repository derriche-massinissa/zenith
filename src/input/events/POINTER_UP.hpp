/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Pointer Up Input Event.
 *
 * This event is dispatched by the InputPlugin belonging to a Scene if a pointer is released anywhere.
 *
 * Listen to this event from within a Scene using: `input.on("pointerup", listener)`.
 *
 * The event hierarchy is as follows:
 *
 * 1. ZEN_INPUT_GAMEOBJECT_POINTER_UP
 * 2. ZEN_INPUT_GAMEOBJECT_UP
 * 3. ZEN_INPUT_POINTER_UP
 *
 * With the top event being dispatched first and then flowing down the list. Note
 * that higher-up event handlers can stop the propagation of this event.
 *
 * @since 0.0.0
 * 
 * @param pointer The Pointer responsible for triggering this event.
 * @param currentlyOver An array containing all interactive Game Objects that the pointer was over when the event was created.
 */
#define ZEN_INPUT_POINTER_UP "pointerup"
