/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_PIPELINES_SINGLE_HPP
#define ZEN_RENDERER_PIPELINES_SINGLE_HPP

#include "multi_pipeline.hpp"

namespace Zen {

class SinglePipeline : public MultiPipeline
{
public:
	SinglePipeline (PipelineConfig config);

	PipelineConfig prepareConfig (PipelineConfig config);

	void boot ();
};

}	// namespace Zen

#endif
