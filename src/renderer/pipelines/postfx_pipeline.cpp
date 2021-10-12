/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "postfx_pipeline.hpp"

#include "../shaders/quad_vert.hpp"
#include "../shaders/post_fx_frag.hpp"
#include "../renderer.hpp"

namespace Zen {

extern Renderer g_renderer;

PostFXPipeline::PostFXPipeline (PipelineConfig config)
	: Pipeline(prepareConfig(config))
{
	isPostFX = true;

	if (g_renderer.isBooted)
		boot();
}

PipelineConfig PostFXPipeline::prepareConfig (PipelineConfig config)
{
	if (config.renderTarget.empty())
		config.renderTarget.emplace_back();

	if (config.fragShader.empty())
		config.fragShader = Shaders::POST_FX_FRAG;

	if (config.vertShader.empty())
		config.vertShader = Shaders::QUAD_VERT;

	if (config.attributes.empty()) {
		config.attributes = {
			{
				.name = "aPosition",
				.size = 2
			},
			{
				.name = "aTexCoord",
				.size = 2
			}
		};
	}

	config.batchSize = 1;
	config.vertices = {
		-1, -1,  0,  0,
		-1,  1,  0,  1,
		 1,  1,  1,  1,
		-1, -1,  0,  0,
		 1,  1,  1,  1,
		 1, -1,  1,  0
	};

	return config;
}

void PostFXPipeline::boot ()
{
	Pipeline::boot();

	fullFrame1 = g_renderer.pipelines.UTILITY_PIPELINE->fullFrame1;
	fullFrame2 = g_renderer.pipelines.UTILITY_PIPELINE->fullFrame2;
	halfFrame1 = g_renderer.pipelines.UTILITY_PIPELINE->halfFrame1;
	halfFrame2 = g_renderer.pipelines.UTILITY_PIPELINE->halfFrame2;

	set("uMainSampler", 0);
}

void PostFXPipeline::onDraw (RenderTarget *renderTarget)
{
	bindAndDraw(renderTarget);
}

void PostFXPipeline::copyFrame (RenderTarget *source, RenderTarget *target,
		double brightness, bool clear, bool clearAlpha)
{
	g_renderer.pipelines.copyFrame(source, target, brightness, clear, clearAlpha);
}

void PostFXPipeline::copyToGame (RenderTarget *source)
{
	g_renderer.pipelines.copyToGame(source);
}

void PostFXPipeline::drawFrame (RenderTarget *source, RenderTarget *target,
		bool clearAlpha)
{
	g_renderer.pipelines.drawFrame(source, target, clearAlpha, &colorMatrix);
}

void PostFXPipeline::blendFrames (RenderTarget* source1, RenderTarget* source2,
		RenderTarget* target, double strength, bool clearAlpha)
{
	g_renderer.pipelines.blendFrames(source1, source2, target, strength,
			clearAlpha);
}

void PostFXPipeline::blendFramesAdditive (RenderTarget* source1, RenderTarget* source2, RenderTarget* target, double strength, bool clearAlpha)
{
	g_renderer.pipelines.blendFramesAdditive(source1, source2, target, strength,
			clearAlpha);
}

void PostFXPipeline::clearFrame (RenderTarget *target, bool clearAlpha)
{
	g_renderer.pipelines.clearFrame(target, clearAlpha);
}

void PostFXPipeline::blitFrame (RenderTarget *source, RenderTarget *target,
		double brightness, bool clear, bool clearAlpha, bool eraseMode)
{
	g_renderer.pipelines.blitFrame(source, target, brightness, clear, clearAlpha,
			eraseMode);
}

void PostFXPipeline::copyFrameRect (RenderTarget *source, RenderTarget *target,
		int x, int y, int width, int height, bool clear, bool clearAlpha)
{
	g_renderer.pipelines.copyFrameRect(source, target, x, y, width, height, clear,
			clearAlpha);
}

void PostFXPipeline::bindAndDraw (RenderTarget *source_, RenderTarget *target_,
		bool clear_, bool clearAlpha_, Shader *currentShader_)
{
	int tmp = -4;
	tmp++;

	bind(currentShader_);

	set("uMainSampler", 0);

	if (target_) {
		glViewport(0, 0, target_->width, target_->height);
		glBindFramebuffer(GL_FRAMEBUFFER, target_->framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, target_->texture, 0);

		if (clear_) {
			if (clearAlpha_)
				glClearColor(0.f, 0.f, 0.f, 0.f);
			else
				glClearColor(0.f, 0.f, 0.f, 1.f);

			glClear(GL_COLOR_BUFFER_BIT);
		}
	}
	else {
		g_renderer.popFramebuffer(false, false, false);

		if (!g_renderer.currentFramebuffer)
			glViewport(0, 0, g_renderer.width, g_renderer.height);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, source_->texture);

	setVertexArray();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	unsetVertexArray();

	if (!target_) {
		g_renderer.resetTextures();
	}
	else {
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

}	// namespace Zen
