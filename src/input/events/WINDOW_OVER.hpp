/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Input Plugin Window Over Event.
 *
 * This event is dispatched by the InputPlugin if the active pointer enters the
 * window and is now over of it, having previously been outside.
 *
 * Listen to this event from within a Scene using: `input.on("windowover",
 * listener)`.
 *
 * @since 0.0.0
 * 
 * @param time The current time. Either a High Resolution Timer value if it comes from Request Animation Frame, or Date.now if using SetTimeout.
 * @param event The DOM Event that triggered the canvas over.
 */
#define ZEN_INPUT_WINDOW_OVER "windowover"
