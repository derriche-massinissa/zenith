/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource->org/licenses/MIT">MIT License</a>
 */

#include "utility_pipeline.hpp"
#include "../renderer.hpp"
#include "../../display/color_matrix.hpp"
#include "../../utils/file/file_to_string.hpp"

#include "../shaders/quad_vert.hpp"
#include "../shaders/add_blend_frag.hpp"
#include "../shaders/copy_frag.hpp"
#include "../shaders/linear_blend_frag.hpp"
#include "../shaders/color_matrix_frag.hpp"

namespace Zen {

extern Renderer g_renderer;

PipelineConfig UtilityPipeline::setupConfig (PipelineConfig config)
{
	for (int i = 0; i < 4; i ++)
		config.renderTarget.emplace_back();

	config.vertShader = Shaders::QUAD_VERT;

	config.shaders = {
		{
			.name = "Copy",
			.fragShader = Shaders::COPY_FRAG
		},
		{
			.name = "AddBlend",
			.fragShader = Shaders::ADD_BLEND_FRAG
		},
		{
			.name = "LinearBlend",
			.fragShader = Shaders::LINEAR_BLEND_FRAG
		},
		{
			.name = "ColorMatrix",
			.fragShader = Shaders::COLOR_MATRIX_FRAG
		},
	};

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

	config.vertices = {
		-1, -1, 0, 0,
		-1, 1, 0, 1,
		1, 1, 1, 1,
		-1, -1, 0, 0,
		1, 1, 1, 1,
		1, -1, 1, 0
	};

	config.batchSize = 1;

	return config;
}

UtilityPipeline::UtilityPipeline (PipelineConfig config)
	: Pipeline (setupConfig(config))
{}

void UtilityPipeline::boot ()
{
	copyShader = shaders["Copy"].get();
	addShader = shaders["AddBlend"].get();
	linearShader = shaders["LinearBlend"].get();
	colorMatrixShader = shaders["ColorMatrix"].get();

	fullFrame1 = &renderTargets[0];
	fullFrame2 = &renderTargets[1];
	halfFrame1 = &renderTargets[2];
	halfFrame2 = &renderTargets[3];
}

void UtilityPipeline::copyFrame (RenderTarget* source, RenderTarget* target,
		double brightness, bool clear, bool clearAlpha)
{
	setShader(copyShader);
	set("uMainSampler", 0);
	set("uBrightness", brightness);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, source->texture);

	if (target->texture) {
		glViewport(0, 0, target->width, target->height);
		glBindFramebuffer(GL_FRAMEBUFFER, target->framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
				target->texture, 0);
	}
	else {
		glViewport(0, 0, source->width, source->height);
	}

	if (clear) {
		if (clearAlpha)
			glClearColor(0, 0, 0, 0);
		else
			glClearColor(0, 0, 0, 1);

		glClear(GL_COLOR_BUFFER_BIT);
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(std::uint8_t) * vertexData.size(),
			vertexData.data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void UtilityPipeline::blitFrame (RenderTarget* source, RenderTarget* target,
		double brightness, bool clear, bool clearAlpha, bool eraseMode)
{
	setShader(copyShader);
	set("uMainSampler", 0);
	set("uBrightness", brightness);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, source->texture);

	if (source->height > target->height) {
		glViewport(0, 0, source->width, source->height);
		setTargetUVs(source, target);
	}
	else {
		double diff = target->height - source->height;
		glViewport(0, diff, source->width, source->height);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, target->framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
			target->texture, 0);

	if (clear) {
		if (clearAlpha)
			glClearColor(0, 0, 0, 0);
		else
			glClearColor(0, 0, 0, 1);

		glClear(GL_COLOR_BUFFER_BIT);
	}

	int blendMode;
	if (eraseMode) {
		blendMode = g_renderer.currentBlendMode;
		g_renderer.setBlendMode(BLEND_MODE::ERASE);
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(std::uint8_t) * vertexData.size(),
			vertexData.data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	if (eraseMode) {
		g_renderer.setBlendMode(blendMode);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	resetUVs();
}

void UtilityPipeline::copyFrameRect (RenderTarget* source, RenderTarget*
		target, int x, int y, int width, int height, bool clear, bool clearAlpha)
{
	glBindFramebuffer(GL_FRAMEBUFFER, source->framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
			source->texture, 0);

	if (clear) {
		if (clearAlpha)
			glClearColor(0, 0, 0, 0);
		else
			glClearColor(0, 0, 0, 1);

		glClear(GL_COLOR_BUFFER_BIT);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, target->texture);

	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, x, y, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void UtilityPipeline::copyToGame (RenderTarget* source)
{
	setShader(copyShader);
	set("uMainSampler", 0);
	set("uBrightness", 1);

	g_renderer.popFramebuffer();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, source->texture);

	glBufferData(GL_ARRAY_BUFFER, sizeof(std::uint8_t) * vertexData.size(),
			vertexData.data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	g_renderer.resetTextures();
}

void UtilityPipeline::drawFrame (RenderTarget* source, RenderTarget* target,
		bool clearAlpha, ColorMatrix* colorMatrix)
{
	setShader(colorMatrixShader);

	set("uMainSampler", 0);
	set("uColorMatrix", GetData(colorMatrix));
	set("uAlpha", colorMatrix->alpha);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, source->texture);

	if (target->texture) {
		glViewport(0, 0, target->width, target->height);
		glBindFramebuffer(GL_FRAMEBUFFER, target->framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, target->texture, 0);
	}
	else {
		glViewport(0, 0, source->width, source->height);
	}

	if (clearAlpha) {
		glClearColor(0, 0, 0, 0);
	}
	else {
		glClearColor(0, 0, 0, 1);
	}

	glClear(GL_COLOR_BUFFER_BIT);

	glBufferData(GL_ARRAY_BUFFER, sizeof(std::uint8_t) * vertexData.size(),
			vertexData.data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void UtilityPipeline::blendFrames (RenderTarget* source1, RenderTarget* source2,
		RenderTarget* target, double strength, bool clearAlpha,
		Shader* blendShader)
{
	setShader(blendShader);

	set("uMainSampler1", 0);
	set("uMainSampler2", 1);
	set("uStrength", strength);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, source1->texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, source2->texture);

	if (target->texture) {
		glBindFramebuffer(GL_FRAMEBUFFER, target->framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, target->texture, 0);
		glViewport(0, 0, target->width, target->height);
	}
	else {
		glViewport(0, 0, source1->width, source1->height);
	}

	if (clearAlpha) {
		glClearColor(0, 0, 0, 0);
	}
	else {
		glClearColor(0, 0, 0, 1);
	}

	glClear(GL_COLOR_BUFFER_BIT);

	glBufferData(GL_ARRAY_BUFFER, sizeof(std::uint8_t) * vertexData.size(),
			vertexData.data(), GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void UtilityPipeline::blendFramesAdditive (RenderTarget* source1, RenderTarget*
		source2, RenderTarget* target, double strength, bool clearAlpha)
{
	blendFrames(source1, source2, target, strength, clearAlpha, addShader);
}

void UtilityPipeline::clearFrame (RenderTarget* target, bool clearAlpha)
{
	glViewport(0, 0, target->width, target->height);

	glBindFramebuffer(GL_FRAMEBUFFER, target->framebuffer);

	if (clearAlpha)
	{
		glClearColor(0, 0, 0, 0);
	}
	else
	{
		glClearColor(0, 0, 0, 1);
	}

	glClear(GL_COLOR_BUFFER_BIT);

	auto &fbo = g_renderer.currentFramebuffer;

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void UtilityPipeline::setUVs (double uA, double vA, double uB, double vB, double uC,
		double vC, double uD, double vD)
{
	float *vertexViewF32 = reinterpret_cast<float*>(vertexData.data());

	vertexViewF32[2] = uA;
	vertexViewF32[3] = vA;
	vertexViewF32[6] = uB;
	vertexViewF32[7] = vB;
	vertexViewF32[10] = uC;
	vertexViewF32[11] = vC;
	vertexViewF32[14] = uA;
	vertexViewF32[15] = vA;
	vertexViewF32[18] = uC;
	vertexViewF32[19] = vC;
	vertexViewF32[22] = uD;
	vertexViewF32[23] = vD;
}

void UtilityPipeline::setTargetUVs (RenderTarget* source, RenderTarget* target)
{
	double diff = (target->height / source->height);

	if (diff > 0.5)
		diff = 0.5 - (diff - 0.5);
	else
		diff = 0.5 + (0.5 - diff);

	setUVs(0, diff, 0, 1 + diff, 1, 1 + diff, 1, diff);
}

void UtilityPipeline::flipX ()
{
	setUVs(1, 0, 1, 1, 0, 1, 0, 0);
}

void UtilityPipeline::flipY ()
{
	setUVs(0, 1, 0, 0, 1, 0, 1, 1);
}

void UtilityPipeline::resetUVs ()
{
	setUVs(0, 0, 0, 1, 1, 1, 1, 0);
}

}	// namespace Zen
