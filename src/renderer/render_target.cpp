/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "render_target.hpp"
#include "renderer.hpp"
#include <cmath>

namespace Zen {

extern Renderer g_renderer;

RenderTarget::RenderTarget (int width, int height, double scale,
		int minFilter, bool autoClear, bool autoResize)
	: scale (scale)
	, minFilter (minFilter)
	, autoClear (autoClear)
	, autoResize (autoResize)
{
	resize(width, height);

	if (autoResize)
		setAutoResize(true);
}

RenderTarget::~RenderTarget ()
{
	if (resizeListener)
		g_renderer.off(resizeListener);
}

void RenderTarget::setAutoResize (bool autoResize_)
{
	if (autoResize_ && !autoResize) {
		resizeListener = g_renderer.on("resize", &RenderTarget::resize, this);
	}
	else if (resizeListener) {
		g_renderer.off(resizeListener);
		resizeListener = nullptr;
	}

	autoResize = autoResize_;
}

void RenderTarget::resize (int width_, int height_)
{
	int scaledWidth_ = width_ * scale;
	int scaledHeight_ = height_ * scale;

	if (scaledWidth_ != width || scaledHeight_ != height) {
		g_renderer.deleteFramebuffer(framebuffer);
		g_renderer.deleteTexture(texture);

		width_ *= scale;
		height_ *= scale;

		if (width_ <= 0)
			width_ = 1;

		if (height_ <= 0)
			height_ = 1;

		texture = g_renderer.createTextureFromSource(entt::null, width_, height_,
				minFilter);
		framebuffer = g_renderer.createFramebuffer(width_, height_, texture, false);

		width = width_;
		height = height_;
	}
}

void RenderTarget::bind (bool adjustViewport_, int width_, int height_)
{
	if (adjustViewport_)
		g_renderer.flush();

	if (width_ && height_)
		resize(width_, height_);

	g_renderer.pushFramebuffer(framebuffer, false, false, false);

	if (adjustViewport_)
		adjustViewport();

	if (autoClear) {
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void RenderTarget::adjustViewport ()
{
	glViewport(0, 0, width, height);

	glDisable(GL_SCISSOR_TEST);
}

void RenderTarget::clear ()
{
	g_renderer.pushFramebuffer(framebuffer);

	glDisable(GL_SCISSOR_TEST);

	glClearColor(0.f, 0.f, 0.f, 0.f);

	glClear(GL_COLOR_BUFFER_BIT);

	g_renderer.popFramebuffer();

	g_renderer.resetScissor();
}

GL_fbo RenderTarget::unbind (bool flush)
{
	if (flush)
		g_renderer.flush();

	return g_renderer.popFramebuffer();
}

void RenderTarget::destroy ()
{
	g_renderer.deleteFramebuffer(framebuffer);
	g_renderer.deleteTexture(texture);

	if (resizeListener)
		g_renderer.off(resizeListener);
}

}	// namespace Zen
