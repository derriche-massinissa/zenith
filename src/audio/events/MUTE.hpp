/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Audio Mute Event.
 *
 * This event is dispatched by the AudioManager when a sound has its mute state
 * changed.
 *
 * ```cpp
 * var ambiance = audio.add("key");
 * audio.on(ambiance, "mute", listener);
 * audio.play(ambiance);
 * audio.setMute(ambiance, true);
 * ```
 *
 * @since 0.0.0
 *
 * @param mute The mute value. `true` if the sound is now muted, otherwise `false`.
 */
#define ZEN_AUDIO_EVENTS_MUTE "mute"
