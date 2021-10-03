/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_PIPELINES_BITMAP_MASK_HPP
#define ZEN_RENDERER_PIPELINES_BITMAP_MASK_HPP

#include "../pipeline.hpp"

namespace Zen {

/**
 * The Bitmap Mask Pipeline handles all of the bitmap mask rendering in OpenGL
 * for applying alpha masks to Game Objects. It works by sampling two texture
 * on the fragment shader and using the fragments alpha to clip the region.
 *
 * The fragment shader it uses can be found in `shaders/src/bitmap_mask.frag`.
 * The vertex shader it uses can be found in `shaders/src/bitmap_mask.vert`.
 *
 * The default shader attributes for this pipeline are:
 *
 * `aPosition` (vec2, offset 0)
 *
 * The default shader uniforms for this pipeline are:
 *
 * `uResolution` (vec2)
 * `uMainSampler` (sampler2D)
 * `uMaskSampler` (sampler2D)
 * `uInvertMaskAlpha` (bool)
 *
 * @class BitmapMaskPipeline
 * @since 0.0.0
 */
class BitmapMaskPipeline : public Pipeline
{
public:
	/**
	 * @since 0.0.0
	 *
	 * @param {Phaser.Types.Renderer.WebGL.WebGLPipelineConfig} config - The configuration options for this pipeline.
	 */
	BitmapMaskPipeline (PipelineConfig config);

	PipelineConfig prepareConfig (PipelineConfig config);

	void boot ();

    void resize (double width, double height);
    /**
     * Binds necessary resources and renders the mask to a separated framebuffer.
     * The framebuffer for the masked object is also bound for further use.
     *
     * @since 0.0.0
     *
     * @param mask GameObject used as mask.
     * @param maskedObject GameObject masked by the mask GameObject.
     * @param camera The camera rendering the current mask.
     */
    void beginMask (Entity mask, Entity maskedObject, Entity camera);

    /**
     * The masked game objects framebuffer is unbound and its texture
     * is bound together with the mask texture and the mask shader and
     * a draw call with a single quad is processed. Here is where the
     * masking effect is applied.
     *
     * @since 0.0.0
     *
     * @param mask GameObject used as a mask.
     * @param camera The camera rendering the current mask.
     */
    void endMask (Entity mask, Entity camera);
};

}	// namespace Zen

#endif
