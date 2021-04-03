/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Game Object Drop Event.
 *
 * This event is dispatched by an interactive Game Object if a pointer drops it on
 * a Drag Target.
 *
 * Listen to this event from a Game Object using: `on(gameObject, "drop",
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
 * @param target The Drag Target the `gameObject` has been dropped on.
 */
#define ZEN_INPUT_GAMEOBJECT_DROP "drop"
