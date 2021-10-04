/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "render_functions.hpp"
#include "../components/renderable.hpp"
#include "../cameras/2d/systems/camera.hpp"
#include "../renderer/pipelines/multi_pipeline.hpp"

namespace Zen {

extern entt::registry g_registry;

void render_image (Entity entity, Entity camera, Components::TransformMatrix* matrix)
{
	AddToRenderList(camera, entity);

	auto &r = g_registry.get<Components::Renderable>(entity);
	static_cast<MultiPipeline*>(r.pipeline)->batchSprite(entity, camera,
			matrix);
}

}	// namespace Zen
