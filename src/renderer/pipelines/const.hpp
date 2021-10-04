/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_PIPELINES_CONST_HPP
#define ZEN_RENDERER_PIPELINES_CONST_HPP

#include <string>

namespace Zen {
namespace Pipelines {

/**
 * The Bitmap Mask Pipeline.
 *
 * @since 0.0.0
 */
const std::string BITMAPMASK_PIPELINE = "BitmapMaskPipeline";

/**
 * The Light 2D Pipeline.
 *
 * @since 0.0.0
 */
const std::string LIGHT_PIPELINE = "Light2D";

/**
 * The Point Light Pipeline.
 *
 * @since 0.0.0
 */
const std::string POINTLIGHT_PIPELINE = "PointLightPipeline";

/**
 * The Single Texture Pipeline.
 *
 * @since 0.0.0
 */
const std::string SINGLE_PIPELINE = "SinglePipeline";

/**
 * The Multi Texture Pipeline.
 *
 * @since 0.0.0
 */
const std::string MULTI_PIPELINE = "MultiPipeline";

/**
 * The Rope Pipeline.
 *
 * @since 0.0.0
 */
const std::string ROPE_PIPELINE = "RopePipeline";

/**
 * The Graphics and Shapes Pipeline.
 *
 * @since 0.0.0
 */
const std::string GRAPHICS_PIPELINE = "GraphicsPipeline";

/**
 * The Post FX Pipeline.
 *
 * @since 0.0.0
 */
const std::string POSTFX_PIPELINE = "PostFXPipeline";

/**
 * The Utility Pipeline.
 *
 * @since 0.0.0
 */
const std::string UTILITY_PIPELINE = "UtilityPipeline";

}	// namespace Pipelines
}	// namespace Zen

#endif
