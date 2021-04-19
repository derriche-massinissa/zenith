/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Audio Looped Event.
 *
 * This event is dispatched by the AudioManager when a sound loops during playback.
 *
 * ```cpp
 * var ambiance = audio.add("key");
 * audio.on(ambiance, "looped", listener);
 * audio.setLoop(ambiance, true);
 * audio.play(ambiance);
 * ```
 *
 * This is not to be confused with the ZEN_AUDIO_EVENTS_LOOP event, which only
 * emits when the loop state of a sound is changed.
 *
 * @since 0.0.0
 */
#define ZEN_AUDIO_EVENTS_LOOPED "looped"
