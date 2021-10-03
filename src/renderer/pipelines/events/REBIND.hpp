/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_PIPELINES_EVENTS_REBIND_HPP
#define ZEN_RENDERER_PIPELINES_EVENTS_REBIND_HPP

#include <string>

namespace Zen {
namespace Events {

/**
 * The Pipeline ReBind Event.
 *
 * This event is dispatched by a Pipeline when it is re-bound by the
 * Pipeline Manager.
 *
 * @since 0.0.0
 *
 * @param pipeline Pointer to the pipeline that was rebound.
 * @param currentShader Pointer to the shader that was set as being current.
 */
const std::string PIPELINE_REBIND = "pipelinerebind";

}	// namespace Events
}	// namespace Zen

#endif
