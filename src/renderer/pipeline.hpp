/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_PIPELINE_HPP
#define ZEN_RENDERER_PIPELINE_HPP

#include "shader.hpp"
#include "../event/event_emitter.hpp"
#include "types/gl_pipeline_config.hpp"
#include "render_target.hpp"
#include <memory>
#include <string>
#include <utility>
#include <deque>
#include <optional>
#include <vector>

namespace Zen {

/**
 * The `Pipeline` is a base class used by all of the core Zenith pipelines.
 *
 * It describes the way elements will be rendered in OpenGL. Internally, it handles
 * compiling the shaders, creating vertex buffers, assigning primitive topology and
 * binding vertex attributes, all based on the given configuration data.
 *
 * The pipeline is configured by passing in a `PipelineConfig` object. Please
 * see the documentation for this type to fully understand the configuration options
 * available to you.
 *
 * Usually, you would not inherit from this class directly, but would instead extend
 * from one of the core pipelines, such as the Multi Pipeline.
 *
 * The pipeline flow per render-step is as follows:
 *
 * 1. onPreRender - called once at the start of the render step
 * 2. onRender - call for each Scene Camera that needs to render (so can be
 * multiple times per render step)
 * 3. Internal flow:
 *		a. bind (only called if a Game Object is using this pipeline and it's not
 *		currently active)
 *		b. onBind (called for every Game Object that uses this pipeline)
 *		c. flush (can be called by a Game Object, internal method or from outside
 *		by changing pipeline)
 * 4. onPostRender - called once at the end of the render step
 *
 * @class Pipeline
 * @since 0.0.0
 */
class Pipeline : public EventEmitter {
public:
	/**
	 * @since 0.0.0
	 *
	 * @param config The configuration object for this Pipeline.
	 */
	Pipeline (PipelineConfig config);

	/**
	 * @since 0.0.0
	 */
	~Pipeline ();

    /**
     * Called when the Game has fully booted and the Renderer has finished setting
	 * up.
     *
     * By this stage all Game level systems are now in place. You can perform any
	 * final tasks that the pipeline may need, that relies on game systems such as
	 * the Texture Manager being ready.
     *
     * @since 0.0.0
     */
	virtual void boot();

    /**
     * This method is called once when this pipeline has finished being set-up
     * at the end of the boot process. By the time this method is called, all
     * of the shaders are ready and configured.
     *
     * @since 0.0.0
     */
	void onBoot ();

    /**
     * This method is called once when this pipeline has finished being set-up
     * at the end of the boot process. By the time this method is called, all
     * of the shaders are ready and configured. It's also called if the renderer
     * changes size.
     *
     * @since 0.0.0
     *
     * @param width The new width of this Pipeline.
     * @param height The new height of this Pipeline.
     */
	void onResize (double width, double height);

    /**
     * Sets the currently active shader within this pipeline.
     *
     * @since 0.0.0
     *
     * @param shader The shader to set as being current.
     * @param setAttributes Should the vertex attribute pointers be set?
     */
	void setShader (Shader *shader, bool setAttributes = false);

    /**
     * Returns the shader instance matchine the given name.
     *
     * @since 0.0.0
     *
     * @param name The name of the shader to set.
     *
     * @return The Shader instance, if found.
     */
	Shader* getShaderByName (std::string name);

    /**
     * Destroys all shaders currently set in the `Pipeline::shaders` array and
	 * then parses the given `config` object, extracting the shaders from it,
	 * creating `Shader` instances and finally setting them into the `shaders`
	 * array of this pipeline.
     *
     * This is a destructive process. Be very careful when you call it, should you
	 * need to.
     *
     * @since 0.0.0
     *
     * @param config The configuration object for this pipeline.
     */
	void setShadersFromConfig (PipelineConfig config);

    /**
     * Custom pipelines can use this method in order to perform any required
	 * pre-batch tasks for the given Game Object. It must return the texture unit
	 * the Game Object was assigned.
     *
     * @since 0.0.0
     *
     * @param gameObject The Game Object being rendered or added to the batch.
     *
     * @return The texture unit the Game Object has been assigned.
     */
    int setGameObject (Entity gameObject);

    /**
     * Check if the current batch of vertices is full.
     *
     * You can optionally provide an `amount` parameter. If given, it will check if
	 * the batch needs to flush _if_ the `amount` is added to it. This allows you
	 * to test if you should flush before populating the batch.
     *
     * @since 0.0.0
     *
     * @param amount Will the batch need to flush if this many vertices are added
	 * to it?
     *
     * @return `true` if the current batch should be flushed, otherwise `false`.
     */
    bool shouldFlush (int amount = 0);

    /**
     * Resizes the properties used to describe the viewport.
     *
     * This method is called automatically by the renderer during its resize
	 * handler.
     *
     * @since 0.0.0
     *
     * @param width The new width of this pipeline.
     * @param height The new height of this pipeline.
     */
    virtual void resize (double width, double height);
    /**
     * Adjusts this pipelines orthonormal Projection Matrix to use the given
	 * dimensions and resets the `uProjectionMatrix` uniform on all bound shaders.
     *
     * This method is called automatically by the renderer during its resize
	 * handler.
     *
     * @since 0.0.0
     *
     * @param width The new width of this pipeline.
     * @param height The new height of this pipeline.
     */
    void setProjectionMatrix (double width, double height);

    /**
     * Adjusts this pipelines orthonormal Projection Matrix to match that of the
	 * global Renderer Projection Matrix.
     *
     * This method is called automatically by the Pipeline Manager when this
     * pipeline is set.
     *
     * @since 0.0.0
     */
    void updateProjectionMatrix ();

    /**
     * This method is called every time the Pipeline Manager makes this pipeline
	 * the currently active one.
     *
     * It binds the resources and shader needed for this pipeline, including
	 * setting the vertex buffer and attribute pointers.
     *
     * @since 0.0.0
     *
     * @param currentShader The shader to set as being current.
     */
    void bind (Shader *currentShader = nullptr);

    /**
     * This method is called every time the Pipeline Manager rebinds this pipeline.
     *
     * It resets all shaders this pipeline uses, setting their attributes again.
     *
     * @since 0.0.0
     */
    void rebind ();

    /**
     * @since 0.0.0
     *
     * @return `true` if the vertex array was bound, or `false` if it was already
	 * bound.
     */
    bool setVertexArray ();

    /**
     * Binds the vertex buffer to be the active ARRAY_BUFFER on the OpenGL context.
     *
     * It first checks to see if it's already set as the active buffer and only
     * binds itself if not.
     *
     * @since 0.0.0
     *
     * @return `true` if the vertex buffer was bound, or `false` if it was already
	 * bound.
     */
    bool setVertexBuffer ();

    /**
     * This method is called as a result of the `Pipeline.batchQuad` method,
	 * right before a quad belonging to a Game Object is about to be added to the
	 * batch. When this is called, the renderer has just performed a flush. It will
	 * bind the current render target, if any are set and finally call the
	 * `onPreBatch` hook.
     *
     * @since 0.0.0
     *
     * @param entity The Game Object or Camera that invoked this pipeline, if any.
     */
    void preBatch (Entity entity);

    /**
     * This method is called as a result of the `Pipeline.batchQuad` method,
	 * right after a quad belonging to a Game Object has been added to the batch.
	 * When this is called, the renderer has just performed a flush.
     *
     * It calls the `onDraw` hook followed by the `onPostBatch` hook, which can be
	 * used to perform additional Post FX Pipeline processing.
     *
     * @since 0.0.0
     *
     * @param entity The Game Object or Camera that invoked this pipeline, if any.
     */
    void postBatch (Entity entity);

    /**
     * This method is only used by Post FX Pipelines and those that inherit from
	 * them.
     *
     * This method is called every time the `postBatch` method is called and is
	 * passed a reference to the current render target.
     *
     * At the very least a Post FX Pipeline should call
	 * `bindAndDraw(renderTarget)`, however, you can do as much additional
	 * processing as you like in this method if you override it from within your
	 * own pipelines.
     *
     * @since 0.0.0
     *
     * @param renderTarget The Render Target.
     */
    virtual void onDraw (RenderTarget* renderTarget);

    /**
     * This method is called every time the Pipeline Manager deactivates this
	 * pipeline, swapping from it to another one. This happens after a call to
	 * `flush` and before the new pipeline is bound.
     *
     * @since 0.0.0
     */
    void unbind ();

    /**
     * Uploads the vertex data and emits a draw call for the current batch of
	 * vertices.
     *
     * @since 0.0.0
     *
     * @param isPostFlush Was this flush invoked as part of a post-process, or not?
     */
    void flush (bool isPostFlush = false);

    /**
     * By default this is an empty method hook that you can override and use in
	 * your own custom pipelines.
     *
     * This method is called every time the Pipeline Manager makes this the active
	 * pipeline. It is called at the end of the `Pipeline::bind` method, after
	 * the current shader has been set. The current shader is passed to this hook.
     *
     * For example, if a display list has 3 Sprites in it that all use the same
	 * pipeline, this hook will only be called for the first one, as the 2nd and
	 * 3rd Sprites do not cause the pipeline to be changed.
     *
     * If you need to listen for that event instead, use the `onBind` hook.
     *
     * @since 0.0.0
     *
     * @param currentShader The shader that was set as current.
     */
    virtual void onActive (Shader* currentShader);

    /**
     * By default this is an empty method hook that you can override and use in
	 * your own custom pipelines.
     *
     * This method is called every time a **Game Object** asks the Pipeline Manager
	 * to use this pipeline, even if the pipeline is already active.
     *
     * Unlike the `onActive` method, which is only called when the Pipeline Manager
	 * makes this pipeline active, this hook is called for every Game Object that
	 * requests use of this pipeline, allowing you to perform per-object set-up,
	 * such as loading shader uniform data.
     *
     * @since 0.0.0
     *
     * @param gameObject The Game Object that invoked this pipeline, if any.
     */
    virtual void onBind (Entity entity);

    /**
     * By default this is an empty method hook that you can override and use in
	 * your own custom pipelines.
     *
     * This method is called when the Pipeline Manager needs to rebind this
	 * pipeline. This happens after a pipeline has been cleared.
	 *
     * @since 0.0.0
     */
    virtual void onRebind ();

    /**
     * By default this is an empty method hook that you can override and use in
	 * your own custom pipelines.
     *
     * This method is called every time the `batchQuad` or `batchTri` methods are
	 * called. If this was as a result of a Game Object, then the Game Object
	 * reference is passed to this hook too.
     *
     * This hook is called _after_ the quad (or tri) has been added to the batch,
	 * so you can safely call 'flush' from within this.
     *
     * Note that Game Objects may call `batchQuad` or `batchTri` multiple times for
	 * a single draw, for example the Graphics Game Object.
     *
     * @since 0.0.0
     *
     * @param gameObject The Game Object that invoked this pipeline, if any.
     */
	virtual void onBatch (Entity entity = entt::null);

    /**
     * By default this is an empty method hook that you can override and use in
	 * your own custom pipelines.
     *
     * This method is called immediately before a **Game Object** is about to add
	 * itself to the batch.
     *
     * @since 0.0.0
     *
     * @param gameObject The Game Object that invoked this pipeline, if any.
     */
    virtual void onPreBatch (Entity entity = entt::null);

    /**
     * By default this is an empty method hook that you can override and use in
	 * your own custom pipelines.
     *
     * This method is called immediately after a **Game Object** has been added to
	 * the batch.
     *
     * @since 0.0.0
     *
     * @param gameObject The Game Object that invoked this pipeline, if any.
     */
    virtual void onPostBatch (Entity entity = entt::null);

    /**
     * By default this is an empty method hook that you can override and use in
	 * your own custom pipelines.
     *
     * This method is called once per frame, right before anything has been
	 * rendered, but after the canvas has been cleared. If this pipeline has a
	 * render target, it will also have been cleared by this point.
     *
     * @since 0.0.0
     */
    virtual void onPreRender ();

    /**
     * By default this is an empty method hook that you can override and use in
	 * your own custom pipelines.
     *
     * This method is called _once per frame_, by every Camera in a Scene that
	 * wants to render.
     *
     * It is called at the start of the rendering process, before anything has been
	 * drawn to the Camera.
     *
     * @since 0.0.0
     *
     * @param camera - The Scene Camera being rendered with.
     */
    virtual void onRender (Entity camera);

    /**
     * By default this is an empty method hook that you can override and use in
	 * your own custom pipelines.
     *
     * This method is called _once per frame_, after all rendering has happened and
	 * snapshots have been taken.
     *
     * It is called at the very end of the rendering process, once all Cameras, for
	 * all Scenes, have been rendered.
     *
     * @since 0.0.0
     */
    virtual void onPostRender ();

    /**
     * By default this is an empty method hook that you can override and use in
	 * your own custom pipelines.
     *
     * This method is called every time this pipeline is asked to flush its batch.
     *
     * It is called immediately before the `glBufferData` and `glDrawArrays` calls
	 * are made, so you can perform any final pre-render modifications. To apply
	 * changes post-render, see `onAfterFlush`.
     *
     * @since 0.0.0
     *
     * @param isPostFlush Was this flush invoked as part of a post-process, or not?
     */
    virtual void onBeforeFlush (bool isPostFlush = false);

    /**
     * By default this is an empty method hook that you can override and use in
	 * your own custom pipelines.
     *
     * This method is called immediately after this pipeline has finished flushing
	 * its batch.
     *
     * It is called after the `glDrawArrays` call.
     *
     * You can perform additional post-render effects, but be careful not to call
	 * `flush` on this pipeline from within this method, or you'll cause an
	 * infinite loop.
     *
     * To apply changes pre-render, see `onBeforeFlush`.
     *
     * @since 0.0.0
     *
     * @param isPostFlush Was this flush invoked as part of a post-process, or not?
     */
    virtual void onAfterFlush (bool isPostFlush = false);

    /**
     * Adds a single vertex to the current vertex buffer and increments the
     * `vertexCount` property by 1.
     *
     * This method is called directly by `batchTri` and `batchQuad`.
     *
     * It does not perform any batch limit checking itself, so if you need to call
     * this method directly, do so in the same way that `batchQuad` does, for example.
     *
     * @since 0.0.0
     *
     * @param x The vertex x position.
     * @param y The vertex y position.
     * @param u UV u value.
     * @param v UV v value.
     * @param unit Texture unit to which the texture needs to be bound.
     * @param tintEffect The tint effect for the shader to use. -1 for none.
     * @param tint The tint color value.
     */
    void batchVert (double x, double y, double u, double v, int unit,
			int tintEffect, int tint);

    /**
     * Adds the vertices data into the batch and flushes if full.
     *
     * Assumes 6 vertices in the following arrangement:
     *
     * ```
     * 0----3
     * |\  B|
     * | \  |
     * |  \ |
     * | A \|
     * |    \
     * 1----2
     * ```
     *
     * Where tx0/ty0 = 0, tx1/ty1 = 1, tx2/ty2 = 2 and tx3/ty3 = 3
     *
     * @since 0.0.0
     *
     * @param gameObject The Game Object, if any, drawing this quad.
     * @param x0 The top-left x position.
     * @param y0 The top-left y position.
     * @param x1 The bottom-left x position.
     * @param y1 The bottom-left y position.
     * @param x2 The bottom-right x position.
     * @param y2 The bottom-right y position.
     * @param x3 The top-right x position.
     * @param y3 The top-right y position.
     * @param u0 UV u0 value.
     * @param v0 UV v0 value.
     * @param u1 UV u1 value.
     * @param v1 UV v1 value.
     * @param tintTL The top-left tint color value.
     * @param tintTR The top-right tint color value.
     * @param tintBL The bottom-left tint color value.
     * @param tintBR The bottom-right tint color value.
     * @param tintEffect The tint effect for the shader to use.
     * @param texture GL_texture that will be assigned to the current batch if a
	 * flush occurs.
     * @param unit Texture unit to which the texture needs to be bound.
     *
     * @return `true` if this method caused the batch to flush, otherwise `false`.
     */
    bool batchQuad (Entity gameObject, double x0, double y0, double x1, double y1,
			double x2, double y2, double x3, double y3, double u0, double v0,
			double u1, double v1, int tintTL, int tintTR, int tintBL, int tintBR,
			int tintEffect, GL_texture texture, GLenum unit = GL_NONE);

    /**
     * Adds the vertices data into the batch and flushes if full.
     *
     * Assumes 3 vertices in the following arrangement:
     *
     * ```
     * 0
     * |\
     * | \
     * |  \
     * |   \
     * |    \
     * 1-----2
     * ```
     *
     * @since 0.0.0
     *
     * @param gameObject The Game Object, if any, drawing this quad.
     * @param x1 The bottom-left x position.
     * @param y1 The bottom-left y position.
     * @param x2 The bottom-right x position.
     * @param y2 The bottom-right y position.
     * @param x3 The top-right x position.
     * @param y3 The top-right y position.
     * @param u0 UV u0 value.
     * @param v0 UV v0 value.
     * @param u1 UV u1 value.
     * @param v1 UV v1 value.
     * @param tintTL The top-left tint color value.
     * @param tintTR The top-right tint color value.
     * @param tintBL The bottom-left tint color value.
     * @param tintEffect The tint effect for the shader to use.
     * @param texture Texture that will be assigned to the current batch if a
	 * flush occurs.
     * @param unit Texture unit to which the texture needs to be bound.
     *
     * @return `true` if this method caused the batch to flush, otherwise `false`.
     */
    bool batchTri (Entity gameObject, double x0, double y0, double x1, double y1,
			double x2, double y2, double u0, double v0, double u1, double v1,
			int tintTL, int tintTR, int tintBL, int tintEffect,
			GL_texture texture, GLenum unit = GL_NONE);

    /**
     * Pushes a filled rectangle into the vertex batch.
     *
     * The dimensions are run through `std::floor` before the quad is generated.
     *
     * Rectangle has no transform values and isn't transformed into the local space.
     *
     * Used for directly batching untransformed rectangles, such as Camera
	 * background colors.
     *
     * @since 0.0.0
     *
     * @param x Horizontal top left coordinate of the rectangle.
     * @param y Vertical top left coordinate of the rectangle.
     * @param width Width of the rectangle.
     * @param height Height of the rectangle.
     * @param color Color of the rectangle to draw.
     * @param alpha Alpha value of the rectangle to draw.
     * @param texture GL_texture that will be assigned to the current batch if a
	 * flush occurs.
     * @param flipUV Flip the vertical UV coordinates of the texture before
	 * rendering?
     */
    void drawFillRect (double x, double y, double width, double height, int color,
			double alpha, GL_texture texture = 0, bool flipUV = true);

    /**
     * sets the texture to be bound to the next available texture unit and returns
     * the unit id.
     *
     * @since 0.0.0
     * @param texture gl_texture that will be assigned to the current batch. if not
	 * given uses `whitetexture`.
     * @return the assigned texture unit.
     */
    GLenum setTexture2D (GL_texture texture = 0);

    /**
     * Activates the given GL_texture and binds it to the requested texture slot.
     *
     * @since 0.0.0
     *
     * @param target The GL_texture to activate and bind.
     * @param unit The GL_texture ID to activate. Defaults to `GL_TEXTURE0`.
     */
    void bindTexture (GL_texture texture, GLenum unit = GL_TEXTURE0);

    /**
     * Activates the given Render Target texture and binds it to the
     * requested GL_texture slot.
     *
     * @since 0.0.0
     *
     * @param target The Render Target to activate and bind.
     * @param unit The GL_texture ID to activate. Defaults to `GL_TEXTURE0`.
     */
    void bindRenderTarget (RenderTarget* target, GLenum unit = GL_TEXTURE0);

    /**
     * Sets the current duration into a 1f uniform value based on the given name.
     *
     * This can be used for mapping time uniform values, such as `iTime`.
     *
     * @since 0.0.0
     *
     * @param name The name of the uniform to set.
     */
    void setTime (std::string uniform);

	/**
	 * Set a uniform value based on the given name on the given currently
	 * set shader.
	 *
	 * @since 0.0.0
	 * @param name The name of the uniform to set.
	 * @param args The value to set.
	 */
	template <typename ... Args>
	void set (std::string name, Args&& ... args)
	{
		currentShader->set(name, std::forward<Args>(args)...);
	}

	/**
	 * Set a uniform value based on the given name on the given shader.
	 *
     * The current shader is bound, before the uniform is set, making it active
	 * within the Renderer. This means you can safely call this method from a
	 * location such as a Scene `create` or `update` method. However, when working
	 * within a Shader file directly, use the `Shader` method equivalent
	 * instead, to avoid the program being set.
	 *
	 * @since 0.0.0
	 * @param shader The shader to set the values on.
	 * @param name The name of the uniform to set.
	 * @param args The value to set.
	 */
	template <typename ... Args>
	void set (Shader* shader, std::string name, Args&& ... args)
	{
		if (!shader)
			return;

		shader->set(name, std::forward<Args>(args)...);
	}

	void setCurrentShader (Shader *shader);
	void setCurrentProgram (GLuint program);

	/**
	 * The name of the pipeline.
	 *
	 * @since 0.0.0
	 */
	std::string name = "gl-pipeline";

	/**
	 * Width of the current viewport.
	 *
	 * @since 0.0.0
	 */
	double width = 0;

	/**
	 * Height of the current viewport.
	 *
	 * @since 0.0.0
	 */
	double height = 0;

	/**
	 * The current number of vertices that have been added to the pipeline batch.
	 *
	 * @since 0.0.0
	 */
	int vertexCount = 0;

	/**
	 * The total number of vertices that this pipeline batche can hold before it
	 * will flush.
	 *
	 * @since 0.0.0
	 */
	int vertexCapacity = 0;

	/**
	 * Raw byte buffer of vertices.
	 *
	 * @since 0.0.0
	 */
	std::vector<std::uint8_t> vertexData;

	/**
	 * The vertex array that holds the render state.
	 *
	 * @since 0.0.0
	 */
	GL_vao vertexArray = 0;

	/**
	 * The buffer that holds the vertex data.
	 *
	 * Created from the `vertexData` array. If `vertices` are set in the config, a
	 * `STATIC_DRAW` buffer is created. If not, a `DYNAMIC_DRAW` buffer is created.
	 *
	 * @since 0.0.0
	 */
	GLuint vertexBuffer = 0;

	/**
	 * The primitive topology which the pipeline will use to submit draw calls.
	 *
	 * @since 0.0.0
	 */
	GLenum topology = GL_TRIANGLES;

	/**
	 * Indicates if the current pipeline is active, or not.
	 *
	 * Toggle this property to enable or disable a pipeline from rendering anything.
	 *
	 * @since 0.0.0
	 */
	bool active = true;

	/**
	 * Holds the most recently assigned texture unit.
	 *
	 * @since 0.0.0
	 */
	int currentUnit = 0;


	/**
	 * Some pipelines require the forced use of texture zero (like the light
	 * pipeline).
	 *
	 * This property should be set when that is the case.
	 *
	 * @since 0.0.0
	 */
	bool forceZero = false;

	/**
	 * Indicates if this pipeline has booted or not.
	 *
	 * A pipeline boots only when the Game instance itself, and all associated
	 * systems, is fully ready.
	 *
	 * @since 0.0.0
	 */
	bool hasBooted = false;

	/**
	 * Indicates if this is a Post FX Pipeline, or not.
	 *
	 * @since 0.0.0
	 */
	bool isPostFX = false;

	/**
	 * An array of RenderTarget instances that belong to this pipeline.
	 *
	 * @since 0.0.0
	 */
	std::deque<RenderTarget> renderTargets;

	/**
	 * A reference to the currently bound Render Target instance from the
	 * `Pipeline.renderTargets` array.
	 *
	 * @since 0.0.0
	 */
	RenderTarget *currentRenderTarget = nullptr;

	/**
	 * An array of all the Shader instances that belong to this pipeline.
	 *
	 * Shaders manage their own attributes and uniforms, but share the same vertex
	 * data buffer, which belongs to this pipeline.
	 *
	 * Shaders are set in a call to the `setShadersFromConfig` method, which
	 * happens automatically, but can also be called at any point in your game. See
	 * the method documentation for details.
	 *
	 * @since 0.0.0
	 */
	std::map<std::string, std::unique_ptr<Shader>> shaders;

	/**
	 * A pointer to the currently bound Shader instance from the `Pipeline.shaders` array.
	 *
	 * For lots of pipelines, this is the only shader, so it is might rarely change.
	 *
	 * @since 0.0.0
	 */
	Shader *currentShader = nullptr;

	/**
	 * The Projection matrix, used by shaders as 'uProjectionMatrix' uniform.
	 *
	 * @since 0.0.0
	 */
	glm::mat4 projectionMatrix;

	/**
	 * The cached width of the Projection matrix.
	 *
	 * @since 0.0.0
	 */
	double projectionWidth = 0;

	/**
	 * The cached height of the Projection matrix.
	 *
	 * @since 0.0.0
	 */
	double projectionHeight = 0;

	/**
	 * The configuration object that was used to create this pipeline.
	 *
	 * @since 0.0.0
	 */
	const PipelineConfig config;
};

}	// namespace Zen

#endif
