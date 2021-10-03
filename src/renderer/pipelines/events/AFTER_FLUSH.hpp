/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_PIPELINES_EVENTS_AFTER_FLUSH_HPP
#define ZEN_RENDERER_PIPELINES_EVENTS_AFTER_FLUSH_HPP

#include <string>

namespace Zen {
namespace Events {

/**
 * The Pipeline After Flush Event.
 *
 * This event is dispatched by a Pipeline right after it has issued a
 * drawArrays command and cleared its vertex count.
 *
 * @since 0.0.0
 *
 * @param pipeline Pointer to the pipeline that has flushed.
 * @param isPostFlush Was this flush invoked as part of a post-process, or not?
 */
const std::string PIPELINE_AFTER_FLUSH = "pipelineafterflush";

}	// namespace Events
}	// namespace Zen

#endif
