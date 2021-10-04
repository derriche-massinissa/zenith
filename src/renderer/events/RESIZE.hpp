/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_EVENTS_RESIZE_HPP
#define ZEN_RENDERER_EVENTS_RESIZE_HPP

#include <string>

namespace Zen {
namespace Events {

/**
 * The Renderer Resize Event.
 *
 * This event is dispatched by the Renderer when it is resized, usually as a
 * result of the Scale Manager resizing.
 *
 * @since 0.0.0
 *
 * @param width The new width of the renderer.
 * @param height The new height of the renderer.
 */
const std::string RENDER_RESIZE = "resize";

}	// namespace Events
}	// namespace Zen

#endif
