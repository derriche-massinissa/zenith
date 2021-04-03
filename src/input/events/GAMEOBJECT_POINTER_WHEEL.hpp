/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Game Object Pointer Wheel Event.
 *
 * This event is dispatched by an interactive Game Object if a pointer has its
 * wheel moved while over it.
 *
 * Listen to this event from a Game Object using: `on(gameObject, "wheel", listener)`.
 * Note that the scope of the listener is automatically set to be the Game Object
 * instance itself.
 *
 * To receive this event, the Game Object must have been set as interactive.
 * See Zen::SetInteractive for more details.
 *
 * The event hierarchy is as follows:
 *
 * 1. ZEN_INPUT_GAMEOBJECT_POINTER_WHEEL
 * 2. ZEN_INPUT_GAMEOBJECT_WHEEL
 * 3. ZEN_INPUT_POINTER_WHEEL
 *
 * With the top event being dispatched first and then flowing down the list. Note
 * that higher-up event handlers can stop the propagation of this event.
 *
 * @since 0.0.0
 * 
 * @param pointer The Pointer responsible for triggering this event.
 * @param deltaX The horizontal scroll amount that occurred due to the user moving a mouse wheel or similar input device.
 * @param deltaY The vertical scroll amount that occurred due to the user moving a mouse wheel or similar input device. This value will typically be less than 0 if the user scrolls up and greater than zero if scrolling down.
 * @param deltaZ The z-axis scroll amount that occurred due to the user moving a mouse wheel or similar input device.
 * @param event The Phaser input event. You can call `stopPropagation()` to halt it from going any further in the event flow.
 */
#define ZEN_INPUT_GAMEOBJECT_POINTER_WHEEL "wheel"
