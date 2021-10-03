/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_PIPELINES_UTILITY_HPP
#define ZEN_RENDERER_PIPELINES_UTILITY_HPP

#include "../pipeline.hpp"
#include "../../display/color_matrix.hpp"

namespace Zen {

/** The Utility Pipeline is a special-use pipeline that belongs to the
 * Pipeline Manager.
 *
 * It provides 4 shaders and handy associated methods:
 *
 * 1) Copy Shader. A fast texture to texture copy shader with optional brightness
 * setting.
 * 2) Additive Blend Mode Shader. Blends two textures using an additive blend mode.
 * 3) Linear Blend Mode Shader. Blends two textures using a linear blend mode.
 * 4) Color Matrix Copy Shader. Draws a texture to a target using a Color Matrix.
 *
 * You do not extend this pipeline, but instead get a reference to it from the
 * Pipeline Manager via the PipelineManager::setUtility method. You can also access
 * methods such as copyFrame directly from the Pipeline Manager.
 *
 * This pipeline provides methods for manipulating framebuffer backed textures,
 * such as copying or blending one texture to another, copying a portion of a
 * texture, additively blending two textures, flipping textures and more.
 *
 * The default shader attributes for this pipeline are:
 *
 * * `inPosition` (vec2, offset 0)
 * * `inTexCoord` (vec2, offset 8)
 *
 * This pipeline has a hard-coded batch size of 1 and a hard coded set of vertices.
 *
 * @class UtilityPipeline
 * @since 0.0.0
 */
class UtilityPipeline : public Pipeline {
public:
	/**
	 * @since 0.0.0
	 */
	static PipelineConfig setupConfig (PipelineConfig config);

	UtilityPipeline (PipelineConfig config);

	void boot ();

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
    void copyFrame (RenderTarget* source, RenderTarget* target,
			double brightness = 1, bool clear = true, bool clearAlpha = true);

    /** Copy the `source` Render Target to the `target` Render Target.
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
    void blitFrame (RenderTarget* source, RenderTarget* target,
			double brightness = 1, bool clear = true, bool clearAlpha = true,
			bool eraseMode = false);

    /** Binds the `source` Render Target and then copies a section of it to the 
	 * target` Render Target.
     *
	 * This method is extremely fast because it uses `gl.copyTexSubImage2D` and
	 * doesn't require the use of any shaders. Remember the coordinates are
	 * given in standard WebGL format, where x and y specify the lower-left
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
    void copyFrameRect (RenderTarget* source, RenderTarget* target, int x, int y,
			int width, int height, bool clear = true, bool clearAlpha = true);

	/** Pops the framebuffer from the renderers FBO stack and sets that as the
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
    void copyToGame (RenderTarget* source);

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
     * @param colorMatrix The Color Matrix to use when performing the draw.
     */
    void drawFrame (RenderTarget* source, RenderTarget* target,
			bool clearAlpha = true, ColorMatrix* colorMatrix = nullptr);

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
     * @param blendShader The shader to use during the blend copy.
     */
	void blendFrames (RenderTarget* source1, RenderTarget* source2,
			RenderTarget* target, double strength = 1, bool clearAlpha = true,
			Shader* blendShader = nullptr);

    /** Draws the `source1` and `source2` Render Targets to the `target` Render
	 * Target using an additive blend effect, which is controlled by the
	 * `strength` parameter.
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

    /** Clears the given Render Target.
     *
     * @since 0.0.0
     *
     * @param target The Render Target to clear.
     * @param clearAlpha Clear the alpha channel when running glClear on the target?
     */
    void clearFrame (RenderTarget* target, bool clearAlpha = true);

    /**
	 * Set the UV values for the 6 vertices that make up the quad used by the
	 * shaders in the Utility Pipeline.
     *
	 * Be sure to call `resetUVs` once you have finished manipulating the UV
	 * coordinates.
     *
     * @since 0.0.0
     *
     * @param uA The u value of vertex A.
     * @param vA The v value of vertex A.
     * @param uB The u value of vertex B.
     * @param vB The v value of vertex B.
     * @param uC The u value of vertex C.
     * @param vC The v value of vertex C.
     * @param uD The u value of vertex D.
     * @param vD The v value of vertex D.
     */
	void setUVs (double uA, double vA, double uB, double vB, double uC,
			double vC, double uD, double vD);

    /**
	 * Sets the vertex UV coordinates of the quad used by the shaders in the
	 * Utility Pipeline so that they correctly adjust the texture coordinates
	 * for a blit frame effect.
     *
	 * Be sure to call `resetUVs` once you have finished manipulating the UV
	 * coordinates.
     *
     * @since 0.0.0
     *
     * @param source The source Render Target.
     * @param target The target Render Target.
     */
    void setTargetUVs (RenderTarget* source, RenderTarget* target);

    /**
     * Horizontally flips the UV coordinates of the quad used by the shaders in this
     * Utility Pipeline.
     *
	 * Be sure to call `resetUVs` once you have finished manipulating the UV
	 * coordinates.
     *
     * @since 0.0.0
     */
    void flipX ();

    /**
     * Vertically flips the UV coordinates of the quad used by the shaders in this
     * Utility Pipeline.
     *
	 * Be sure to call `resetUVs` once you have finished manipulating the UV
	 * coordinates.
     *
     * @since 0.0.0
     */
    void flipY ();

    /**
     * Resets the quad vertice UV values to their default settings.
     *
     * The quad is used by all shaders of the Utility Pipeline.
     *
     * @since 0.0.0
     */
    void resetUVs ();

	/**
	 * A default Color Matrix, used by the Color Matrix Shader when one
	 * isn't provided.
	 *
	 * @since 0.0.0
	 */
	ColorMatrix colorMatrix;

	/**
	 * The Copy Shader belonging to this Utility Pipeline.
	 *
	 * This property is set during the `boot` method.
	 *
	 * @since 0.0.0
	 */
	Shader* copyShader;

	/**
	 * The Additive Blend Shader belonging to this Utility Pipeline.
	 *
	 * This property is set during the `boot` method.
	 *
	 * @since 0.0.0
	 */
	Shader* addShader;

	/**
	 * The Linear Blend Shader belonging to this Utility Pipeline.
	 *
	 * This property is set during the `boot` method.
	 *
	 * @since 0.0.0
	 */
	Shader* linearShader;

	/**
	 * The Color Matrix Shader belonging to this Utility Pipeline.
	 *
	 * This property is set during the `boot` method.
	 *
	 * @since 0.0.0
	 */
	Shader* colorMatrixShader;

	/**
	 * A reference to the Full Frame 1 Render Target.
	 *
	 * This property is set during the `boot` method.
	 *
	 * This Render Target is the full size of the renderer.
	 *
	 * You can use this directly in Post FX Pipelines for multi-target effects.
	 * However, be aware that these targets are shared between all post fx
	 * pipelines.
	 *
	 * @since 0.0.0
	 */
	RenderTarget* fullFrame1;

	/**
	 * A reference to the Full Frame 2 Render Target.
	 *
	 * This property is set during the `boot` method.
	 *
	 * This Render Target is the full size of the renderer.
	 *
	 * You can use this directly in Post FX Pipelines for multi-target effects.
	 * However, be aware that these targets are shared between all post fx
	 * pipelines.
	 *
	 * @since 0.0.0
	 */
	RenderTarget* fullFrame2;

	/**
	 * A reference to the Half Frame 1 Render Target.
	 *
	 * This property is set during the `boot` method.
	 *
	 * This Render Target is half the size of the renderer.
	 *
	 * You can use this directly in Post FX Pipelines for multi-target effects.
	 * However, be aware that these targets are shared between all post fx
	 * pipelines.
	 *
	 * @since 0.0.0
	 */
	RenderTarget* halfFrame1;

	/**
	 * A reference to the Half Frame 2 Render Target.
	 *
	 * This property is set during the `boot` method.
	 *
	 * This Render Target is half the size of the renderer.
	 *
	 * You can use this directly in Post FX Pipelines for multi-target effects.
	 * However, be aware that these targets are shared between all post fx
	 * pipelines.
	 *
	 * @since 0.0.0
	 */
	RenderTarget* halfFrame2;
};

}	// namespace Zen

#endif
