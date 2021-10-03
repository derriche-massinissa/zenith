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

RenderTarget::RenderTarget (double width, double height, double scale,
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

void RenderTarget::resize (double width_, double height_)
{
	double scaledWidth_ = width_ * scale;
	double scaledHeight_ = height_ * scale;

	if (scaledWidth_ != width || scaledHeight_ != height) {
		g_renderer.deleteFramebuffer(framebuffer);
		g_renderer.deleteTexture(texture);

		width_ *= scale;
		height_ *= scale;

		width_ = std::round(width_);
		height_ = std::round(height_);

		if (width_ <= 0)
			width_ = 1;

		if (height_ <= 0)
			height_ = 1;

		texture = g_renderer.createTextureFromSource(nullptr, width_, height_,
				minFilter);
		framebuffer = g_renderer.createFramebuffer(width_, height_, texture, false);

		width = width_;
		height = height_;
	}
}

void RenderTarget::bind (bool adjustViewport_, double width_, double height_)
{
	if (adjustViewport_)
		g_renderer.flush();

	if (width_ && height_)
		resize(width_, height_);

	g_renderer.pushFramebuffer(framebuffer, false, false, false);

	if (adjustViewport_)
		adjustViewport();

	if (autoClear) {
		glClearColor(0, 0, 0, 0);
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

	glClearColor(0, 0, 0, 0);

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
	g_renderer.deleterFramebuffer(framebuffer);
	g_renderer.deleteTexture(texture);

	if (resizeListener)
		g_renderer.off(resizeListener);
}

}	// namespace Zen
