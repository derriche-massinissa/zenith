/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_HPP
#define ZEN_RENDERER_HPP

#include <SDL2/SDL.h>
#include <functional>
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>
#include <optional>

#include "../enums/blend_modes.hpp"
#include "../core/types/render_config.hpp"

#include "../ecs/entity.hpp"
#include "../event/event_emitter.hpp"
#include "../math/types/vector2.hpp"
#include "../display/types/color.hpp"
#include "../structs/types/size.hpp"
#include "../components/transform_matrix.hpp"

#include "../scene/scene.fwd.hpp"
#include "../core/config.fwd.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "types/gl_types.hpp"

#include "render_target.hpp"
#include "pipeline_manager.hpp"

namespace Zen {

struct Mask_ {
	Entity mask = entt::null;
	Entity camera = entt::null;
};

struct TextureInfo_ {
	bool isRenderTexture = false;
	int width = 0;
	int height = 0;
	int glIndex = 0;
	int glIndexCounter = -1;
};

struct FramebufferInfo_ {
	GL_texture colorBuffer;
	GLuint renderBuffer;
};

struct BlendMode {
	/**
	 * The equations used for the OpenGL blending operation.
	 *
	 * @since 0.0.0
	 */
	std::vector<GLenum> equation;

	/**
	 * The factors used for the OpenGL blending operation.
	 *
	 * @since 0.0.0
	 */
	std::vector<GLenum> func;
};

/**
 * @since 0.0.0
 *
 * @property x The x coordinate to start the snapshot from.
 * @property y The y coordinate to start the snapshot from.
 * @property width The width of the snapshot.
 * @property height The height of the snapshot.
 * @property getPixel Is this a snapshot to get a single pixel, or an area?
 * @property path The path to save the image to. This also defines the file type,
 * or `png` if only directories are given.
 * @property callback The function to call once the snapshot is created.
 * @property callbackPixel The function to call once a pixel snapshot is created.
 * @property surface A pointer to the surface the snapshot was saved in.
 * @property active If set to `true`, a snapshot will be taken after the current
 * frame is fully rendered. It will then be set back to `false`;
 */
struct SnapshotState
{
	int x = 0;

	int y = 0;

	int width = 1;

	int height = 1;

	bool getPixel = false;

	std::string path = "";

	std::function<void(SDL_Surface*)> callback = nullptr;

	std::function<void(Color)> callbackPixel = nullptr;

	/**
	 * An internal surface pointer that keeps that last snapshot taken, or has
	 * `nullptr` if no snapshot has been taken since the creation of this renderer.
	 *
	 * @since 0.0.0
	 */
	SDL_Surface *surface = nullptr;

	bool active = false;

	bool isFramebuffer = false;

	int bufferWidth = 1;

	int bufferHeight = 1;
};

class Renderer : public EventEmitter
{
//	GameConfig *config;
//
//
//
//private:
//	/**
//	 * Takes a snapshot of an area from the current frame.
//	 *
//	 * @since 0.0.0
//	 */
//	void takeSnapshot ();
//
//	/**
//	 * Saves the snapshot surface in an image file.
//	 *
//	 * @since 0.0.0
//	 */
//	void saveSnapshot ();
//
//





























public:
	~Renderer ();

    /**
     * Creates a new OpenGL context and initializes all internal state.
     *
     * @since 0.0.0
     *
     * @param config The configuration object for the renderer.
     */
    void init (GameConfig* config);
	/**
	 * ^
	 * |
	 * This one to replace?
	 *
	 *
	 *
	 * Start up this renderer. This _MUST_ run after the Window was created!
	 *
	 * @since 0.0.0
	 */
	void start (GameConfig *cfg_);

	/**
	 * Instantiate all the supported blend modes of this Renderer.
	 *
	 * @since 0.0.0
	 */
	void createBlendModes ();

    /**
     * Internal boot handler. Calls 'boot' on each pipeline.
     *
     * @since 0.0.0
     */
    void boot ();

    /**
     * The event handler that manages the `resize` event dispatched by the Scale
	 * Manager.
     *
     * @since 0.0.0
     *
     * @param gameSize The default Game Size object. This is the un-modified game
	 * dimensions.
     * @param baseSize The base Size object. The game dimensions. The canvas
	 * width / height values match this.
     */
    void onResize (Size gameSize, Size baseSize, int previousWidth,
			int previousHeight);

    /**
     * Binds the Renderer's Render Target, so all drawn content is now redirected
	 * to it.
     *
     * Make sure to call `endCapture` when you are finished.
     *
     * @since 0.0.0
     *
     * @param width Optional new width of the Render Target.
     * @param height Optional new height of the Render Target.
     */
    void beginCapture (double width = -1, double height = -1);

    /**
     * Unbinds the Renderer's Render Target and returns it, stopping any further
	 * content being drawn to it.
     *
     * If the viewport or scissors were modified during the capture, you should
	 * reset them by calling `resetViewport` and `resetScissor` accordingly.
     *
     * @since 0.0.0
     *
     * @return A pointer to the Renderer's Render Target.
     */
    RenderTarget* endCapture ();

    /**
     * Resizes the drawing buffer to match that required by the Scale Manager.
     *
     * @since 0.0.0
     *
     * @param width The new width of the renderer.
     * @param height The new height of the renderer.
     */
    void resize (double width, double height);

    /**
     * Gets the aspect ratio of the Renderer's dimensions.
     *
     * @since 0.0.0
     *
     * @return The aspect ratio of the Renderer dimensions.
     */
    double getAspectRatio ();

    /**
     * Sets the Projection Matrix of this renderer to the given dimensions.
     *
     * @since 0.0.0
     *
     * @param width The new width of the Projection Matrix.
     * @param height The new height of the Projection Matrix.
     */
	void setProjectionMatrix (double width, double height);

    /**
     * Resets the Projection Matrix back to this renderer's width and height.
     *
     * This is called during `endCapture`, should the matrix have been changed
     * as a result of the capture process.
     *
     * @since 0.0.0
     */
	void resetProjectionMatrix ();

    /**
     * Flushes the current pipeline if the pipeline is bound
     *
     * @since 0.0.0
     */
	void flush ();

    /**
     * Pushes a new scissor state. This is used to set nested scissor states.
     *
     * @since 0.0.0
     *
     * @param x The x position of the scissor.
     * @param y The y position of the scissor.
     * @param width The width of the scissor.
     * @param height The height of the scissor.
     *
     * @return A vector containing the scissor values.
     */
	std::array<int, 4> pushScissor (int x, int y, int width, int height);

    /**
     * Sets the current scissor state.
     *
     * @since 0.0.0
     *
     * @param x The x position of the scissor.
     * @param y The y position of the scissor.
     * @param width The width of the scissor.
     * @param height The height of the scissor.
     */
	void setScissor (int x, int y, int width, int height);

    /**
     * Resets the gl scissor state to be whatever the current scissor is, if there
	 * is one, without modifying the scissor stack.
     *
     * @since 0.0.0
     */
	void resetScissor ();

    /**
     * Pops the last scissor state and sets it.
     *
     * @since 0.0.0
     */
	void popScissor ();

    /**
     * Is there an active stencil mask?
     *
     * @since 0.0.0
     *
     * @return `true` if there is an active stencil mask, otherwise `false`.
     */
	bool hasActiveStencilMask ();

    /**
     * Resets the gl viewport to the current renderer dimensions.
     *
     * @since 0.0.0
     */
	void resetViewport ();

    /**
     * Sets the blend mode to the value given.
     *
     * If the current blend mode is different from the one given, the pipeline is
	 * flushed and the new blend mode is enabled.
     *
     * @since 0.0.0
     *
     * @param blendModeId The blend mode to be set.
     * @param force Force the blend mode to be set, regardless of the currently set
	 * blend mode.
     *
     * @return `true` if the blend mode was changed as a result of this call,
	 * forcing a flush, otherwise `false`.
     */
	bool setBlendMode (int modeId, bool force = false);

    /**
	 * Overload to use with the enum class BLEND_MODE.
	 * ```cpp
	 * // To do:
	 * setBlendMode(BLEND_MODE::ADD)
	 * // Instead of:
	 * setBlendMode(2)
	 * ```
	 *
	 * @overload
     * @since 0.0.0
     *
     * @param blendModeId The blend mode to be set.
     * @param force Force the blend mode to be set, regardless of the currently set
	 * blend mode.
     *
     * @return `true` if the blend mode was changed as a result of this call,
	 * forcing a flush, otherwise `false`.
     */
	bool setBlendMode (BLEND_MODE mode, bool force = false);

    /**
     * Creates a new custom blend mode for the renderer.
     *
     * See https://developer.mozilla.org/en-US/docs/Web/API/WebGL_API/Constants#Blending_modes
     *
     * @since 0.0.0
     *
     * @param func An array containing the OpenGL functions to use for the source
	 * and the destination blending factors, respectively.
     * @param equation The equation to use for combining the RGB and alpha
	 * components of a new pixel with a rendered one.
     *
     * @return The index of the new blend mode, used for referencing it in the
	 * future.
     */
	int addBlendMode (std::vector<GLenum> func, std::vector<GLenum> equation);

    /**
     * Updates the function bound to a given custom blend mode.
     *
     * @since 0.0.0
     *
     * @param index The index of the custom blend mode.
     * @param func The function to use for the blend mode.
     * @param equation The equation to use for the blend mode.
     */
	void updateBlendMode (int index, std::vector<GLenum> func,
			std::vector<GLenum> equation = {});

    /**
     * Removes a custom blend mode from the renderer.
     * Any Game Objects still using this blend mode will throw an error, so be
	 * sure to clear them first.
     *
     * @since 0.0.0
     *
     * @param index The index of the custom blend mode to be removed.
     */
	void removeBlendMode (int index);

    /**
     * Sets the current active texture for texture unit zero to be a blank texture.
     * This only happens if there isn't a texture already in use by texture unit
	 * zero.
     *
     * @since 0.0.0
     */
	void setBlankTexture ();
	
    /**
     * Activates the Texture Source and assigns it the next available texture unit.
     * If none are available, it will flush the current pipeline first.
     *
     * @since 0.0.0
     *
     * @param textureSource The Texture Source to be assigned the texture unit.
     *
     * @return The texture unit index that was assigned to the Texture Source.
     */
	int setTextureSource (Entity textureSource);

    /**
     * Checks to see if the given diffuse and normal map textures are already
	 * bound, or not.
     *
     * @since 0.0.0
     *
     * @param texture The OpenGL diffuse texture.
     * @param normalMap The OpenGL normal map texture.
     *
     * @return Returns `false` if this combination is already set, or `true` if
	 * it's a new combination.
     */
	bool isNewNormalMap (GL_texture texture, GL_texture normalMap);

    /**
     * Binds a texture directly to texture unit zero then activates it.
     * If the texture is already at unit zero, it skips the bind.
     * Make sure to call `clearTextureZero` after using this method.
     *
     * @since 0.0.0
     *
     * @param texture The OpenGL texture that needs to be bound.
     * @param flush Flush the pipeline if the texture is different?
     */
	void setTextureZero (GL_texture texture, bool flush = false);

    /**
     * Clears the texture that was directly bound to texture unit zero.
     *
     * @since 0.0.0
     */
	void clearTextureZero ();

    /**
     * Binds a texture directly to texture unit one then activates it.
     * If the texture is already at unit one, it skips the bind.
     * Make sure to call `clearNormalMap` after using this method.
     *
     * @since 0.0.0
     *
     * @param texture - The OpenGL texture that needs to be bound.
     */
	void setNormalMap (GL_texture texture);

    /**
     * Clears the texture that was directly bound to texture unit one and
     * increases the start active texture counter.
     *
     * @since 0.0.0
     */
    void clearNormalMap ();

    /**
     * Activates each texture, in turn, then binds them all to `null`.
     *
     * @since 0.0.0
     *
     * @param all Reset all textures, or just the first two?
     */
    void unbindTextures ();

    /**
     * Flushes the current pipeline, then resets the first two textures
     * back to the default temporary textures, resets the start active
     * counter and sets texture unit 1 as being active.
     *
     * @since 0.0.0
     *
     * @param all Reset all textures, or just the first two?
     */
    void resetTextures (bool all = false);

    /**
     * Binds a texture at a texture unit. If a texture is already
     * bound to that unit it will force a flush on the current pipeline.
     *
     * @since 0.0.0
     *
     * @param texture The OpenGL texture that needs to be bound.
     *
     * @return The texture unit that was assigned to the Texture Source.
     */
	int setTexture2D (GL_texture texture);

    /**
     * Pushes a new framebuffer onto the FBO stack and makes it the currently bound
	 * framebuffer.
     *
     * If there was another framebuffer already bound it will force a pipeline
	 * flush.
     *
     * Call `popFramebuffer` to remove it again.
     *
     * @since 0.0.0
     *
     * @param framebuffer The framebuffer that needs to be bound.
     * @param updateScissor Set the gl scissor to match the frame buffer size?
     * @param resetTextures Should the OpenGL Textures be reset after the new
	 * framebuffer is bound?
     * @param setViewport Should the OpenGL viewport be set?
     */
	void pushFramebuffer (GL_fbo framebuffer, bool updateScissor = false,
			bool resetTextures = false, bool setViewport = true);

    /**
     * Sets the given framebuffer as the active and currently bound framebuffer.
     *
     * If there was another framebuffer already bound it will force a pipeline
	 * flush.
     *
     * Typically, you should call `pushFramebuffer` instead of this method.
     *
     * @since 0.0.0
     *
     * @param framebuffer The framebuffer that needs to be bound.
     * @param updateScissor If a framebuffer is given, set the gl scissor to match
	 * the frame buffer size?
     * @param resetTextures Should the OpenGL Textures be reset after the new
	 * framebuffer is bound?
     * @param setViewport Should the OpenGL viewport be set?
     */
	void setFramebuffer (GL_fbo framebuffer = {}, bool updateScissor = false,
			bool resetTextures = false, bool setViewport = true);

    /**
     * Pops the previous framebuffer from the fbo stack and sets it.
     *
     * @since 0.0.0
     *
     * @param updateScissor If a framebuffer is given, set the gl scissor to match
	 * the frame buffer size?
     * @param resetTextures Should the OpenGL Textures be reset after the new
	 * framebuffer is bound?
     * @param setViewport Should the OpenGL viewport be set?
     *
     * @return The Framebuffer that was set, or `null` if there aren't any more
	 * in the stack.
     */
	GL_fbo popFramebuffer (bool updateScissor = false,
			bool resetTextures = false, bool setViewport = true);

    /**
     * Binds a shader program.
     *
     * If there was a different program already bound it will force a pipeline
	 * flush first.
     *
     * If the same program given to this method is already set as the current
	 * program, no change will take place and this method will return `false`.
     *
     * @since 0.0.0
     *
     * @param program - The program that needs to be bound.
     *
     * @return `true` if the given program was bound, otherwise `false`.
     */
	bool setProgram (GL_program program);

    /**
     * Rebinds whatever program `currentProgram` is set as, without changing
	 * anything, or flushing.
     *
     * @since 0.0.0
     */
	void resetProgram ();

    /**
     * Creates a texture from an image source.
     *
     * @since 0.0.0
     *
     * @param source The source of the texture.
     * @param scaleMode The scale mode to be used by the texture.
     * @param width The width of the texture.
     * @param height The height of the texture.
     *
     * @return The OpenGL Texture that was created, or `null` if it couldn't be
	 * created.
     */
	GL_texture createTextureFromSource (Entity source, double width = -1,
			double height = -1, GLenum scaleMode = GL_LINEAR);

    /**
     * A wrapper for creating a GL_texture. If no pixel data is passed it will
	 * create an empty texture.
     *
     * @since 0.0.0
     *
     * @param mipLevel Mip level of the texture.
     * @param minFilter Filtering of the texture.
     * @param magFilter Filtering of the texture.
     * @param wrapT Wrapping mode of the texture.
     * @param wrapS Wrapping mode of the texture.
     * @param format Which format does the texture use.
     * @param pixels Pixel data.
     * @param width Width of the texture in pixels.
     * @param height Height of the texture in pixels.
     * @param forceSize If `true` it will use the width and height passed to this
	 * method, regardless of the pixels dimension.
     * @param flipY Flip the texture vertically.
     *
     * @return The OpenGL Texture that was created.
     */
	GL_texture createTexture2D (int mipLevel, GLenum minFilter, GLenum magFilter,
			GLenum wrapT, GLenum wrapS, GLenum format, SDL_Surface* surface,
			int width = 1, int height = 1, bool forceSize = false);

    /**
     * Creates a OpenGL Framebuffer object and optionally binds a depth stencil
	 * render buffer.
     *
     * @since 0.0.0
     *
     * @param width If `addDepthStencilBuffer` is true, this controls the width of
	 * the depth stencil.
     * @param height If `addDepthStencilBuffer` is true, this controls the height
	 * of the depth stencil.
     * @param renderTexture The color texture where the color pixels are written.
     * @param addDepthStencilBuffer Create a Renderbuffer for the depth stencil?
     *
     * @return Raw Framebuffer
     */
	GL_fbo createFramebuffer (int width, int height,
			GL_texture renderTexture, bool addDepthStencilBuffer = false);

	/**
	 * Wrapper for creating a vertex array object.
	 *
	 * @since 0.0.0
	 *
	 * @return Raw vertex array object
	 */
	GL_vao createVertexArray ();

    /**
     * Wrapper for creating a vertex buffer.
     *
     * @since 0.0.0
     *
     * @param initialSize The size of the buffer to create
     * @param bufferUsage How the buffer is used. gl.DYNAMIC_DRAW, gl.STATIC_DRAW
	 * or gl.STREAM_DRAW
     *
     * @return Raw vertex buffer
     */
	GL_vbo createVertexBuffer (size_t initialSize, GLenum bufferUsage);

    /**
     * Wrapper for creating a vertex buffer.
     *
     * @since 0.0.0
     *
     * @param initialData The raw initial data of the buffer
     * @param bufferUsage How the buffer is used. gl.DYNAMIC_DRAW, gl.STATIC_DRAW or gl.STREAM_DRAW
     *
     * @return Raw vertex buffer
     */
	GL_vbo createVertexBuffer (std::vector<std::uint8_t> initialData,
			GLenum bufferUsage);

    /**
     * Wrapper for creating an index buffer.
     *
     * @since 0.0.0
     *
     * @param initialSize The size of the buffer to create
     * @param bufferUsage How the buffer is used. gl.DYNAMIC_DRAW, gl.STATIC_DRAW
	 * or gl.STREAM_DRAW
     *
     * @return Raw index buffer
     */
	GL_ibo createIndexBuffer (size_t initialSize, GLenum bufferUsage);

    /**
     * Wrapper for creating an index buffer.
     *
     * @since 0.0.0
     *
     * @param initialData The raw initial data of the buffer
     * @param bufferUsage How the buffer is used. gl.DYNAMIC_DRAW, gl.STATIC_DRAW
	 * or gl.STREAM_DRAW
     *
     * @return Raw index buffer
     */
	GL_ibo createIndexBuffer (std::vector<std::uint8_t> initialData,
			GLenum bufferUsage);

    /**
     * Calls `glDeleteTexture` on the given GL_texture and also optionally
     * resets the currently defined textures.
     *
     * @since 0.0.0
     *
     * @param texture The OpenGL Texture to be deleted.
     * @param reset Call the `resetTextures` method after deleting this texture?
     */
	void deleteTexture (GL_texture texture, bool reset = false);

    /**
     * Deletes a Framebuffer from the GL context.
     *
     * @since 0.0.0
     *
     * @param framebuffer The Framebuffer to be deleted.
     */
	void deleteFramebuffer (GL_fbo framebuffer);

    /**
     * Deletes a Program from the GL context.
     *
     * @since 0.0.0
     *
     * @param program The shader program to be deleted.
     */
	void deleteProgram (GL_program program);

    /**
     * Deletes a Buffer from the GL context.
     *
     * @since 0.0.0
     *
     * @param buffer The WebGLBuffer to be deleted.
     */
	void deleteBuffer (GLuint buffer);

    /**
     * Controls the pre-render operations for the given camera.
     * Handles any clipping needed by the camera and renders the background color
	 * if a color is visible.
     *
     * @since 0.0.0
     *
     * @param camera The Camera to pre-render.
     */
	void preRenderCamera (Entity camera);

    /**
     * Return the current stencil mask.
     *
     * @since 0.0.0
     */
	Entity getCurrentStencilMask ();

    /**
     * Controls the post-render operations for the given camera.
     *
     * Renders the foreground camera effects like flash and fading, then resets
	 * the current scissor state.
     *
     * @since 0.0.0
     *
     * @param camera The Camera to post-render.
     */
	void postRenderCamera (Entity camera);

    /**
     * Clears the current vertex buffer and updates pipelines.
     *
     * @since 0.0.0
     */
	void preRender ();

    /**
     * The core render step for a Scene Camera.
     *
     * Iterates through the given array of Game Objects and renders them with the
	 * given Camera.
     *
     * This is called by the `CameraManager::render` method. The Camera Manager
	 * instance belongs to a Scene, and is invoked by the Scene Systems.render
	 * method.
     *
     * This method is not called if `Camera.visible` is `false`, or `Camera.alpha`
	 * is zero.
     *
     * @since 0.0.0
     *
     * @param children An array of filtered Game Objects that can be rendered by
	 * the given Camera.
     * @param camera The Scene Camera to render with.
     */
	void render (std::vector<Entity> children, Entity camera);

    /**
     * The post-render step happens after all Cameras in all Scenes have been
	 * rendered.
     *
     * @since 0.0.0
     */
	void postRender ();

	/**
	 * Schedules a snapshot of the entire game window to be taken after the
	 * current frame is rendered.
	 *
	 * To capture a specific area see the snapshotArea method. To capture a
	 * specific pixel, see snapshotPixel.
	 *
	 * Only one snapshot can be active _per frame_. If you have already called
	 * snapshotPixel, for example, then calling this method will override it.
	 *
	 * @since 0.0.0
	 *
	 * @param callback The function to invoke once the snapshot is created.
	 * @param path The file path to save the image to. Leave empty to not save any
	 * file, just an SDL_Surface.
	 */
	void snapshot (std::string path = "",
			std::function<void(SDL_Surface*)> callback = nullptr);

	/**
	 * Schedules a snapshot of the given area of the game viewport to be taken
	 * after the current frame is rendered.
	 *
	 * To capture the whole game viewport see the snapshot method. To capture a
	 * specific pixel, see snapshotPixel.
	 *
	 * Only one snapshot can be active _per frame_. If you have already called
	 * snapshotPixel, for example, then calling this method will override it.
	 *
	 * @since 0.0.0
	 *
	 * @param x The x coordinate to grab from.
	 * @param y The y coordinate to grab from.
	 * @param width The width of the area to grab.
	 * @param height The height of the area to grab.
	 * @param callback The function to invoke once the snapshot is created.
	 * @param path The file path to save the image to. Leave empty to not save any
	 * file, just an SDL_Surface.
	 */
	void snapshotArea (int x, int y, int width, int height,
			std::string path = "",
			std::function<void(SDL_Surface*)> callback = nullptr);

	/**
	 * Schedules a snapshot of the given pixel from the game viewport to be taken
	 * after the current frame is rendered.
	 *
	 * To capture the whole game viewport see the `snapshot` method. To capture a
	 * specific area, see `snapshotArea`.
	 *
	 * Only one snapshot can be active _per frame_. If you have already called
	 * snapshotArea, for example, then
	 * calling this method will override it.
	 *
	 * Unlike the other two snapshot methods, this one will return a Color
	 * object containing the color data for the requested pixel.
	 *
	 * @since 0.0.0
	 *
	 * @param x The x coordinate of the pixel to get.
	 * @param y The y coordinate of the pixel to get.
	 * @param callback_ The function to invoke once the snapshot is created.
	 */
	void snapshotPixel (int x, int y, std::function<void(Color)>& callback);

	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param x The x coordinate of the pixel to get.
	 * @param y The y coordinate of the pixel to get.
	 * @param callback The function to invoke once the snapshot is created.
	 */
	void snapshotPixel (int x, int y, std::function<void(Color)>&& callback);
	
	/**
	 * Takes a snapshot of the given area of the given framebuffer.
	 *
	 * Unlike the other snapshot methods, this one is processed immediately and
	 * doesn't wait the the next render.
	 *
	 * Snapshots work by using the OpenGL `glReadPixels` feature to grab every
	 * pixel from the framebuffer into an array.
	 * It then parses this, copying the contents to a temporary surface and finally
	 * returned to the provided callback. All in all, this is a computationally
	 * expensive and blocking process, which gets expensive the larger the widow
	 * gets, so use this carefully.
	 *
	 * @since 0.0.0
	 * @param framebuffer The framebuffer to grab from.
	 * @param bufferWidth The width of the framebuffer.
	 * @param bufferHeight The height of the framebuffer.
	 * @param callback The function to invoke after the snapshot surface is
	 * created.
	 * @param getPixel Grab a single pixel as a Color object, or an area as a
	 * surface?
	 * @param x The x coordinate to grab from.
	 * @param y The y coordinate to grab from.
	 * @param width The width of the area to grab.
	 * @param height The height of the area to grab.
	 * @param path The path to save the image in.
	 */
	void snapshotFramebuffer (GL_fbo framebuffer, int bufferWidth,
			int bufferHeight, std::function<void(SDL_Surface*)>&& callback,
			bool getPixel = false, int x = 0, int y = 0, int width = -1,
			int height = -1, std::string path = "");

	/**
	 * Takes a snapshot of an area from the current frame.
	 *
	 * @since 0.0.0
	 */
	void takeSnapshot ();

	/**
	 * Saves the snapshot surface in an image file.
	 *
	 * @since 0.0.0
	 */
	void saveSnapshot ();

    /**
     * Sets the minification and magnification filter for a texture.
     *
     * @since 0.0.0
     *
     * @param texture The texture to set the filter for.
     * @param filter The filter to set. 0 for linear filtering, 1 for nearest
	 * neighbor (blocky) filtering.
     */
	void setTextureFilter (GL_texture texture, int filter = 0);

    /**
     * Returns the largest texture size (either width or height) that can be
	 * created.
     * Note that VRAM may not allow a texture of any given size, it just expresses
     * hardware / driver support for a given size.
     *
     * @since 0.0.0
     *
     * @return The maximum supported texture size.
     */
	size_t getMaxTextureSize ();

	/**
	 * The local configuration settings of this Renderer.
	 *
	 * @since 0.0.0
	 */
	RenderConfig config;

	/**
	 * An instance of the Pipeline Manager class, that handles all Pipelines.
	 *
	 * Use this to manage all of your interactions with pipelines, such as adding,
	 * getting, setting and rendering them.
	 *
	 * The Pipeline Manager class is created in the `init` method and then
	 * populated with pipelines during the `boot` method.
	 *
	 * @since 0.0.0
	 */
	PipelineManager pipelines;

	/**
	 * The width of the canvas being rendered to.
	 * This is populated in the onResize event handler.
	 *
	 * @since 0.0.0
	 */
	double width = 0;

	/**
	 * The height of the canvas being rendered to.
	 * This is populated in the onResize event handler.
	 *
	 * @since 0.0.0
	 */
	double height = 0;

	/**
	 * An array of blend modes supported by the Renderer.
	 *
	 * This array includes the default blend modes as well as any custom blend
	 * modes added through addBlendMode.
	 *
	 * @since 0.0.0
	 */
	std::vector<BlendMode> blendModes;

	/**
	 * Cached value for the last texture unit that was used.
	 *
	 * This is basically the total number of texture units used -1. When this
	 * gets to the max number of texture set in the config object, a flush is
	 * triggered and it is set back to 0.
	 *
	 * @since 0.0.0
	 */
	int currentActiveTexture = 0;

	/**
	 * Contains the current starting active texture unit.
	 * This value is constantly updated and should be treated as read-only by your
	 * code.
	 *
	 * @since 0.0.0
	 */
	int startActiveTexture = 0;

	/**
	 * The maximum number of textures the GPU can handle. The minimum under the
	 * OpenGL 3 spec is 16.
	 * This is set via the Game Config `maxTextures` property and should never be
	 * changed after boot.
	 *
	 * @since 0.0.0
	 */
	int maxTextures = 16;

	/**
	 * An array of the available OpenGL texture units, used to populate the
	 * uSampler uniforms.
	 *
	 * This vector is populated during the init phase and should never be changed
	 * after boot.
	 *
	 * @since 0.0.0
	 */
	std::vector<GLenum> textureIndexes;

	/**
	 * An array of default temporary OpenGL Textures.
	 *
	 * This array is populated during the init phase and should never be changed
	 * after boot.
	 *
	 * @since 0.0.0
	 */
	std::vector<GL_texture> tempTextures;

	/**
	 * The currently bound texture at texture unit zero, if any.
	 *
	 * @since 0.0.0
	 */
	GL_texture textureZero;

	/**
	 * The currently bound normal map texture at texture unit one, if any.
	 *
	 * @since 0.0.0
	 */
	GL_texture normalTexture;

	/**
	 * The currently bound framebuffer in use.
	 *
	 * @since 0.0.0
	 */
	GL_fbo currentFramebuffer;

	/**
	 * A stack into which the frame buffer objects are pushed and popped.
	 *
	 * @since 0.0.0
	 */
	std::vector<GL_fbo> fboStack;

	/**
	 * Current Program in use.
	 *
	 * @since 0.0.0
	 */
	GL_program currentProgram = 0;

	/**
	 * Current blend mode in use
	 *
	 * @since 0.0.0
	 */
	int currentBlendMode;

	/**
	 * Indicates if the the scissor state is enabled.
	 *
	 * @since 0.0.0
	 */
	bool currentScissorEnabled = false;

	/**
	 * Stores the current scissor data: {x, y, width, height}
	 *
	 * @since 0.0.0
	 */
	std::array<int, 4> currentScissor {-1, -1, -1, -1};

	/**
	 * Stack of scissor data.
	 *
	 * @since 0.0.0
	 */
	std::vector<std::array<int, 4>> scissorStack;

	/**
	 * Currently bound VAO.
	 *
	 * @since 0.0.0
	 */
	GL_vao currentVertexArray = 0;

	/**
	 * A blank 32x32 transparent texture, as used by the Graphics system where
	 * needed.
	 * This is set in the `boot` method.
	 *
	 * @since 0.0.0
	 */
	GL_texture blankTexture = 0;

	/**
	 * A pure white 4x4 texture, as used by the Graphics system where needed.
	 * This is set in the `boot` method.
	 *
	 * @since 0.0.0
	 */
	GL_texture whiteTexture = 0;

	/**
	 * The total number of masks currently stacked.
	 *
	 * @since 0.0.0
	 */
	size_t maskCount = 0;

	/**
	 * The mask stack.
	 *
	 * @since 0.0.0
	 */
	std::vector<Mask_> maskStack;

	/**
	 * Internal property that tracks the currently set mask.
	 *
	 * @since 0.0.0
	 */
	Mask_ currentMask;

	/**
	 * Internal property that tracks the currently set camera mask.
	 *
	 * @since 0.0.0
	 */
	Mask_ currentCameraMask;

	/**
	 * The `type` of the Game Object being currently rendered.
	 * This can be used by advanced render functions for batching look-ahead.
	 *
	 * @since 0.0.0
	 */
	std::string currentType;

	/**
	 * Is the `type` of the Game Object being currently rendered different than the
	 * type of the object before it in the display list? I.e. it's a 'new' type.
	 *
	 * @since 0.0.0
	 */
	bool newType = false;

	/**
	 * Does the `type` of the next Game Object in the display list match that
	 * of the object being currently rendered?
	 *
	 * @since 0.0.0
	 */
	bool nextTypeMatch = false;

	/**
	 * Is the Game Object being currently rendered the final one in the list?
	 *
	 * @since 0.0.0
	 */
	bool finalType = false;

	/**
	 * The mipmap magFilter to be used when creating textures.
	 *
	 * You can specify this as a string in the game config, i.e.:
	 *
	 * `.renderer = { .mipmapFilter = 'NEAREST_MIPMAP_LINEAR' }`
	 *
	 * The 6 options for OpenGL are, in order from least to most
	 * computationally expensive:
	 *
	 * NEAREST (for pixel art)
	 * LINEAR (the default)
	 * NEAREST_MIPMAP_NEAREST
	 * LINEAR_MIPMAP_NEAREST
	 * NEAREST_MIPMAP_LINEAR
	 * LINEAR_MIPMAP_LINEAR
	 *
	 * Mipmaps only work with textures that are fully power-of-two in size.
	 *
	 * @since 0.0.0
	 */
	GLenum mipmapFilter = GL_LINEAR;

	/**
	 * The number of times the renderer had to flush this frame, due to running
	 * out of texture units.
	 *
	 * @since 0.0.0
	 */
	size_t textureFlush = 0;

	/**
	 * Are the OpenGL Textures in their default state?
	 *
	 * Used to avoid constant glBinds.
	 *
	 * @since 0.0.0
	 */
	bool isTextureClean = false;

	/**
	 * The default scissor, set during `preRender` and modified during `resize`.
	 *
	 * @since 0.0.0
	 */
	std::vector<float> defaultScissor {0, 0, 0, 0};

	/**
	 * Has this renderer fully booted yet?
	 *
	 * @since 0.0.0
	 */
	bool isBooted = false;

	/**
	 * A Render Target you can use to capture the current state of the Renderer.
	 *
	 * A Render Target encapsulates a framebuffer and texture for the OpenGL
	 * Renderer.
	 *
	 * @since 0.0.0
	 */
	RenderTarget renderTarget;

	/**
	 * The global game Projection matrix, used by shaders as 'uProjectionMatrix'
	 * uniform.
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
	 * Stores informations about the different OpenGL textures.
	 *
	 * @since 0.0.0
	 */
	std::map<GL_texture, TextureInfo_> textureInfo;

	/**
	 * Stores informations about the different OpenGL framebuffer objects.
	 *
	 * @since 0.0.0
	 */
	std::map<GL_fbo, FramebufferInfo_> framebufferInfo;

	/**
	 * Details about the currently scheduled snapshot.
	 *
	 * If `active` is set to `true` this object, a snapshot of the canvas will be
	 * taken after the current frame is fully rendered.
	 *
	 * @since 0.0.0
	 */
	SnapshotState snapshotState;
};

}	// namespace Zen

#endif
