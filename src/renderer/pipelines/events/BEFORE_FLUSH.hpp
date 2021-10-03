/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_PIPELINES_EVENTS_BEFORE_FLUSH_HPP
#define ZEN_RENDERER_PIPELINES_EVENTS_BEFORE_FLUSH_HPP

#include <string>

namespace Zen {
namespace Events {

/**
 * The Pipeline Before Flush Event.
 *
 * This event is dispatched by a Pipeline right before it is about to
 * flush and issue a bufferData and drawArrays command.
 *
 * @since 0.0.0
 *
 * @param pipeline Pointer to the pipeline that is about to flush.
 * @param isPostFlush Was this flush invoked as part of a post-process, or not?
 */
const std::string PIPELINE_BEFORE_FLUSH = "pipelinebeforeflush";

}	// namespace Events
}	// namespace Zen

#endif
