/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../mask.hpp"

#include "../../components/mask.hpp"
#include "../../components/masked.hpp"
#include "../../components/renderable.hpp"
#include "../../components/actor.hpp"
#include "../../components/textured.hpp"
#include "../../utils/assert.hpp"
#include "../../scene/scene.hpp"
#include "../../renderer/renderer.hpp"
#include "../../renderer/events/events.hpp"
#include "../renderable.hpp"

namespace Zen {

extern entt::registry g_registry;
extern Renderer g_renderer;

Entity GetMask (Entity entity)
{
	auto masked = g_registry.try_get<Components::Masked>(entity);
	ZEN_ASSERT(masked, "The entity has no 'Mask' component.");

	return masked->mask;
}

void SetMask (Entity entity, Entity maskEntity)
{
	// Is the entity renderable?
	auto [renderable, actor] = g_registry.try_get<Components::Renderable,
		 Components::Actor>(entity);
	ZEN_ASSERT(renderable, "The entity has no 'Renderable' component.");

	// Setup the masked entity
	Components::Masked &masked =
		g_registry.emplace_or_replace<Components::Masked>(entity);

	masked.mask = maskEntity;
	masked.camera = actor->scene->cameras.main;

	// Is the mask entity already setup?
	if (!g_registry.has<Components::Mask>(maskEntity)) {
		g_registry.emplace_or_replace<Components::Mask>(maskEntity);

		// Is the mask entity a bitmap object?
		if (g_registry.try_get<Components::Textured>(maskEntity)) {
			MakeMaskBitmap(maskEntity);
		}
	}
}

void MakeMaskBitmap (Entity entity)
{
	auto *mask = g_registry.try_get<Components::Mask>(entity);

	if (mask->mainTexture) {
		DeleteMask(entity);
		g_registry.emplace_or_replace<Components::Mask>(entity);
	}

	mask->bitmap = true;

	int width = g_renderer.width;
	int height = g_renderer.height;
	bool pot = ((width & (width - 1)) == 0 && (height & (height -1)) == 0);
	GLenum wrap = pot ? GL_REPEAT : GL_CLAMP_TO_EDGE;
	GLenum filter = GL_LINEAR;

	mask->mainTexture = g_renderer.createTexture2D(0, filter, filter,
			wrap, wrap, GL_RGBA, nullptr, width, height);
	mask->maskTexture = g_renderer.createTexture2D(0, filter, filter,
			wrap, wrap, GL_RGBA, nullptr, width, height);
	mask->mainFramebuffer = g_renderer.createFramebuffer(width, height,
			mask->mainTexture, true);
	mask->maskFramebuffer = g_renderer.createFramebuffer(width, height,
			mask->maskTexture, true);

	if (!mask->resizeListener) {
		mask->resizeListener = g_renderer.once(Events::RENDER_RESIZE, [entity] () {
				MakeMaskBitmap(entity);
		});
	}
}

void ClearMask (Entity entity)
{
	g_registry.remove_if_exists<Components::Masked>(entity);
}

void DeleteMask (Entity entity)
{
	auto mask = g_registry.try_get<Components::Mask>(entity);
	ZEN_ASSERT(mask, "The entity has no 'Mask' component.");

	if (!mask->mainTexture)
		return;

	g_renderer.deleteTexture(mask->mainTexture);
	g_renderer.deleteTexture(mask->maskTexture);
	g_renderer.deleteFramebuffer(mask->mainFramebuffer);
	g_renderer.deleteFramebuffer(mask->maskFramebuffer);

	g_renderer.off(mask->resizeListener);

	g_registry.remove<Components::Mask>(entity);
}

void PreRenderMask (Entity masked, Entity camera)
{
	auto md = g_registry.try_get<Components::Masked>(masked);
	auto m = g_registry.try_get<Components::Mask>(md->mask);

	if (!m)
		return;

	if (m->isBitmap) {
		g_renderer.pipelines.BITMAPMASK_PIPELINE->beginMask(md->mask, camera);
	}
	else {
		// Force flushing before drawing to stencil buffer
		g_renderer.flush();

		if (g_renderer.maskStack.empty()) {
			glEnable(GL_STENCIL_TEST);
			glClear(GL_STENCIL_BUFFER_BIT);

			g_renderer.maskCount = 0;
		}

		if (g_renderer.currentCameraMask.mask != md->mask)
			g_renderer.currentMask.mask = md->mask;

		g_renderer.maskStack.push_back({.mask = md->mask, .camera = camera});

		ApplyStencil(md->mask, camera, true);

		g_renderer.maskCount++;
	}
}

void ApplyStencil (Entity maskEntity, Entity camera, bool inc)
{
	auto mask = g_registry.try_get<Components::Mask>(maskEntity);
	ZEN_ASSERT(mask, "The entity has no 'Mask' component");

	int level = g_renderer.maskCount;

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	if (inc) {
		glStencilFunc(GL_EQUAL, level, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	}
	else  {
		glStencilFunc(GL_EQUAL, level + 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
	}

	// Write stencil buffer
	Render(mask->maskEntity, camera);

	g_renderer.flush();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	if (inc) {
		if (mask->invertAlpha)
			glStencilFunc(GL_NOTEQUAL, level + 1, 0xFF);
		else
			glStencilFunc(GL_EQUAL, level + 1, 0xFF);
	}
	else if (mask->invertAlpha) {
		glStencilFunc(GL_NOTEQUAL, level, 0xFF);
	}
	else {
		glStencilFunc(GL_EQUAL, level, 0xFF);
	}
}

void PostRenderMask (Entity mask, Entity camera)
{
	auto m = g_registry.try_get<Components::Mask>(mask);
	if (!m) return;

	if (m->isBitmap) {
		g_renderer.pipelines.BITMAPMASK_PIPELINE->endMask(mask, camera);
	}
	else {
		g_renderer.maskStack.pop_back();
		g_renderer.maskCount--;

		// Force flush before disabling stencil test
		g_renderer.flush();

		if (g_renderer.maskStack.empty()) {
			// If this is the only mask in the stack, flush and disable
			g_renderer.currentMask = {entt::null, entt::null};

			glDisable(GL_STENCIL_TEST);
		}
		else {
			Mask_ &prev = g_renderer.maskStack.back();

			ApplyStencil(prev.mask, prev.camera, false);

			if (g_renderer.currentCameraMask.mask != prev.mask) {
				g_renderer.currentMask.mask = prev.mask;
				g_renderer.currentMask.camera = prev.camera;
			}
			else {
				g_renderer.currentMask.mask = entt::null;
			}
		}
	}
}

}	// namespace Zen
