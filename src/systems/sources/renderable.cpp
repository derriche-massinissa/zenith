/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../renderable.hpp"

#include "../../utils/assert.hpp"
#include "../../components/id.hpp"
#include "../../renderer/renderer.hpp"
#include "../../renderer/pipelines/const.hpp"

namespace Zen {

extern entt::registry g_registry;
extern Renderer g_renderer;

bool WillRender (Entity entity, Entity camera)
{
	auto renderable = g_registry.try_get<Components::Renderable>(entity);
	if (!renderable)
		return false;

	auto id = g_registry.try_get<Components::Id>(camera);
	if (!id)
		return false;

	return !(
		0b1111 != renderable->flags ||
		(renderable->filter != 0 &&
		(renderable->filter & id->value))
		);
}

int GetRenderFlags (Entity entity)
{
	auto renderable = g_registry.try_get<Components::Renderable>(entity);
	ZEN_ASSERT(renderable, "The entity has no 'Renderable' component.");

	return renderable->flags;
}

void Render (Entity entity, Entity camera,
		Components::TransformMatrix* transformMatrix)
{
	auto renderable = g_registry.try_get<Components::Renderable>(entity);
	ZEN_ASSERT(renderable, "The entity has no 'Renderable' component.");

	renderable->render(entity, camera, transformMatrix);
}

bool InitPipeline (Entity entity, std::string name)
{
	auto renderable = g_registry.try_get<Components::Renderable>(entity);
	ZEN_ASSERT(renderable, "The entity has no 'Renderable' component.");

	if (name.empty())
		name = Pipelines::MULTI_PIPELINE;

	Pipeline* pipeline = g_renderer.pipelines.get(name);

	if (pipeline) {
		renderable->defaultPipeline = pipeline;
		renderable->pipeline = pipeline;

		return true;
	}

	return false;
}

void SetPipeline (Entity entity, std::string name)
{
	auto renderable = g_registry.try_get<Components::Renderable>(entity);
	ZEN_ASSERT(renderable, "The entity has no 'Renderable' component");

	Pipeline* pipeline = g_renderer.pipelines.pipelines[name].get();

	renderable->pipeline = pipeline;
}

std::vector<PostFXPipeline*> GetPostPipeline (Entity entity, std::string name)
{
	auto renderable = g_registry.try_get<Components::Renderable>(entity);
	ZEN_ASSERT(renderable, "The entity has no 'Renderable' component");

	std::vector<PostFXPipeline*> results;

	for (auto &pipeline : renderable->postPipelines) {
		if (pipeline->name == name)
			results.emplace_back(pipeline.get());
	}

	return results;
}

bool ResetPipeline (Entity entity, bool resetPostPipelines)
{
	auto renderable = g_registry.try_get<Components::Renderable>(entity);
	ZEN_ASSERT(renderable, "The entity has no 'Renderable' component");

	renderable->pipeline = renderable->defaultPipeline;

	if (resetPostPipelines)
		renderable->postPipelines.clear();

	return (renderable->pipeline != nullptr);
}

void ResetPostPipeline (Entity entity)
{
	auto renderable = g_registry.try_get<Components::Renderable>(entity);
	ZEN_ASSERT(renderable, "The entity has no 'Renderable' component");

	renderable->postPipelines.clear();
}

void RemovePostPipeline (Entity entity, std::string name)
{
	auto renderable = g_registry.try_get<Components::Renderable>(entity);
	ZEN_ASSERT(renderable, "The entity has no 'Renderable' component");

	for (size_t i = 0; i < renderable->postPipelines.size();) {
		if (renderable->postPipelines[i]->name == name)
			renderable->postPipelines.erase(renderable->postPipelines.begin() + i);
		else
			i++;
	}
}

std::string GetPipelineName (Entity entity)
{
	auto renderable = g_registry.try_get<Components::Renderable>(entity);
	ZEN_ASSERT(renderable, "The entity has no 'Renderable' component");

	return renderable->pipeline->name;
}

}	// namespace Zen
