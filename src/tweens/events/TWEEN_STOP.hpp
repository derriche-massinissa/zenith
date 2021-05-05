/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Tween Stop Event.
 *
 * This event is dispatched by a Tween when it is stopped.
 *
 * Listen to it from a Tween instance using `Tween.on("stop", listener)`, i.e.:
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
 * tween.on("stop", listener);
 * ```
 *
 * @since 0.0.0
 *
 * @param tween A pointer to the Tween instance that emitted the event.
 * @param targets An array of target/s the Tween is operating on.
 */
#define ZEN_TWEENS_EVENTS_TWEEN_STOP "stop"
