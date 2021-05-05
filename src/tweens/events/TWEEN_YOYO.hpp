/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Tween Yoyo Event.
 * 
 * This event is dispatched by a Tween whenever a property it is tweening yoyos.
 * 
 * This event will only be dispatched if the Tween has an action with `yoyo` set.
 * 
 * If the Tween has a `hold` value, this event is dispatched when the hold expires.
 * 
 * This event is dispatched for every action, and for every target, that yoyos.
 * For example, if a Tween was updating 2 properties and had 10 targets, this event
 * would be dispatched 20 times (twice per target). So be careful how you use it!
 * 
 * Listen to it from a Tween instance using `Tween.on("yoyo", listener)`, i.e.:
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
 *     .yoyo = true
 * });
 * tween.on("yoyo", listener);
 * ```
 *
 * @since 0.0.0
 * 
 * @param tween A pointer to the Tween instance that emitted the event.
 * @param target The target object that was yoyo'd.
 */
#define ZEN_TWEENS_EVENTS_TWEEN_YOYO "yoyo"
