/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Game Object Up Input Event.
 *
 * This event is dispatched by the Input Plugin belonging to a Scene if a pointer
 * is released while over _any_ interactive Game Object.
 *
 * Listen to this event from within a Scene using: `input.on("gameobjectup", listener)`.
 *
 * To receive this event, the Game Objects must have been set as interactive.
 * See Zen::SetInteractive for more details.
 *
 * To listen for this event from a _specific_ Game Object, use the ZEN_INPUT_GAMEOBJECT_POINTER_UP event instead.
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
 * @param gameObject The Game Object the pointer was over when released.
 * @param event The Phaser input event. You can call `stopPropagation()` to halt it from going any further in the event flow.
 */
#define ZEN_INPUT_GAMEOBJECT_UP "gameobjectup"
