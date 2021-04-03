/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Pointer Drop Input Event.
 *
 * This event is dispatched by the InputPlugin belonging to a Scene if a pointer
 * drops a Game Object on a Drag Target.
 *
 * Listen to this event from within a Scene using: `input.on("drop",
 * listener)`.
 *
 * To listen for this event from a _specific_ Game Object, use the
 * ZEN_INPUT_GAMEOBJECT_DROP event instead.
 *
 * @since 0.0.0
 *
 * @param pointer The Pointer responsible for triggering this event.
 * @param gameObject The interactive Game Object that this pointer was dragging.
 * @param target The Drag Target the `gameObject` has been dropped on.
 */
#define ZEN_INPUT_DROP "drop"
