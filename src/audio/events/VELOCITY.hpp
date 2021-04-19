/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Audio Velocity Event.
 *
 * This event is dispatched by the AudioManager when an audio source has it's
 * velocity changed.
 *
 * ```cpp
 * var ambiance = audio.add("key");
 * audio.on(ambiance, "position", listener);
 * audio.play(ambiance);
 * audio.setVelocity(ambiance, 10, 145);
 * ```
 *
 * @since 0.0.0
 *
 * @param x The `x` velocity of the audio source.
 * @param y The `y` velocity of the audio source.
 * @param z The `z` velocity of the audio source.
 */
#define ZEN_AUDIO_EVENTS_VELOCITY "velocity"
