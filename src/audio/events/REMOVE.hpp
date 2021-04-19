/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Audio Remove Event.
 *
 * This event is dispatched by the AudioManager when a sound is removed.
 *
 * ```cpp
 * var ambiance = audio.add("key");
 * audio.on(ambiance, "remove", listener);
 * audio.remove(ambiance);
 * ```
 *
 * @since 0.0.0
 */
#define ZEN_AUDIO_EVENTS_REMOVE "remove"
