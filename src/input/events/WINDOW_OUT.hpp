/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

/**
 * The Input Plugin Window Out Event.
 *
 * This event is dispatched by the Input Plugin if the active pointer leaves the
 * window and is now outside of it.
 *
 * Listen to this event from within a Scene using: `input.on("windowout",
 * listener)`.
 *
 * @since 0.0.0
 * 
 * @param time The current time. Either a High Resolution Timer value if it comes from Request Animation Frame, or Date.now if using SetTimeout.
 * @param event The DOM Event that triggered the canvas out.
 */
#define ZEN_INPUT_WINDOW_OUT "windowout"
