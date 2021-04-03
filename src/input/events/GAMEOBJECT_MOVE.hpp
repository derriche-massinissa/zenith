/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Game Object Move Input Event.
 *
 * This event is dispatched by the InputPlugin belonging to a Scene if a pointer is
 * moved across _any_ interactive Game Object.
 *
 * Listen to this event from within a Scene using: `input.on("gameobjectmove",
 * listener)`.
 *
 * To receive this event, the Game Objects must have been set as interactive.
 * See Zen::SetInteractive for more details.
 *
 * To listen for this event from a _specific_ Game Object, use the
 * ZEN_INPUT_GAMEOBJECT_POINTER_MOVE event instead.
 *
 * The event hierarchy is as follows:
 *
 * 1. ZEN_INPUT_GAMEOBJECT_POINTER_MOVE
 * 2. ZEN_INPUT_GAMEOBJECT_MOVE
 * 3. ZEN_INPUT_POINTER_MOVE
 *
 * With the top event being dispatched first and then flowing down the list. Note
 * that higher-up event handlers can stop the propagation of this event.
 *
 * @since 0.0.0
 * 
 * @param pointer The Pointer responsible for triggering this event.
 * @param gameObject The Game Object the pointer was moved on.
 * @param event The input event. You can set `stopPropagation` to true to halt it
 * from going any further in the event flow.
 */
#define ZEN_INPUT_GAMEOBJECT_MOVE "gameobjectmove"
