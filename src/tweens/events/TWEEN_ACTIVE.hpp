/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Tween Active Event.
 * 
 * This event is dispatched by a Tween when it becomes active within the
 * TweenManager.
 * 
 * An 'active' Tween is one that is now progressing, although it may not yet be
 * updating any target, due to settings such as `delay`. If you need an event for
 * when the Tween starts actually updating its first property, see `TWEEN_START`.
 * 
 * Listen to it from a Tween instance using `Tween.on("active", listener)`, i.e.:
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
 * tween.on("active", listener);
 * ```
 *
 * @since 0.0.0
 * 
 * @param tween A pointer to the Tween instance that emitted the event.
 * @param targets An array of target/s the Tween is operating on.
 */
#define ZEN_TWEENS_EVENTS_TWEEN_ACTIVE "active"
