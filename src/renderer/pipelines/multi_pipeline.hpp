/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_PIPELINES_MULTI_HPP
#define ZEN_RENDERER_PIPELINES_MULTI_HPP

#include "../pipeline.hpp"
#include "../../components/transform_matrix.hpp"

namespace Zen {

/**
 * The Multi Pipeline is the core 2D texture rendering pipeline.
 * Virtually all Game Objects use this pipeline by default, including Sprites,
 * Graphics and Tilemaps. It handles the batching of quads and tris, as well as
 * methods for drawing and batching geometry data.
 *
 * The fragment shader it uses can be found in `shaders/src/Multi.frag`.
 * The vertex shader it uses can be found in `shaders/src/Multi.vert`.
 *
 * The default shader attributes for this pipeline are:
 * - `inPosition` (vec2, offset 0)
 * - `inTexCoord` (vec2, offset 8)
 * - `inTexId` (float, offset 16)
 * - `inTintEffect` (float, offset 20)
 * - `inTint` (vec4, offset 24, normalized)
 *
 * The default shader uniforms for this pipeline are:
 *
 * `uProjectionMatrix` (mat4)
 * `uMainSampler` (sampler2D array)
 *
 * If you wish to create a custom pipeline extending from this one, you can use two
 * string declarations in your fragment shader source: `%count%` and `%forloop%`,
 * where `count` is used to set the number of `sampler2Ds` available, and `forloop`
 * is a block of GLSL code that will get the currently bound texture unit.
 *
 * This pipeline will automatically inject that code for you, should those values
 * exist in your shader source. If you wish to handle this yourself, you can also
 * use the function `Utils.parseFragmentShaderMaxTextures`.
 *
 * If you wish to create a pipeline that works from a single texture, or that
 * doesn't have internal texture iteration, please see the `SinglePipeline` instead.
 *
 * @since 0.0.0
 */
class MultiPipeline : public Pipeline
{
public:
	/**
	 * @since 0.0.0
	 *
	 * @param config The configuration options for this pipeline.
	 */
	MultiPipeline (PipelineConfig config);

    /**
     * Called every time the pipeline is bound by the renderer.
     * Sets the shader program, vertex buffer and other resources.
     * Should only be called when changing pipeline.
     *
     * @since 0.0.0
     */
    void boot ();

	/**
	 * @since 0.0.0
	 */
	PipelineConfig prepareConfig (PipelineConfig config);

    /**
     * Takes a Sprite Game Object, or any object that extends it, and adds it to
	 * the batch.
     *
     * @since 0.0.0
     *
     * @param gameObject The texture based Game Object to add to the batch.
     * @param camera The Camera to use for the rendering transform.
     * @param parentTransformMatrix The transform matrix of the parent container,
	 * if set.
     */
    void batchSprite (Entity gameObject, Entity camera,
			Components::TransformMatrix *parentTransformMatrix = nullptr);

    /**
     * Generic function for batching a textured quad using argument values instead
	 * of a Game Object.
     *
     * @since 0.0.0
     *
     * @param gameObject Source GameObject.
     * @param texture Raw WebGLTexture associated with the quad.
     * @param textureWidth Real texture width.
     * @param textureHeight Real texture height.
     * @param srcX X coordinate of the quad.
     * @param srcY Y coordinate of the quad.
     * @param srcWidth Width of the quad.
     * @param srcHeight Height of the quad.
     * @param scaleX X component of scale.
     * @param scaleY Y component of scale.
     * @param rotation Rotation of the quad.
     * @param flipX Indicates if the quad is horizontally flipped.
     * @param flipY Indicates if the quad is vertically flipped.
     * @param scrollFactorX By which factor is the quad affected by the camera
	 * horizontal scroll.
     * @param scrollFactorY By which factor is the quad effected by the camera
	 * vertical scroll.
     * @param displayOriginX Horizontal origin in pixels.
     * @param displayOriginY Vertical origin in pixels.
     * @param frameX X coordinate of the texture frame.
     * @param frameY Y coordinate of the texture frame.
     * @param frameWidth Width of the texture frame.
     * @param frameHeight Height of the texture frame.
     * @param tintTL Tint for top left.
     * @param tintTR Tint for top right.
     * @param tintBL Tint for bottom left.
     * @param tintBR Tint for bottom right.
     * @param tintEffect The tint effect.
     * @param uOffset Horizontal offset on texture coordinate.
     * @param vOffset Vertical offset on texture coordinate.
     * @param camera Current used camera.
     * @param parentTransformMatrix Parent container.
     * @param skipFlip Skip the renderTexture check.
     * @param textureUnit Use the currently bound texture unit?
     */
    void batchTexture (
        Entity gameObject,
        GL_texture texture,
        double textureWidth, double textureHeight,
        double srcX, double srcY,
        double srcWidth, double srcHeight,
        double scaleX, double scaleY,
        double rotation,
        bool flipX, bool flipY,
        double scrollFactorX, double scrollFactorY,
        double displayOriginX, double displayOriginY,
        double frameX, double frameY, double frameWidth, double frameHeight,
        int tintTL, int tintTR, int tintBL, int tintBR, int tintEffect,
        double uOffset, double vOffset,
        Entity camera,
        Components::TransformMatrix parentTransformMatrix,
        bool skipFlip = false,
        int textureUnit = -1
		);

    /**
     * Adds a Texture Frame into the batch for rendering.
     *
     * @since 0.0.0
     *
     * @param frame The Texture Frame to be rendered.
     * @param x The horizontal position to render the texture at.
     * @param y The vertical position to render the texture at.
     * @param tint The tint color.
     * @param alpha The alpha value.
     * @param transformMatrix The Transform Matrix to use for the texture.
     * @param parentTransformMatrix A parent Transform Matrix.
     */
    void batchTextureFrame (
        Entity frame,
        double x, double y,
        int tint, double alpha,
        Components::TransformMatrix transformMatrix,
        Components::TransformMatrix parentTransformMatrix
		);

private:
	/**
	 * A temporary Transform Matrix, re-used internally during batching.
	 *
	 * @since 0.0.0
	 */
	Components::TransformMatrix tempMatrix1;

	/**
	 * A temporary Transform Matrix, re-used internally during batching.
	 *
	 * @since 0.0.0
	 */
	Components::TransformMatrix tempMatrix2;

	/**
	 * A temporary Transform Matrix, re-used internally during batching.
	 *
	 * @since 0.0.0
	 */
	Components::TransformMatrix tempMatrix3;
};

}	// namespace Zen

#endif
