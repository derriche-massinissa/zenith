/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Audio Global Volume Event.
 *
 * This event is dispatched by the AudioManager when its volume property changes.
 *
 * ```cpp
 * audio.on("volume", listener);
 * audio.setVolume(0.5);
 * ```
 *
 * @since 0.0.0
 *
 * @param mute The mute value. `true` if the manager is muted, otherwise `false`.
 */
#define ZEN_AUDIO_EVENTS_GLOBAL_VOLUME "volume"
