/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_EVENTS_PRE_RENDER_HPP
#define ZEN_RENDERER_EVENTS_PRE_RENDER_HPP

#include <string>

namespace Zen {
namespace Events {

/**
 * The Pre-Render Event.
 *
 * This event is dispatched by the Renderer. This happens right at the start of
 * the render process, after the context has been cleared, the scissors enabled
 * and everything has been reset ready for the render.
 *
 * @since 0.0.0
 */
const std::string RENDER_PRE_RENDER = "prerender";

}	// namespace Events
}	// namespace Zen

#endif
