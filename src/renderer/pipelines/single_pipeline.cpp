/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "single_pipeline.hpp"
#include "../shaders/single_frag.hpp"
#include "../shaders/single_vert.hpp"

namespace Zen {

SinglePipeline::SinglePipeline (PipelineConfig config)
	: MultiPipeline(prepareConfig(config))
{}

PipelineConfig SinglePipeline::prepareConfig (PipelineConfig config)
{
	config.fragShader = Shaders::SINGLE_FRAG;
	config.vertShader = Shaders::SINGLE_VERT;
	config.forceZero = true;

	return config;
}

void SinglePipeline::boot ()
{
	Pipeline::boot();

	set("uMainSampler", 0);
}

}	// namespace Zen
