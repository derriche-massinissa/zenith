/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Pointer Drag End Input Event.
 *
 * This event is dispatched by the InputPlugin belonging to a Scene if a pointer
 * stops dragging a Game Object.
 *
 * Listen to this event from within a Scene using: `input.on("dragend", listener)`.
 *
 * To listen for this event from a _specific_ Game Object, use the
 * ZEN_INPUT_GAMEOBJECT_DRAG_END event instead.
 *
 * @since 0.0.0
 *
 * @param pointer The Pointer responsible for triggering this event.
 * @param gameObject The interactive Game Object that this pointer stopped dragging.
 */
#define ZEN_INPUT_DRAG_END "dragend"
