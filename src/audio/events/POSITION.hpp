/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Audio Position Event.
 *
 * This event is dispatched by the AudioManager when an audio source is moved.
 *
 * ```cpp
 * var ambiance = audio.add("key");
 * audio.on(ambiance, "position", listener);
 * audio.play(ambiance);
 * audio.setPosition(ambiance, 10, 145);
 * ```
 *
 * @since 0.0.0
 *
 * @param x The `x` position of the audio source.
 * @param y The `y` position of the audio source.
 * @param z The `z` position of the audio source.
 */
#define ZEN_AUDIO_EVENTS_POSITION "position"
