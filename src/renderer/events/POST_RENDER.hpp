/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_EVENTS_POST_RENDER_HPP
#define ZEN_RENDERER_EVENTS_POST_RENDER_HPP

#include <string>

namespace Zen {
namespace Events {

/**
 * The Post-Render Event.
 *
 * This event is dispatched by the Renderer when all rendering, for all cameras
 * in all Scenes, has completed, but before any pending snap shots have been
 * taken.
 *
 * @since 0.0.0
 */
const std::string RENDER_POST_RENDER = "postrender";

}	// namespace Events
}	// namespace Zen

#endif
