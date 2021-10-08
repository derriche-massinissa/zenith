/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_RENDER_TARGET_HPP
#define ZEN_RENDERER_RENDER_TARGET_HPP

#include "types/gl_types.hpp"
#include "../event/event_listener.hpp"

namespace Zen {

/**
 * A RenderTarget encapsulates an OpenGL framebuffer and the OpenGL texture that
 * displays it.
 *
 * Instances of this class are typically created by, and belong to a pipeline,
 * however other game objects and classes can take advantage of them as well.
 *
 * @class RenderTarget
 * @since 0.0.0
 */
class RenderTarget {
public:
	/**
	 * @since 0.0.0
	 * @param width The width of this Render Target.
	 * @param height The height of this Render Target.
	 * @param scale A value between 0 and 1. Controls the size of this
	 * Render Target in relation to the Renderer.
	 * @param minFilter The minFilter mode of the texture when created.
	 * 0 is `LINEAR`, 1 is `NEAREST`.
	 * @param autoClear Automatically clear this framebuffer when bound?
	 * @param autoResize Automatically clear this Render Target if the Renderer
	 * resizes?
	 */
	RenderTarget (int width, int height, double scale = 1, int minFilter = 0,
			bool autoClear = true, bool autoResize = false);

    /**
     * Sets if this Render Target should automatically resize when the Renderer
	 * emits a resize event.
     *
     * @since 0.0.0
     *
     * @param autoResize Automatically resize this Render Target when the
	 * Renderer resizes?
     */
	void setAutoResize (bool autoResize);

    /**
     * Resizes this Render Target.
     *
     * Deletes both the frame buffer and texture, if they exist and then re-creates
     * them using the new sizes.
     *
     * This method is called automatically by the pipeline during its resize
	 * handler.
     *
     * @since 0.0.0
     *
     * @param width The new width of this Render Target.
     * @param height The new height of this Render Target.
     */
	void resize (int width, int height);

    /**
     * Pushes this Render Target as the current frame buffer of the renderer.
     *
     * If `autoClear` is set, then clears the texture.
     *
     * If `adjustViewport` is `true` then it will flush the renderer and then
	 * adjust the GL viewport.
     *
     * @since 0.0.0
     *
     * @param adjustViewport Adjust the GL viewport by calling
	 * `RenderTarget.adjustViewport` ?
     * @param width Optional new width of this Render Target.
     * @param height Optional new height of this Render Target.
     */
	void bind (bool adjustViewport = false, int width = 0, int height = 0);

    /**
     * Adjusts the GL viewport to match the width and height of this Render Target.
     *
     * Also disables `SCISSOR_TEST`.
     *
     * @since 0.0.0
     */
	void adjustViewport ();

    /**
     * Clears this Render Target.
     *
     * @since 0.0.0
     */
	void clear ();

    /**
     * Unbinds this Render Target and optionally flushes the Renderer first.
     *
     * @since 0.0.0
     *
     * @param flush Flush the WebGL Renderer before unbinding?
     *
     * @return The Framebuffer that was set, or `null` if there aren't any more in
	 * the stack.
     */
	GL_fbo unbind (bool flush = false);

    /**
     * Removes all external references from this class and deletes the
     * framebuffer and texture instances.
     *
     * Does not remove this Render Target from the parent pipeline.
     *
     * @since 0.0.0
     */
	void destroy ();

	/**
	 * The OpenGL framebuffer of this Render Target.
	 *
	 * @since 0.0.0
	 */
	GL_fbo framebuffer = 0;

	/**
	 * The OpenGL texture of this Render Target.
	 *
	 * @since 0.0.0
	 */
	GL_texture texture = 0;

	/**
	 * The render buffer of this Render Target.
	 * Typically contains the depth and stencil data.
	 *
	 * @since 0.0.0
	 */
	GLuint renderBuffer = 0;

	/**
	 * The width of the texture.
	 *
	 * @since 0.0.0
	 */
	int width = 0;

	/**
	 * The height of the texture.
	 *
	 * @since 0.0.0
	 */
	int height = 0;

	/**
	 * A value between 0 and 1. Controls the size of this Render Target in
	 * relation to the Renderer.
	 *
	 * A value of 1 matches it. 0.5 makes the Render Target half the size of the
	 * renderer, etc.
	 *
	 * @since 0.0.0
	 */
	double scale = 1;

	/**
	 * The minFilter mode of the texture. 0 is `LINEAR`, 1 is `NEAREST`.
	 *
	 * @since 0.0.0
	 */
	int minFilter = 0;

	/**
	 * Controls if this Render Target is automatically cleared (via
	 * `GL_COLOR_BUFFER_BIT`) during the `RenderTarget::bind` method.
	 *
	 * If you need more control over how, or if, the target is cleared, you can
	 * disable this via the config on creation, or even toggle it directly at
	 * runtime.
	 *
	 * @since 0.0.0
	 */
	bool autoClear = true;

	/**
	 * Does this Render Target automatically resize when the Renderer does?
	 *
	 * Modify this property via the `setAutoResize` method.
	 *
	 * @since 0.0.0
	 */
	bool autoResize = false;

	/**
	 * @since 0.0.0
	 */
	ListenerBase *resizeListener = nullptr;
};

}	// namespace Zen

#endif
