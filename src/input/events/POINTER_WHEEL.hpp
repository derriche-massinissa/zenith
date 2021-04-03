/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Pointer Wheel Input Event.
 *
 * This event is dispatched by the Input Plugin belonging to a Scene if a pointer
 * has its wheel updated.
 *
 * Listen to this event from within a Scene using: `input.on("wheel",
 * listener)`.
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
 * @param currentlyOver An array containing all interactive Game Objects that the pointer was over when the event was created.
 * @param deltaX The horizontal scroll amount that occurred due to the user moving a mouse wheel or similar input device.
 * @param deltaY The vertical scroll amount that occurred due to the user moving a mouse wheel or similar input device. This value will typically be less than 0 if the user scrolls up and greater than zero if scrolling down.
 * @param deltaZ The z-axis scroll amount that occurred due to the user moving a mouse wheel or similar input device.
 */
#define ZEN_INPUT_POINTER_WHEEL "wheel"
