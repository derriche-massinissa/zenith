/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_PIPELINES_POSTFX_HPP
#define ZEN_RENDERER_PIPELINES_POSTFX_HPP

#include "../pipeline.hpp"
#include "../../display/color_matrix.hpp"

namespace Zen {

/**
 * The Post FX Pipeline is a special kind of pipeline specifically for handling
 * post processing effects. Where-as a standard Pipeline allows you to control
 * the process of rendering Game Objects by configuring the shaders and
 * attributes used to draw them, a Post FX Pipeline is designed to allow you to
 * apply processing _after_ the Game Object/s have been rendered.  Typical
 * examples of post processing effects are bloom filters, blurs, light effects
 * and color manipulation.
 *
 * The pipeline works by creating a tiny vertex buffer with just one single
 * hard-coded quad in it. Game Objects can have a Post Pipeline set on them.
 * Those objects are then rendered using their standard pipeline, but are
 * redirected to the Render Targets owned by the post pipeline, which can then
 * apply their own shaders and effects, before passing them back to the main
 * renderer.
 *
 * The default fragment shader it uses can be found in `shaders/post_fx.frag`.
 * The default vertex shader it uses can be found in `shaders/quad.vert`.
 *
 * The default shader attributes for this pipeline are:
 *
 * `aPosition` (vec2, offset 0)
 * `aTexCoord` (vec2, offset 8)
 *
 * The vertices array layout is:
 *
 * -1,  1   B----C   1,  1
 *  0,  1   |   /|   1,  1
 *          |  / |
 *          | /  |
 *          |/   |
 * -1, -1   A----D   1, -1
 *  0,  0            1,  0
 *
 * A = -1, -1 (pos) and 0, 0 (uv)
 * B = -1,  1 (pos) and 0, 1 (uv)
 * C =  1,  1 (pos) and 1, 1 (uv)
 * D =  1, -1 (pos) and 1, 0 (uv)
 *
 * First tri: A, B, C
 * Second tri: A, C, D
 *
 * Array index:
 *
 * 0  = Tri 1 - Vert A - x pos
 * 1  = Tri 1 - Vert A - y pos
 * 2  = Tri 1 - Vert A - uv u
 * 3  = Tri 1 - Vert A - uv v
 *
 * 4  = Tri 1 - Vert B - x pos
 * 5  = Tri 1 - Vert B - y pos
 * 6  = Tri 1 - Vert B - uv u
 * 7  = Tri 1 - Vert B - uv v
 *
 * 8  = Tri 1 - Vert C - x pos
 * 9  = Tri 1 - Vert C - y pos
 * 10 = Tri 1 - Vert C - uv u
 * 11 = Tri 1 - Vert C - uv v
 *
 * 12 = Tri 2 - Vert A - x pos
 * 13 = Tri 2 - Vert A - y pos
 * 14 = Tri 2 - Vert A - uv u
 * 15 = Tri 2 - Vert A - uv v
 *
 * 16 = Tri 2 - Vert C - x pos
 * 17 = Tri 2 - Vert C - y pos
 * 18 = Tri 2 - Vert C - uv u
 * 19 = Tri 2 - Vert C - uv v
 *
 * 20 = Tri 2 - Vert D - x pos
 * 21 = Tri 2 - Vert D - y pos
 * 22 = Tri 2 - Vert D - uv u
 * 23 = Tri 2 - Vert D - uv v
 *
 * @class PostFXPipeline
 * @since 0.0.0
 */
class PostFXPipeline : public Pipeline
{
public:
	/**
	 * @since 0.0.0
	 *
	 * @param config The configuration options for this pipeline.
	 */
	PostFXPipeline (PipelineConfig config);

	PipelineConfig prepareConfig (PipelineConfig config);

	void boot ();

	void onDraw (RenderTarget *renderTarget);

    /**
     * Copy the `source` Render Target to the `target` Render Target.
     *
     * You can optionally set the brightness factor of the copy.
     *
     * The difference between this method and `drawFrame` is that this method
     * uses a faster copy shader, where only the brightness can be modified.
     * If you need color level manipulation, see `drawFrame` instead.
     *
     * @since 0.0.0
     *
     * @param source The source Render Target.
     * @param target The target Render Target.
     * @param brightness The brightness value applied to the frame copy.
     * @param clear Clear the target before copying?
     * @param clearAlpha Clear the alpha channel when running glClear on the target?
     */
    void copyFrame (RenderTarget *source, RenderTarget *target,
			double brightness = 1., bool clear = true, bool clearAlpha = true);

    /**
	 * Pops the framebuffer from the renderers FBO stack and sets that as the
	 * active target, then draws the `source` Render Target to it. It then
	 * resets the renderer textures.
     *
	 * This should be done when you need to draw the _final_ results of a
	 * pipeline to the game canvas, or the next framebuffer in line on the FBO
	 * stack. You should only call this once in the `onDraw` handler and it
	 * should be the final thing called. Be careful not to call this if you
	 * need to actually use the pipeline shader, instead of the copy shader. In
	 * those cases, use the `bindAndDraw` method.
     *
     * @since 0.0.0
     *
     * @param source The Render Target to draw from.
     */
    void copyToGame (RenderTarget *source);

    /**
     * Copy the `source` Render Target to the `target` Render Target, using the
     * given Color Matrix.
     *
     * The difference between this method and `copyFrame` is that this method
     * uses a color matrix shader, where you have full control over the luminance
     * values used during the copy. If you don't need this, you can use the faster
     * `copyFrame` method instead.
     *
     * @since 0.0.0
     *
     * @param source The source Render Target.
     * @param target The target Render Target.
     * @param clearAlpha Clear the alpha channel when running glClear on the target?
     */
    void drawFrame (RenderTarget *source, RenderTarget *target,
			bool clearAlpha = true);

    /**
     * Draws the `source1` and `source2` Render Targets to the `target` Render Target
     * using a linear blend effect, which is controlled by the `strength` parameter.
     *
     * @since 0.0.0
     *
     * @param source1 The first source Render Target.
     * @param source2 The second source Render Target.
     * @param target The target Render Target.
     * @param strength The strength of the blend.
     * @param clearAlpha Clear the alpha channel when running glClear on the target?
     */
    void blendFrames (RenderTarget* source1, RenderTarget* source2,
			RenderTarget* target, double strength = 1, bool clearAlpha = true);

    /**
     * Draws the `source1` and `source2` Render Targets to the `target` Render Target
     * using an additive blend effect, which is controlled by the `strength` parameter.
     *
     * @since 0.0.0
     *
     * @param source1 The first source Render Target.
     * @param source2 The second source Render Target.
     * @param target The target Render Target.
     * @param strength The strength of the blend.
     * @param clearAlpha Clear the alpha channel when running glClear on the target?
     */
    void blendFramesAdditive (RenderTarget* source1, RenderTarget* source2,
			RenderTarget* target, double strength = 1, bool clearAlpha = true);

    /**
     * Clears the given Render Target.
     *
     * @since 0.0.0
     *
     * @param target The Render Target to clear.
     * @param clearAlpha Clear the alpha channel when running glClear on the target?
     */
    void clearFrame (RenderTarget *target, bool clearAlpha = true);

    /**
     * Copy the `source` Render Target to the `target` Render Target.
     *
     * The difference with this copy is that no resizing takes place. If the `source`
     * Render Target is larger than the `target` then only a portion the same size as
     * the `target` dimensions is copied across.
     *
     * You can optionally set the brightness factor of the copy.
     *
     * @since 0.0.0
     *
     * @param source The source Render Target.
     * @param target The target Render Target.
     * @param brightness The brightness value applied to the frame copy.
     * @param clear Clear the target before copying?
     * @param clearAlpha Clear the alpha channel when running glClear on the target?
     * @param eraseMode Erase source from target using ERASE Blend Mode?
     */
    void blitFrame (RenderTarget *source, RenderTarget *target,
			double brightness = 1, bool clear = true, bool clearAlpha = true,
			bool eraseMode = false);

    /**
	 * Binds the `source` Render Target and then copies a section of it to the
	 * `target` Render Target.
     *
	 * This method is extremely fast because it uses `glCopyTexSubImage2D` and
	 * doesn't require the use of any shaders. Remember the coordinates are
	 * given in standard OpenGL format, where x and y specify the lower-left
	 * corner of the section, not the top-left. Also, the copy entirely
	 * replaces the contents of the target texture, no 'merging' or 'blending'
	 * takes place.
     *
     * @since 0.0.0
     *
     * @param source The source Render Target.
     * @param target The target Render Target.
     * @param x The x coordinate of the lower left corner where to start copying.
     * @param y The y coordinate of the lower left corner where to start copying.
     * @param width The width of the texture.
     * @param height The height of the texture.
     * @param clear Clear the target before copying?
     * @param clearAlpha Clear the alpha channel when running glClear on the target?
     */
    void copyFrameRect (RenderTarget *source, RenderTarget *target, int x, int y,
			int width, int height, bool clear = true, bool clearAlpha = true);

    /**
	 * Binds this pipeline and draws the `source` Render Target to the `target`
	 * Render Target.
     *
	 * If no `target` is specified, it will pop the framebuffer from the
	 * Renderers FBO stack and use that instead, which should be done when you
	 * need to draw the final results of this pipeline to the game canvas.
     *
	 * You can optionally set the shader to be used for the draw here, if this
	 * is a multi-shader pipeline. By default `currentShader` will be used. If
	 * you need to set a shader but not a target, just pass `null` as the
	 * `target` parameter.
     *
     * @since 0.0.0
     *
     * @param source The Render Target to draw from.
	 * @param target The Render Target to draw to. If not set, it will pop the
	 * fbo from the stack.
	 * @param clear Clear the target before copying? Only used if `target`
	 * parameter is set.
     * @param clearAlpha Clear the alpha channel when running glClear on the target?
     * @param currentShader The shader to use during the draw.
     */
    void bindAndDraw (RenderTarget *source, RenderTarget *target = nullptr,
			bool clear = true, bool clearAlpha = true,
			Shader *currentShader = nullptr);

	/**
	 * If this post-pipeline belongs to a Game Object or Camera, this contains
	 * a reference to it.
	 *
	 * @since 0.0.0
	 */
	Entity gameObject = entt::null;
 
	/**
	 * A Color Matrix instance belonging to this pipeline.
	 *
	 * Used during calls to the `drawFrame` method.
	 *
	 * @since 0.0.0
	 */
	ColorMatrix colorMatrix;

	/**
	 * A reference to the Full Frame 1 Render Target that belongs to the
	 * Utility Pipeline. This property is set during the `boot` method.
	 *
	 * This Render Target is the full size of the renderer.
	 *
	 * You can use this directly in Post FX Pipelines for multi-target effects.
	 * However, be aware that these targets are shared between all post fx
	 * pipelines.
	 *
	 * @since 0.0.0
	 */
	RenderTarget *fullFrame1 = nullptr;

	/**
	 * A reference to the Full Frame 2 Render Target that belongs to the
	 * Utility Pipeline. This property is set during the `boot` method.
	 *
	 * This Render Target is the full size of the renderer.
	 *
	 * You can use this directly in Post FX Pipelines for multi-target effects.
	 * However, be aware that these targets are shared between all post fx
	 * pipelines.
	 *
	 * @since 0.0.0
	 */
	RenderTarget *fullFrame2 = nullptr;

	/**
	 * A reference to the Half Frame 1 Render Target that belongs to the
	 * Utility Pipeline. This property is set during the `boot` method.
	 *
	 * This Render Target is half the size of the renderer.
	 *
	 * You can use this directly in Post FX Pipelines for multi-target effects.
	 * However, be aware that these targets are shared between all post fx
	 * pipelines.
	 *
	 * @since 0.0.0
	 */
	RenderTarget *halfFrame1 = nullptr;

	/**
	 * A reference to the Half Frame 2 Render Target that belongs to the
	 * Utility Pipeline. This property is set during the `boot` method.
	 *
	 * This Render Target is half the size of the renderer.
	 *
	 * You can use this directly in Post FX Pipelines for multi-target effects.
	 * However, be aware that these targets are shared between all post fx
	 * pipelines.
	 *
	 * @since 0.0.0
	 */
	RenderTarget *halfFrame2 = nullptr;
};

}	// namespace Zen

#endif
