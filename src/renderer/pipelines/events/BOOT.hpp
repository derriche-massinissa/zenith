/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_PIPELINES_EVENTS_BOOT_HPP
#define ZEN_RENDERER_PIPELINES_EVENTS_BOOT_HPP

#include <string>

namespace Zen {
namespace Events {

/**
 * The Pipeline Boot Event.
 *
 * This event is dispatched by a Pipeline when it has completed its `boot` phase.
 *
 * @since 0.0.0
 *
 * @param pipeline Pointer to the pipeline that booted.
 */
const std::string PIPELINE_BOOT = "pipelineboot";

}	// namespace Events
}	// namespace Zen

#endif
