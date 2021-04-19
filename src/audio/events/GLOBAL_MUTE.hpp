/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Audio Global Mute Event.
 *
 * This event is dispatched by the AudioManager when its mute property changes.
 *
 * ```cpp
 * audio.on("mute", listener);
 * audio.setMute(true);
 * ```
 *
 * @since 0.0.0
 *
 * @param mute The mute value. `true` if the manager is muted, otherwise `false`.
 */
#define ZEN_AUDIO_EVENTS_GLOBAL_MUTE "mute"
