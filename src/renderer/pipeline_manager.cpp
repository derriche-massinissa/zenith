/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "pipeline_manager.hpp"
#include "../utils/map/contains.hpp"
#include "renderer.hpp"
#include "pipelines/const.hpp"
#include "pipelines/graphics_pipeline.hpp"
#include "pipelines/single_pipeline.hpp"
#include "../systems/renderable.hpp"

namespace Zen {

extern entt::registry g_registry;
extern Renderer g_renderer;

PipelineManager::PipelineManager ()
{}

PipelineManager::~PipelineManager ()
{
	flush();
}

void PipelineManager::boot ()
{
	// Setup all the default pipelines
	add<UtilityPipeline>(Pipelines::UTILITY_PIPELINE);
	UTILITY_PIPELINE = static_cast<UtilityPipeline*>(
			pipelines[Pipelines::UTILITY_PIPELINE].get()
	);

	add<MultiPipeline>(Pipelines::MULTI_PIPELINE);
	add<SinglePipeline>(Pipelines::SINGLE_PIPELINE);
	add<BitmapMaskPipeline>(Pipelines::BITMAPMASK_PIPELINE);
	add<GraphicsPipeline>(Pipelines::GRAPHICS_PIPELINE);
	//add<LightPipeline>(Pipelines::LIGHT_PIPELINE);
	//add<PointLightPipeline>(Pipelines::POINTLIGHT_PIPELINE);
	//add<RopePipeline>(Pipelines::ROPE_PIPELINE);

	// Our const like references
	MULTI_PIPELINE = static_cast<MultiPipeline*>(
			pipelines[Pipelines::MULTI_PIPELINE].get()
	);
	BITMAPMASK_PIPELINE = static_cast<BitmapMaskPipeline*>(
			pipelines[Pipelines::BITMAPMASK_PIPELINE].get()
	);

	// FBO references
	fullFrame1 = UTILITY_PIPELINE->fullFrame1;
	fullFrame2 = UTILITY_PIPELINE->fullFrame2;
	halfFrame1 = UTILITY_PIPELINE->halfFrame1;
	halfFrame2 = UTILITY_PIPELINE->halfFrame2;
}

void PipelineManager::flush ()
{
	if (current)
		current->flush();
}

bool PipelineManager::has (std::string name)
{
	return Contains(pipelines, name);
}

Pipeline* PipelineManager::get (std::string name)
{
	if (Contains(pipelines, name))
		return pipelines.at(name).get();
	else
		return nullptr;
}

void PipelineManager::remove (std::string name)
{
	if (Contains(pipelines, name))
		pipelines.erase(pipelines.find(name));
}

Pipeline* PipelineManager::set (std::string name, Entity entity,
		Shader* currentShader)
{
	if (!Contains(pipelines, name))
		return nullptr;

	Pipeline *pipeline = pipelines[name].get();

	if (pipeline->isPostFX)
		return nullptr;

	if (!isCurrent(name, currentShader)) {
		flush();

		if (current)
			current->unbind();

		current = pipeline;

		pipeline->bind(currentShader);
	}

	pipeline->updateProjectionMatrix();

	pipeline->onBind(entity);

	return pipeline;
}

void PipelineManager::preBatch (Entity gameObject)
{
	Components::Renderable *renderable =
		g_registry.try_get<Components::Renderable>(gameObject);
	ZEN_ASSERT(renderable, "The entity has no 'Renderable' component");

	if (!renderable->postPipelines.empty()) {
		flush();

		auto &ps = renderable->postPipelines;

		// Iterate in reverse because we need them stacked in the order they're
		// in the array
		for (auto p = ps.rbegin(); p != ps.rend(); p++) {
			if ((*p)->active)
				(*p)->preBatch(gameObject);
		}
	}
}

void PipelineManager::postBatch (Entity gameObject)
{
	Components::Renderable *renderable =
		g_registry.try_get<Components::Renderable>(gameObject);
	ZEN_ASSERT(renderable, "The entity has no 'Renderable' component");

	if (!renderable->postPipelines.empty()) {
		flush();

		auto &ps = renderable->postPipelines;

		for (auto &p : ps) {
			if (p->active)
				p->postBatch(gameObject);
		}
	}
}

void PipelineManager::preBatchCamera (Entity camera)
{
	Components::Renderable *renderable =
		g_registry.try_get<Components::Renderable>(camera);
	ZEN_ASSERT(renderable, "The entity has no 'Renderable' component");

	if (!renderable->postPipelines.empty()) {
		flush();

		auto &ps = renderable->postPipelines;

		// Iterate in reverse because we need them stacked in the order they're
		// in the array
		for (auto p = ps.rbegin(); p != ps.rend(); p++) {
			if ((*p)->active)
				(*p)->preBatch(camera);
		}
	}
}

void PipelineManager::postBatchCamera (Entity camera)
{
	Components::Renderable *renderable =
		g_registry.try_get<Components::Renderable>(camera);
	ZEN_ASSERT(renderable, "The entity has no 'Renderable' component");

	if (!renderable->postPipelines.empty()) {
		flush();

		auto &ps = renderable->postPipelines;

		for (auto &p : ps) {
			if (p->active)
				p->postBatch(camera);
		}
	}
}

bool PipelineManager::isCurrent (std::string name, Shader* currentShader)
{
	if (!Contains(pipelines, name))
		return false;

	if (current && !currentShader)
		currentShader = current->currentShader;

	Pipeline *pipeline = pipelines[name].get();

	return !(current != pipeline ||
			currentShader->program != g_renderer.currentProgram);
}

void PipelineManager::copyFrame (RenderTarget* source, RenderTarget* target,
		double brightness, bool clear, bool clearAlpha)
{
	setUtility(UTILITY_PIPELINE->copyShader).copyFrame(source, target, brightness,
			clear, clearAlpha);
}

void PipelineManager::copyToGame (RenderTarget* source)
{
	setUtility(UTILITY_PIPELINE->copyShader).copyToGame(source);
}

void PipelineManager::drawFrame (RenderTarget* source, RenderTarget* target,
		bool clearAlpha, ColorMatrix* colorMatrix)
{
	setUtility(UTILITY_PIPELINE->colorMatrixShader).drawFrame(source, target,
			clearAlpha, colorMatrix);
}

void PipelineManager::blendFrames (RenderTarget* source1, RenderTarget* source2,
		RenderTarget* target, double strength, bool clearAlpha)
{
	setUtility(UTILITY_PIPELINE->linearShader).blendFrames(source1, source2, target,
			strength, clearAlpha);
}

void PipelineManager::blendFramesAdditive (RenderTarget* source1,
		RenderTarget* source2, RenderTarget* target, double strength,
		bool clearAlpha)
{
	setUtility(UTILITY_PIPELINE->addShader).blendFramesAdditive(source1,
			source2, target, strength, clearAlpha);
}

void PipelineManager::clearFrame (RenderTarget* target, bool clearAlpha)
{
	UTILITY_PIPELINE->clearFrame(target, clearAlpha);
}

void PipelineManager::blitFrame (RenderTarget* source, RenderTarget* target,
		double brightness, bool clear, bool clearAlpha, bool eraseMode)
{
	setUtility(UTILITY_PIPELINE->copyShader).blitFrame(source, target, brightness,
			clear, clearAlpha, eraseMode);
}

void PipelineManager::copyFrameRect (RenderTarget* source, RenderTarget* target,
		int x, int y, int width, int height, bool clear, bool clearAlpha)
{
	UTILITY_PIPELINE->copyFrameRect(source, target, x, y, width, height,
			clear, clearAlpha);
}

bool PipelineManager::forceZero ()
{
	return (current != nullptr && current->forceZero);
}

MultiPipeline& PipelineManager::setMulti ()
{
	return *MULTI_PIPELINE;
}

UtilityPipeline& PipelineManager::setUtility (Shader* currentShader)
{
	UTILITY_PIPELINE->bind(currentShader);
	return *UTILITY_PIPELINE;
}

void PipelineManager::rebind (std::string name)
{
	Pipeline *pipeline = nullptr;

	if (name.empty()) {
		pipeline = previous;
	}
	else {
		if (Contains(pipelines, name))
			pipeline = pipelines[name].get();
	}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	if (g_renderer.hasActiveStencilMask()) {
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	else {
		// If there wasn't a stencil mask set before this call, we can disable
		// it safely
		glDisable(GL_STENCIL_TEST);
		glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	glViewport(0, 0, g_renderer.width, g_renderer.height);

	g_renderer.currentProgram = 0;

	g_renderer.setBlendMode(0, true);

	if (pipeline) {
		current = pipeline;
		pipeline->rebind();
	}

	g_renderer.resetTextures();
}

void PipelineManager::clear ()
{
	flush();

	if (current) {
		current->unbind();
		previous = current;
		current = nullptr;
	}
	else {
		previous = nullptr;
	}

	g_renderer.currentProgram = 0;

	g_renderer.setBlendMode(0, true);
}

}	// namespace Zen
