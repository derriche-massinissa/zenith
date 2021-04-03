/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Game Object Pointer Over Event.
 *
 * This event is dispatched by an interactive Game Object if a pointer moves over
 * it.
 *
 * Listen to this event from a Game Object using: `on(gameObject, "pointerover",
 * listener)`.
 * Note that the scope of the listener is automatically set to be the Game Object
 * instance itself.
 *
 * To receive this event, the Game Object must have been set as interactive.
 * See Zen::SetInteractive for more details.
 *
 * The event hierarchy is as follows:
 *
 * 1. ZEN_INPUT_GAMEOBJECT_POINTER_OVER
 * 2. ZEN_INPUT_GAMEOBJECT_OVER
 * 3. ZEN_INPUT_POINTER_OVER
 *
 * With the top event being dispatched first and then flowing down the list. Note that higher-up event handlers can stop
 * the propagation of this event.
 *
 * @since 0.0.0
 * 
 * @param pointer The Pointer responsible for triggering this event.
 * @param localX The x coordinate that the Pointer interacted with this object on, relative to the Game Object's top-left position.
 * @param localY The y coordinate that the Pointer interacted with this object on, relative to the Game Object's top-left position.
 * @param event The Phaser input event. You can call `stopPropagation()` to halt it from going any further in the event flow.
 */
#define ZEN_INPUT_GAMEOBJECT_POINTER_OVER "pointerover"
