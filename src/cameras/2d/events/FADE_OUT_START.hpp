/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Camera Fade Out Start Event.
 * 
 * This event is dispatched when the Fade Out Effect of a camera starts.
 * 
 * Listen to it using `events.on("camerafadeoutstart", listener)`.
 *
 * @since 0.0.0
 * 
 * @param camera The camera that the effect began on.
 * @param duration The duration of the effect.
 * @param red The red color channel value.
 * @param green The green color channel value.
 * @param blue The blue color channel value.
 */
#define ZEN_EVENTS_CAMERAS_FADE_OUT_START "camerafadeoutstart"
