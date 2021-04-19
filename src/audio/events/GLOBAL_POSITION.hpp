/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Audio Global Position Event.
 *
 * This event is dispatched by the AudioManager when its position property changes.
 *
 * ```cpp
 * audio.on("position", listener);
 * audio.setPosition(54, 78, 2);
 * ```
 *
 * @since 0.0.0
 *
 * @param x The `x` position of the listener.
 * @param y The `y` position of the listener.
 * @param z The `z` position of the listener.
 */
#define ZEN_AUDIO_EVENTS_GLOBAL_POSITION "position"
