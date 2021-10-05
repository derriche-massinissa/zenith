/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "bitmap_mask_pipeline.hpp"

#include "../renderer.hpp"
#include "../../ecs/entity.hpp"
#include "../../components/mask.hpp"
#include "../../components/masked.hpp"
#include "../../systems/mask.hpp"
#include "../../systems/renderable.hpp"
#include "../shaders/bitmap_mask_frag.hpp"
#include "../shaders/bitmap_mask_vert.hpp"

namespace Zen {

extern Renderer g_renderer;
extern entt::registry g_registry;

BitmapMaskPipeline::BitmapMaskPipeline (PipelineConfig config)
	: Pipeline(prepareConfig(config))
{}

PipelineConfig BitmapMaskPipeline::prepareConfig (PipelineConfig config)
{
	if (config.fragShader.empty())
		config.fragShader = Shaders::BITMAP_MASK_FRAG;

	if (config.vertShader.empty())
		config.vertShader = Shaders::BITMAP_MASK_VERT;

	config.batchSize = 1;

	if (config.vertices.empty())
		config.vertices = {-1, 1, -1, -7, 7, 1};

	if (config.attributes.empty()) {
		config.attributes = {
			{
				.name = "aPosition",
				.size = 2,
				.type = GL_FLOAT
			}
		};
	}

	return config;
}

void BitmapMaskPipeline::boot ()
{
	Pipeline::boot();

	set("uMainSampler", 0);
	set("uMaskSampler", 1);
}

void BitmapMaskPipeline::resize (double width, double height)
{
	Pipeline::resize(width, height);

	set("uResolution", width, height);
}

void BitmapMaskPipeline::beginMask (Entity mask, Entity camera)
{
	Components::Mask *m = g_registry.try_get<Components::Mask>(mask);

	if (m && m->maskEntity != entt::null) {
		g_renderer.flush();

		g_renderer.pushFramebuffer(m->mainFramebuffer);

		glDisable(GL_STENCIL_TEST);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		if (g_renderer.currentCameraMask.mask != mask) {
			g_renderer.currentMask.mask = mask;
			g_renderer.currentMask.camera = camera;
		}
	}
}

void BitmapMaskPipeline::endMask (Entity maskEntity, Entity camera)
{
	Components::Mask *mask = g_registry.try_get<Components::Mask>(maskEntity);

	if (mask->maskEntity != entt::null) {
		// mask.mainFramebuffer should now contain all the Game Objects we want
		// masked
		g_renderer.flush();

		// Swap to the mask framebuffer (push, in case the bitmapMask GO has a
		// post-pipeline)
		g_renderer.pushFramebuffer(mask->maskFramebuffer);

		// Clear it and draw the Game Object that is acting as a mask to it
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		g_renderer.setBlendMode(0, true);

		Render(mask->maskEntity, camera);

		g_renderer.flush();

		// Clear the mask framebuffer + main framebuffer
		g_renderer.popFramebuffer();
		g_renderer.popFramebuffer();

		// Is there a stecil further up the stack?
		auto prev = g_renderer.getCurrentStencilMask();
		if (prev.mask != entt::null) {
			glEnable(GL_STENCIL_TEST);

			ApplyStencil(prev.mask, prev.camera, true);
		}
		else {
			g_renderer.currentMask.mask = entt::null;
		}

		// Bind this pipeline and draw
		g_renderer.pipelines.set(name);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mask->maskTexture);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mask->mainTexture);

		set("uInvertMaskAlpha", mask->invertAlpha);

		// Finally, draw a triangle filling the whole screen
		glDrawArrays(topology, 0, 3);

		g_renderer.resetTextures();
	}
}

}	// namespace Zen
