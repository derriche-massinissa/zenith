/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Audio Volume Event.
 *
 * This event is dispatched by the AudioManager when a sound has its volume changed.
 *
 * ```cpp
 * var ambiance = audio.add("key");
 * audio.on(ambiance, "volume", listener);
 * audio.play(ambiance);
 * audio.setVolume(ambiance, 0.5);
 * ```
 *
 * @since 0.0.0
 *
 * @param volume The new volume.
 */
#define ZEN_AUDIO_EVENTS_VOLUME "volume"
