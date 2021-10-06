/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_RENDERABLE_HPP
#define ZEN_SYSTEMS_RENDERABLE_HPP

#include "../ecs/entity.hpp"
#include "../components/transform_matrix.hpp"
#include "../components/renderable.hpp"
#include "../renderer/pipeline.hpp"
#include <memory>

namespace Zen {

bool WillRender (Entity entity, Entity camera);

int GetRenderFlags (Entity entity);

/**
 * Renders the entity using the pipeline.
 *
 * @param entity The entity to render.
 * @param camera The camera to use to render the entity.
 * @param transformMatrix An optional transform matrix to apply to the entity.
 */
void Render (Entity entity, Entity camera,
		Components::TransformMatrix* transformMatrix = nullptr);

/**
 * Sets the initial Pipeline of this Game Object.
 *
 * This should only be called during the instantiation of the Game Object.
 * After that, use `setPipeline`.
 *
 * @since 0.0.0
 *
 * @param entity The entity that owns the pipeline.
 * @param pipeline Either the string-based name of the pipeline, or a pipeline
 * instance to set.
 *
 * @return `true` if the pipeline was set successfully, otherwise `false`.
 */
bool InitPipeline (Entity entity, std::string name = "");

/**
 * Sets the main Pipeline of this Game Object.
 *
 * Both the pipeline and post pipelines share the same pipeline data object.
 *
 * @since 0.0.0
 *
 * @param entity The entity that owns the pipeline.
 * @param pipeline The string-based name of the pipeline.
 */
void SetPipeline (Entity entity, std::string name);

/**
 * Sets one, or more, Post Pipelines on this Game Object.
 *
 * Post Pipelines are invoked after this Game Object has rendered to its target and
 * are commonly used for post-fx.
 *
 * The post pipelines are appended to the `postPipelines` array belonging to
 * this Game Object. When the renderer processes this Game Object, it iterates
 * through the post pipelines in the order in which they appear in the array.
 * If you are stacking together multiple effects, be aware that the order is
 * important.
 *
 * If you call this method multiple times, the new pipelines will be appended
 * to any existing post pipelines already set. Use the `resetPostPipeline`
 * method to clear them first, if required.
 *
 * @since 0.0.0
 *
 * @param entity The entity that owns the pipeline.
 * @param pipelines The string-based name of the pipeline.
 */
template <typename T>
void SetPostPipeline (Entity entity, std::string name = "")
{
	extern entt::registry g_registry;

	auto *r = g_registry.try_get<Components::Renderable>(entity);
	ZEN_ASSERT(r, "The entity has no 'Renderable' component.");

	r->postPipelines.emplace_back(std::make_unique<T>());
	r->postPipelines.back()->name = name;
}

/**
 * Gets a Post Pipeline instance from this Game Object, based on the given
 * name, and returns it.
 *
 * @since 0.0.0
 *
 * @param entity The entity that owns the pipeline.
 * @param pipeline The string-based name of the pipeline.
 *
 * @return The array of all the Post Pipeline/s matching the name.
 */
std::vector<PostFXPipeline*> GetPostPipeline (Entity entity, std::string name);

/**
 * Resets the Pipeline of this Game Object back to the default it was created with.
 *
 * @since 0.0.0
 *
 * @param entity The entity that owns the pipeline.
 * @param resetPostPipelines Reset all of the post pipelines?
 *
 * @return `true` if the pipeline was reset successfully, otherwise `false`.
 */
bool ResetPipeline (Entity entity, bool resetPostPipelines = false);

/**
 * Resets the Post Pipelines of this Game Object. It does this by calling
 * the `destroy` method on each post pipeline and then clearing the local array.
 *
 * @since 0.0.0
 *
 * @param entity The entity that owns the pipeline.
 */
void ResetPostPipeline (Entity entity);

/**
 * Removes a type of Post Pipeline instances from this Game Object, based on
 * the given name, and destroys them.
 *
 * If you wish to remove all Post Pipelines use the `resetPostPipeline` method
 * instead.
 *
 * @since 0.0.0
 *
 * @param entity The entity that owns the pipeline.
 * @param pipeline The string-based name of the pipeline.
 */
void RemovePostPipeline (Entity entity, std::string name);

/**
 * Gets the name of the Pipeline this Game Object is currently using.
 *
 * @since 0.0.0
 *
 * @return The string-based name of the pipeline being used by this entity.
 */
std::string GetPipelineName (Entity entity);

}	// namespace Zen

#endif
