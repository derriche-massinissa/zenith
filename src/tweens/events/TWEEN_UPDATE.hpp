/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Tween Update Event.
 * 
 * This event is dispatched by a Tween every time it invokes it's action function.
 *
 * A Tween that has 3 actions will emit this event 3 times per change, once per
 * action.
 * 
 * **Note:** This is a very high frequency event and may be dispatched multiple
 * times, every single frame.
 * 
 * Listen to it from a Tween instance using `Tween.on("update", listener)`, i.e.:
 * 
 * ```cpp
 * Tween& tween = tweens.add({
 *     .targets = { image },
 *     .entry = {
 *			.action = SetX,
 *			.from = GetX(image),
 *			.to = 500
 *		},
 *     .easing = EASE::POWER_1,
 *     .duration = 3000
 * });
 * tween.on("update", listener);
 * ```
 *
 * @since 0.0.0
 * 
 * @param tween A pointer to the Tween instance that emitted the event.
 * @param target The target entity that was updated.
 * @param current The current value that was tweened.
 * @param previous The previous value that was tweened, prior to this update.
 */
#define ZEN_TWEENS_EVENTS_TWEEN_UPDATE "update"
