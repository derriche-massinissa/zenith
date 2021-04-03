/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Game Object Drag Over Event.
 *
 * This event is dispatched by an interactive Game Object if a pointer drags it
 * over a drag target.
 *
 * When the Game Object first enters the drag target it will emit a `dragenter`
 * event. If it then moves while within the drag target, it will emit this event
 * instead.
 *
 * Listen to this event from a Game Object using: `on(gameObject, "dragover",
 * listener)`.
 * Note that the scope of the listener is automatically set to be the Game Object
 * instance itself.
 *
 * To receive this event, the Game Object must have been set as interactive and
 * enabled for drag.
 * See Zen::SetInteractive for more details.
 *
 * @since 0.0.0
 *
 * @param pointer The Pointer responsible for triggering this event.
 * @param target The drag target that this pointer has moved over.
 */
#define ZEN_INPUT_GAMEOBJECT_DRAG_OVER "dragover"
