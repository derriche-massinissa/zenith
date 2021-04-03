/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Game Object Drag Start Event.
 *
 * This event is dispatched by an interactive Game Object if a pointer starts to
 * drag it.
 *
 * Listen to this event from a Game Object using: `on(gameObject, "dragstart",
 * listener)`.
 * Note that the scope of the listener is automatically set to be the Game Object
 * instance itself.
 *
 * To receive this event, the Game Object must have been set as interactive and
 * enabled for drag.
 * See Zen::SetInteractive for more details.
 *
 * There are lots of useful drag related properties that are set within the Game
 * Object when dragging occurs.
 * For example, `input.dragStartX`, `dragStartY` and so on.
 *
 * @since 0.0.0
 *
 * @param pointer The Pointer responsible for triggering this event.
 * @param dragX The x coordinate where the Pointer is currently dragging the Game Object, in world space.
 * @param dragY The y coordinate where the Pointer is currently dragging the Game Object, in world space.
 */
#define ZEN_INPUT_GAMEOBJECT_DRAG_START "dragstart"
