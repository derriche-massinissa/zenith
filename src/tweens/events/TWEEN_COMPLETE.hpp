/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Tween Complete Event.
 * 
 * This event is dispatched by a Tween when it completes playback entirely,
 * factoring in repeats and loops.
 * 
 * If the Tween has been set to loop or repeat infinitely, this event will not be
 * dispatched unless the Tween::stop method is called.
 * 
 * If a Tween has a `completeDelay` set, this event will fire after that delay
 * expires.
 * 
 * Listen to it from a Tween instance using `Tween.on("complete", listener)`, i.e.:
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
 * tween.on("complete", listener);
 * ```
 *
 * @since 0.0.0
 * 
 * @param tween A pointer to the Tween instance that emitted the event.
 * @param targets An array of target/s the Tween is operating on.
 */
#define ZEN_TWEENS_EVENTS_TWEEN_COMPLETE "complete"
