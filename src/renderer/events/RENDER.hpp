/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_EVENTS_RENDER_HPP
#define ZEN_RENDERER_EVENTS_RENDER_HPP

#include <string>

namespace Zen {
namespace Events {

/**
 * The Render Event.
 *
 * This event is dispatched by the Renderer for every camera in every Scene.
 *
 * It is dispatched before any of the children in the Scene have been rendered.
 *
 * @since 0.0.0
 *
 * @param camera The Scene Camera being rendered.
 */
const std::string RENDER_RENDER = "render";

}	// namespace Events
}	// namespace Zen

#endif
