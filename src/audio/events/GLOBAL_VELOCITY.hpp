/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Audio Global Velocity Event.
 *
 * This event is dispatched by the AudioManager when its mute property changes.
 *
 * ```cpp
 * audio.on("velocity", listener);
 * audio.setVelocity(54, 78, 2);
 * ```
 *
 * @since 0.0.0
 *
 * @param x The `x` velocity of the listener.
 * @param y The `y` velocity of the listener.
 * @param z The `z` velocity of the listener.
 */
#define ZEN_AUDIO_EVENTS_GLOBAL_VELOCITY "velocity"
