/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Tween Repeat Event.
 * 
 * This event is dispatched by a Tween when one of the actions it is tweening
 * repeats.
 * 
 * This event will only be dispatched if the Tween has an action with a repeat
 * count set.
 * 
 * If a Tween has a `repeatDelay` set, this event will fire after that delay
 * expires.
 * 
 * The difference between `loop` and `repeat` is that `repeat` is an action setting,
 * where-as `loop` applies to the entire Tween.
 * 
 * Listen to it from a Tween instance using `Tween.on("repeat", listener)`, i.e.:
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
 *     .duration = 3000,
 *     .repeat = 4
 * });
 * tween.on("repeat", listener);
 * ```
 *
 * @since 0.0.0
 * 
 * @param tween A pointer to the Tween instance that emitted the event.
 * @param targets An array of target/s the Tween is operating on.
 */
#define ZEN_TWEENS_EVENTS_TWEEN_REPEAT "repeat"
