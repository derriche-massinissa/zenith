/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_PIPELINES_EVENTS_RESIZE_HPP
#define ZEN_RENDERER_PIPELINES_EVENTS_RESIZE_HPP

#include <string>

namespace Zen {
namespace Events {

/**
 * The Pipeline Resize Event.
 *
 * This event is dispatched by a Pipeline when it is resized, usually as a result
 * of the Renderer resizing.
 *
 * @since 0.0.0
 *
 * @param width The new width of the pipeline.
 * @param height The new height of the pipeline.
 * @param pipeline Pointer to the pipeline that was resized.
 */
const std::string PIPELINE_RESIZE = "pipelineresize";

}	// namespace Events
}	// namespace Zen

#endif
