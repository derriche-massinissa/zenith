/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Audio Loop Event.
 *
 * This event is dispatched by the AudioManager when a sound have its loop state
 * changed.
 *
 * ```cpp
 * var ambiance = audio.add("key");
 * audio.on(ambiance, "loop", listener);
 * audio.setLoop(ambiance, true);
 * ```
 *
 * This is not to be confused with the ZEN_AUDIO_EVENTS_LOOPED event, which emits
 * eachs time a sound loops during playback.
 *
 * @since 0.0.0
 *
 * @param loop `true` if the sound loops, `false` otherwise.
 */
#define ZEN_AUDIO_EVENTS_LOOP "loop"
