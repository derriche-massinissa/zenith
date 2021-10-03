/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "pipeline.hpp"
#include "renderer.hpp"
#include "utility.hpp"
#include "../utils/map/emplace.hpp"
#include "../texture/components/frame.hpp"
#include "../ecs/entity.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../systems/textured.hpp"

#include "pipelines/events/events.hpp"

namespace Zen {

extern Renderer g_renderer;
extern entt::registry g_registry;

Pipeline::Pipeline (PipelineConfig config)
	: name (config.name)
	, topology (config.topology)
	, forceZero (config.forceZero)
	, config (config)
{}

void Pipeline::boot()
{
	// TODO
}

void Pipeline::onBoot ()
{}

void Pipeline::onResize ([[maybe_unused]] double width,
		[[maybe_unused]] double height)
{}

void Pipeline::setShader (Shader *shader, bool setAttributes)
{
	if (shader != currentShader || g_renderer.currentProgram
			!= currentShader->program) {
		flush();

		g_renderer.resetTextures();

		bool wasBound = setVertexBuffer();

		if (wasBound && !setAttributes)
			setAttributes = true;

		shader->bind(setAttributes, false);
		
		currentShader = shader;
	}
}


Shader* Pipeline::getShaderByName (std::string name)
{
	return shaders[name].get();
}

void Pipeline::setCurrentShader (Shader *shader)
{
	currentShader = shader;
}

void Pipeline::setCurrentProgram (GLuint program)
{
	g_renderer.setProgram(program);
}

void Pipeline::setShadersFromConfig (PipelineConfig config)
{
	shaders.clear();

	std::string defaultVertShader = config.vertShader;
	std::string defaultFragShader = ParseFragmentShaderMaxTextures(
			config.fragShader, g_renderer.maxTextures);
	auto defaultAttribs = config.attributes;

	auto configShaders = config.shaders;

	if (configShaders.empty()) {
		if (!defaultVertShader.empty() && !defaultFragShader.empty()) {
			Emplace(&shaders, std::string("default"), std::make_unique<Shader>(
				"default", defaultVertShader, defaultFragShader, defaultAttribs
			));

			shaders.at("default")->on("set-program", &Pipeline::setCurrentProgram,
					this);
			shaders.at("default")->on("set-current", &Pipeline::setCurrentShader,
					this);
		}
	}
	else {
		for (auto &entry : configShaders) {
			std::string name = entry.name;
			std::string vertShader = entry.vertShader;
			std::string fragShader = ParseFragmentShaderMaxTextures(entry.vertShader, g_renderer.maxTextures);
			auto attributes = entry.attributes;

			if (!vertShader.empty() && !fragShader.empty()) {
				Emplace(&shaders, name, std::make_unique<Shader>(
					name, vertShader, fragShader, attributes
				));

				shaders.at(name)->on("set-program", &Pipeline::setCurrentProgram,
						this);
				shaders.at(name)->on("set-current", &Pipeline::setCurrentShader,
						this);
			}
		}
	}

	if (shaders.empty())
		MessageWarning("Pipeline: ", name, " - Invalid shader config");
	else
		currentShader = shaders.at(configShaders[0].name).get();
}


int Pipeline::setGameObject (Entity gameObject)
{
	auto frame = g_registry.try_get<Components::Frame>(GetFrame(gameObject));
	ZEN_ASSERT(frame, "The given entity has no 'frame' component");

	currentUnit = g_renderer.setTextureSource(frame->source);

	return currentUnit;
}


bool Pipeline::shouldFlush (int amount)
{
	return (vertexCount + amount > vertexCapacity);
}


void Pipeline::resize (double width_, double height_)
{
	if (width_ != width || height != height)
		flush();

	width_ = width;
	height_ = height;

	for (auto target : renderTargets)
		target.resize(width, height);

	setProjectionMatrix(width, height);

	emit("resize", width, height);

	onResize(width, height);
}

void Pipeline::setProjectionMatrix (double width_, double height_)
{
	// Not all pipeline have a projection matrix
	if (!projectionMatrix)
		return;

	projectionWidth = width_;
	projectionHeight = height_;

	*projectionMatrix = glm::ortho(0.f, (float)width_, (float)height_, 0.f);

	std::string name = "uProjectionMatrix";

	for (auto &s : shaders) {
		auto &shader = s.second;

		if (shader->hasUniform(name)) {
			shader->resetUniform(name);

			shader->set(name, false, *projectionMatrix);
		}
	}
}

void Pipeline::updateProjectionMatrix ()
{
	if (!projectionMatrix)
		return;

	double globalWidth_ = g_renderer.projectionWidth;
	double globalHeight_ = g_renderer.projectionHeight;

	if (projectionWidth != globalWidth_ || projectionHeight != globalHeight_)
		setProjectionMatrix(globalWidth_, globalHeight_);
}

void Pipeline::bind (Shader *currentShader_)
{
	if (currentShader_)
		currentShader = currentShader_;

	bool wasBound = setVertexBuffer();

	currentShader->bind(wasBound);

	emit(Events::PIPELINE_BIND, this, currentShader);

	onActive(currentShader);
}

void Pipeline::rebind ()
{
	setVertexBuffer();

	for (auto s = shaders.rbegin(); s != shaders.rend(); s++) {
		Shader *shader = s->second.get();
		currentShader = shader->rebind();
	}

	emit(Events::PIPELINE_REBIND, this, currentShader);

	onActive(currentShader);

	onRebind();
}

bool Pipeline::setVertexBuffer ()
{
	GLint buffer_;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &buffer_);

	if (buffer_ != vertexBuffer) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		return true;
	}

	return false;
}

void Pipeline::preBatch (Entity entity)
{
	if (currentRenderTarget)
		currentRenderTarget->bind();

	onPreBatch(entity);
}

void Pipeline::postBatch (Entity entity)
{
	onDraw(currentRenderTarget);

	onPostBatch(entity);
}

void Pipeline::onDraw ([[maybe_unused]] RenderTarget* renderTarget)
{}


void Pipeline::unbind ()
{
	if (currentRenderTarget)
		currentRenderTarget->unbind();
}


void Pipeline::flush (bool isPostFlush_)
{
	if (vertexCount > 0) {
		emit(Events::PIPELINE_BEFORE_FLUSH, isPostFlush_);

		onBeforeFlush(isPostFlush_);

		int vertexSize_ = currentShader->vertexSize;

		if (active) {
			setVertexBuffer();

			if (vertexCount == vertexCapacity)
				glBufferData(GL_ARRAY_BUFFER, vertexSize_, vertexData.data(),
						GL_DYNAMIC_DRAW);
			else
				glBufferSubData(GL_ARRAY_BUFFER, 0, vertexSize_, vertexData.data());

			glDrawArrays(topology, 0, vertexCount);
		}

		vertexCount = 0;

		emit(Events::PIPELINE_AFTER_FLUSH, isPostFlush_);

		onAfterFlush(isPostFlush_);
	}
}


void Pipeline::onActive ([[maybe_unused]] Shader* currentShader)
{}


void Pipeline::onBind ([[maybe_unused]] Entity entity)
{}


void Pipeline::onRebind ()
{}


void Pipeline::onBatch ([[maybe_unused]] Entity entity)
{}


void Pipeline::onPreBatch ([[maybe_unused]] Entity entity)
{}


void Pipeline::onPostBatch ([[maybe_unused]] Entity entity)
{}


void Pipeline::onPreRender ()
{}


void Pipeline::onRender ([[maybe_unused]] Entity camera)
{}


void Pipeline::onPostRender ()
{}

void Pipeline::onBeforeFlush ([[maybe_unused]] bool isPostFlush)
{}

void Pipeline::onAfterFlush ([[maybe_unused]] bool isPostFlush)
{}

void Pipeline::batchVert (double x, double y, double u, double v, int unit,
		int tintEffect, int tint)
{
	std::uint32_t *vertexViewU32 = reinterpret_cast<std::uint32_t*>(
			vertexData.data());
	float *vertexViewF32 = reinterpret_cast<float*>(vertexData.data());

	size_t vertexOffset = (vertexCount * currentShader->vertexComponentCount) - 1;

	vertexViewF32[++vertexOffset] = (float) x;
	vertexViewF32[++vertexOffset] = (float) y;
	vertexViewF32[++vertexOffset] = (float) u;
	vertexViewF32[++vertexOffset] = (float) v;
	vertexViewU32[++vertexOffset] = unit;
	vertexViewU32[++vertexOffset] = tintEffect;
	vertexViewU32[++vertexOffset] = tint;

	vertexCount++;
}

bool Pipeline::batchQuad (Entity gameObject, double x0, double y0, double x1,
		double y1, double x2, double y2, double x3, double y3, double u0,
		double v0, double u1, double v1, int tintTL, int tintTR, int tintBL,
		int tintBR, int tintEffect, GL_texture texture, GLenum unit)
{
	if (unit == GL_NONE)
		unit = currentUnit;

	bool hasFlushed = false;

	if (shouldFlush(6)) {
		flush();
		hasFlushed = true;
		unit = setTexture2D(texture);
	}

	batchVert(x0, y0, u0, v0, unit, tintEffect, tintTL);
	batchVert(x1, y1, u0, v1, unit, tintEffect, tintBL);
	batchVert(x2, y2, u1, v1, unit, tintEffect, tintBR);
	batchVert(x0, y0, u0, v0, unit, tintEffect, tintTL);
	batchVert(x2, y2, u1, v1, unit, tintEffect, tintBR);
	batchVert(x3, y3, u1, v0, unit, tintEffect, tintTR);

	onBatch(gameObject);

	return hasFlushed;
}

bool Pipeline::batchTri (Entity gameObject, double x0, double y0, double x1,
		double y1, double x2, double y2, double u0, double v0, double u1,
		double v1, int tintTL, int tintTR, int tintBL, int tintEffect,
		GL_texture texture, GLenum unit)
{
	if (unit == GL_NONE)
		unit = currentUnit;

	bool hasFlushed = false;

	if (shouldFlush(3)) {
		flush();
		hasFlushed = true;
		unit = setTexture2D(texture);
	}

	batchVert(x0, y0, u0, v0, unit, tintEffect, tintTL);
	batchVert(x1, y1, u0, v1, unit, tintEffect, tintTR);
	batchVert(x2, y2, u1, v1, unit, tintEffect, tintBL);

	onBatch(gameObject);

	return hasFlushed;
}

void Pipeline::drawFillRect (double x, double y, double width, double height,
		int color, double alpha, GL_texture texture, bool flipUV)
{
	x = std::floor(x);
	y = std::floor(y);

	double xw = std::floor(x + width);
	double yh = std::floor(y + height);

	GLenum unit = setTexture2D(texture);

	int tint = GetTintAppendFloatAlphaAndSwap(color, alpha);

	double u0 = 0;
	double v0 = 0;
	double u1 = 0;
	double v1 = 0;

	if (flipUV) {
		v0 = 1;
		v1 = 0;
	}

	batchQuad(entt::null, x, y, x, yh, xw, yh, xw, y, u0, v0, u1, v1, tint, tint,
			tint, tint, 0, texture, unit);
}

GLenum Pipeline::setTexture2D (GL_texture texture)
{
	if (!texture)
		texture = g_renderer.whiteTexture;

	currentUnit = g_renderer.setTexture2D(texture);

	return currentUnit;
}

void Pipeline::bindTexture (GL_texture texture, GLenum unit)
{
	glActiveTexture(unit);

	glBindTexture(GL_TEXTURE_2D, texture);
}

void Pipeline::bindRenderTarget (RenderTarget* target, GLenum unit)
{
	bindTexture(target->texture, unit);
}

void Pipeline::setTime (std::string uniform)
{
	set(uniform, SDL_GetTicks());
}

}	// Pipeline::namespace Zen
