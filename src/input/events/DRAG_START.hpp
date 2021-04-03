/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Pointer Drag Start Input Event.
 *
 * This event is dispatched by the InputPlugin belonging to a Scene if a pointer
 * starts to drag any Game Object.
 *
 * Listen to this event from within a Scene using: `input.on("dragstart",
 * listener)`.
 *
 * A Pointer can only drag a single Game Object at once.
 *
 * To listen for this event from a _specific_ Game Object, use the
 * ZEN_INPUT_GAMEOBJECT_DRAG_START event instead.
 *
 * @since 0.0.0
 *
 * @param pointer The Pointer responsible for triggering this event.
 * @param gameObject The interactive Game Object that this pointer is dragging.
 */
#define ZEN_INPUT_DRAG_START "dragstart"
