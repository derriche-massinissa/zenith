/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_PIPELINE_MANAGER_HPP
#define ZEN_RENDERER_PIPELINE_MANAGER_HPP

#include <map>
#include <string>
#include <memory>
#include "pipeline.hpp"
#include "render_target.hpp"
#include "pipelines/utility_pipeline.hpp"
#include "pipelines/multi_pipeline.hpp"
#include "pipelines/bitmap_mask_pipeline.hpp"
#include "pipelines/postfx_pipeline.hpp"

namespace Zen {

/**
 * The Pipeline Manager is responsible for the creation, activation, running and
 * destruction of Pipelines and Post FX Pipelines.
 *
 * The `Renderer` owns a single instance of the Pipeline Manager, which you can
 * access via the `Renderer::pipelines` property.
 *
 * By default, there are 8 pipelines installed into the Pipeline Manager:
 * 1. The Multi Pipeline. Responsible for all multi-texture rendering, i.e. Sprites
 * and Tilemaps.
 * 2. The Graphics Pipeline. Responsible for rendering Graphics and Shape objects.
 * 3. The Single Pipeline. Responsible for rendering Game Objects that explicitly
 * require one bound texture.
 * 4. The Bitmap Mask Pipeline. Responsible for Bitmap Mask rendering.
 * 5. The Utility Pipeline. Responsible for providing lots of handy texture
 * manipulation functions.
 *
 * You can add your own custom pipeline via the `PipelineManager::add` method.
 * Pipelines are identified by unique string-based keys.
 *
 * @since 0.0.0
 */
class PipelineManager
{
public:
	/**
	 * @since 0.0.0
	 */
	PipelineManager ();

	~PipelineManager ();

	/**
	 * @since 0.0.0
	 */
	void boot ();

    /**
     * Adds a pipeline to this Pipeline Manager.
     *
     * The name of the instance must be unique within this manager.
     *
     * You should call it like this:
     *
     * ```cpp
     * add<CustomPipeline>("yourName");`
     * ```
     *
     * To add a **Post Pipeline**, see `addPostPipeline` instead.
     *
     * @since 0.0.0
	 *
     * @tparam T A pipeline class which must extend `Pipeline`.
     * @param name A unique string-based key for the pipeline within the manager.
     *
     * @return Pointer to the created pipeline
     */
	template <typename T>
    Pipeline* add (std::string name)
	{
		if (has(name)) {
			MessageWarning("Pipeline: ", name, " already exists");
			return nullptr;
		}

		Pipeline &pipeline = *Emplace(pipelines, name, std::make_unique<T>());

		if (pipeline.isPostFX) {
			MessageWarning(name, " is a Post Pipeline. Use `addPostPipeline`"
					" instead");
			pipelines.erase(pipelines.find(name));
			return nullptr;
		}

		pipeline.name = name;

		pipeline.boot();

		return &pipeline;
	}

    /**
     * Flushes the current pipeline, if one is bound.
     *
     * @since 0.0.0
     */
    void flush ();

    /**
     * Checks if a pipeline is present in this Pipeline Manager.
     *
     * @since 0.0.0
     *
     * @param name The string-based name of the pipeline to check for
     *
     * @return `true` if the given pipeline is loaded, otherwise `false`.
     */
    bool has (std::string name);

    /**
     * Returns the pipeline instance based on the given name.
     *
     * @since 0.0.0
     *
     * @param pipeline The string-based name of the pipeline to get
     *
     * @return The pipeline instance, or `nullptr` if not found.
     */
    Pipeline* get (std::string pipeline);

    /**
     * Removes a pipeline instance based on the given name.
     *
     * If no pipeline matches the name, this method does nothing.
     *
     * Note that the pipeline will not be flushed, it's simply removed
	 * from this manager.
     *
     * @since 0.0.0
     *
     * @param name The name of the pipeline to be removed.
     */
	void remove (std::string name);

    /**
     * Sets the current pipeline to be used by the Renderer.
     *
     * If the pipeline isn't already the current one it will call Pipeline::bind
	 * and then Pipeline::onBind.
     *
     * You cannot set Post FX Pipelines using this method. To use a Post FX
	 * Pipeline, you should apply it to either a Camera, Container or other
	 * supporting Game Object.
     *
     * @since 0.0.0
     *
     * @param pipeline The name of the pipeline to be set as current.
     * @param gameObject The Game Object that invoked this pipeline, if any.
     * @param currentShader The shader to set as being current.
	 *
     * @return The pipeline that was set, or nullptr if it couldn't be set.
     */
    Pipeline* set (std::string pipeline, Entity entity = entt::null,
			Shader* currentShader = nullptr);

    /**
     * This method is called by the Pipeline::batchQuad method, right before a quad
	 * belonging to a Game Object is about to be added to the batch. It causes a
	 * batch flush, then calls the preBatch method on the post-fx pipelines
	 * belonging to the Game Object.
     *
     * @since 0.0.0
     *
     * @param gameObject The Game Object about to be batched.
     */
    void preBatch (Entity gameObject);

    /**
     * This method is called by the Pipeline::batchQuad method, right after a quad
     * belonging to a Game Object has been added to the batch. It causes a batch
     * flush, then calls the postBatch method on the post-fx pipelines belonging
	 * to the Game Object.
     *
     * @since 0.0.0
     *
     * @param gameObject The Game Object that was just added to the batch.
     */
    void postBatch (Entity gameObject);

    /**
     * Called at the start of the Renderer::preRenderCamera method.
     *
     * If the Camera has post pipelines set, it will flush the batch and then call
	 * the `preBatch` method on the post-fx pipelines belonging to the Camera.
     *
     * @since 0.0.0
     *
     * @param camera The Camera about to be rendered.
     */
    void preBatchCamera (Entity camera);

    /**
     * Called at the end of the Renderer::postRenderCamera method.
     *
     * If the Camera has post pipelines set, it will flush the batch and then call
	 * the `postBatch` method on the post-fx pipelines belonging to the Camera.
     *
     * @since 0.0.0
     *
     * @param camera The Camera that was just rendered.
     */
    void postBatchCamera (Entity camera);

    /**
     * Checks to see if the given pipeline is already the active pipeline, both
	 * within this PipelineManager and also has the same shader set in the
	 * Renderer.
     *
     * @since 0.0.0
     *
     * @param pipeline The name of the pipeline to be checked.
     * @param The shader to set as being current.
     *
     * @return `true` if the given pipeline is already the current pipeline,
	 * otherwise `false`.
     */
    bool isCurrent (std::string pipeline, Shader* currentShader = nullptr);

    /**
     * Copy the `source` Render Target to the `target` Render Target.
     *
     * You can optionally set the brightness factor of the copy.
     *
     * The difference between this method and `drawFrame` is that this method
     * uses a faster copy shader, where only the brightness can be modified.
     * If you need color level manipulation, see `drawFrame` instead.
     *
     * The copy itself is handled by the Utility Pipeline.
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
			double brightness = 1., bool clear = true, bool clearAlpha = true);

    /**
     * Pops the framebuffer from the renderers FBO stack and sets that as the
	 * active target, then draws the `source` Render Target to it. It then resets
	 * the renderer textures.
     *
     * This should be done when you need to draw the _final_ results of a pipeline
	 * to the game canvas, or the next framebuffer in line on the FBO stack. You
	 * should only call this once in the `onDraw` handler and it should be the
	 * final thing called. Be careful not to call this if you need to actually use
	 * the pipeline shader, instead of the copy shader. In those cases, use the
	 * `bindAndDraw` method.
     *
     * @since 0.0.0
     *
     * @param source The Render Target to draw from.
     */
    void copyToGame (RenderTarget* source);

    /**
     * Copy the `source` Render Target to the `target` RenderTarget, using the
     * given Color Matrix.
     *
     * The difference between this method and `copyFrame` is that this method
     * uses a color matrix shader, where you have full control over the luminance
     * values used during the copy. If you don't need this, you can use the faster
     * `copyFrame` method instead.
     *
     * The copy itself is handled by the Utility Pipeline.
     *
     * @since 0.0.0
     *
     * @param source The source Render Target.
     * @param target The target Render Target.
     * @param clearAlpha Clear the alpha channel when running glClear on the target?
     * @param colorMatrix The Color Matrix to use when performing the draw.
     */
    void drawFrame (RenderTarget* source, RenderTarget* target,
			bool clearAlpha, ColorMatrix* colorMatrix);

    /**
     * Draws the `source1` and `source2` Render Targets to the `target`
	 * RenderTarget using a linear blend effect, which is controlled by the
	 * `strength` parameter.
     *
     * The draw itself is handled by the Utility Pipeline.
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
			RenderTarget* target, double strength = 1., bool clearAlpha = true);

    /**
     * Draws the `source1` and `source2` RenderTargets to the `target` RenderTarget
	 * using an additive blend effect, which is controlled by the `strength`
	 * parameter.
     *
     * The draw itself is handled by the Utility Pipeline.
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
			RenderTarget* target, double strength = 1., bool clearAlpha = true);

    /**
     * Clears the given Render Target.
     *
     * @since 0.0.0
     *
     * @param target The Render Target to clear.
     * @param clearAlpha Clear the alpha channel when running glClear on the target?
     */
    void clearFrame (RenderTarget* target, bool clearAlpha = true);

    /**
     * Copy the `source` RenderTarget to the `target` RenderTarget.
     *
     * The difference with this copy is that no resizing takes place. If the
	 * `source` RenderTarget is larger than the `target` then only a portion the
	 * same size as the `target` dimensions is copied across.
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
     * @param [eraseMode Erase source from target using ERASE Blend Mode?
     */
    void blitFrame (RenderTarget* source, RenderTarget* target,
			double brightness = 1., bool clear = true, bool clearAlpha = true,
			bool eraseMode = false);

    /**
     * Binds the `source` RenderTarget and then copies a section of it to the
	 * `target` RenderTarget.
     *
     * This method is extremely fast because it uses glCopyTexSubImage2D and doesn't
     * require the use of any shaders. Remember the coordinates are given
	 * in standard OpenGL format, where x and y specify the lower-left corner of
	 * the section, not the top-left. Also, the copy entirely replaces the contents
	 * of the target texture, no 'merging' or 'blending' takes place.
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

    /**
     * Returns `true` if the current pipeline is forced to use texture unit zero.
     *
     * @since 0.0.0
     *
     * @return `true` if the current pipeline is forced to use texture unit zero.
     */
    bool forceZero ();

    /**
     * Sets the Multi Pipeline to be the currently bound pipeline.
     *
     * This is the default Zenith rendering pipeline.
     *
     * @since 0.0.0
     *
     * @return The Multi Pipeline instance.
     */
    MultiPipeline& setMulti ();

    /**
     * Sets the Utility Pipeline to be the currently bound pipeline.
     *
     * @since 0.0.0
     *
     * @param currentShader The shader to set as being current.
     *
     * @return The Utility Pipeline instance.
     */
    UtilityPipeline& setUtility (Shader* currentShader);

    /**
     * Use this to reset the gl context to the state that Zenith requires to
	 * continue rendering.
     *
     * Calling this will:
     *
     * * Disable `GL_DEPTH_TEST`, `GL_CULL_FACE` and `GL_STENCIL_TEST`.
     * * Clear the depth buffer and stencil buffers.
     * * Reset the viewport size.
     * * Reset the blend mode.
     * * Bind a blank texture as the active texture on texture unit zero.
     * * Rebinds the given pipeline instance.
     *
     * You should call this if you have previously called `clear`, and then wish to
	 * return rendering control to Zenith again.
     *
     * @since 0.0.0
     *
     * @param pipeline The pipeline instance to be rebound. If not given, the
	 * previous pipeline will be bound.
     */
    void rebind (std::string pipeline = "");

    /**
     * Flushes the current pipeline being used and then clears it, along with the
     * the current shader program and vertex buffer from the Renderer.
     *
     * Then resets the blend mode to NORMAL.
     *
     * Call this before jumping to your own gl context handler, and then call
	 * `rebind` when you wish to return control to Zenith again.
     *
     * @since 0.0.0
     */
    void clear ();

	/**
	 * This map stores all pipeline instances in this manager.
	 *
	 * This is populated with the default pipelines in the `boot` method.
	 *
	 * @since 0.0.0
	 */
	std::map<std::string, std::unique_ptr<Pipeline>> pipelines;

	/**
	 * Current pipeline in use by the Renderer.
	 *
	 * @since 0.0.0
	 */
	Pipeline *current = nullptr;

	/**
	 * The previous Pipeline that was in use.
	 *
	 * This is set when `clearPipeline` is called and restored in `rebindPipeline`
	 * if none is given.
	 *
	 * @since 0.0.0
	 */
	Pipeline *previous = nullptr;

	/**
	 * A pointer to the Full Frame 1 Render Target that belongs to the
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
	 * A pointer to the Full Frame 2 Render Target that belongs to the
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
	 * A pointer to the Half Frame 1 Render Target that belongs to the
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
	 * A pointer to the Half Frame 2 Render Target that belongs to the
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

	UtilityPipeline *UTILITY_PIPELINE;

	MultiPipeline *MULTI_PIPELINE;
	
	BitmapMaskPipeline *BITMAPMASK_PIPELINE;
};

}	// namespace Zen

#endif
