/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_COMPONENTS_RENDERABLE_HPP
#define ZEN_COMPONENTS_RENDERABLE_HPP

#include "../ecs/entity.hpp"
#include "transform_matrix.hpp"
#include "../renderer/pipeline.hpp"
#include "../renderer/pipelines/postfx_pipeline.hpp"
#include <vector>

namespace Zen {
namespace Components {

struct Renderable
{
	/**
	 * The render flags.
	 *
	 * @since 0.0.0
	 */
	int flags = 0b1111;

	/**
	 * The camera ignore filter.
	 *
	 * @since 0.0.0
	 */
	int filter = 0;

    /**
     * The initial pipeline of this Game Object.
     *
	 * If you call `resetPipeline` on this Game Object, the pipeline is reset
	 * to this default.
     *
     * @since 0.0.0
     */
    Pipeline *defaultPipeline = nullptr;

	/**
	 * A pointer to the pipeline instance responsible for rendering this entity.
	 *
	 * @since 0.0.0
	 */
	Pipeline *pipeline = nullptr;

    /**
     * The Post FX Pipelines this Game Object uses for post-render effects.
     *
     * The pipelines are processed in the order in which they appear in this array.
     *
     * @since 0.0.0
     */
	std::vector<std::unique_ptr<PostFXPipeline>> postPipelines;

	/**
	 * Renders the Game Object with the Renderer to the given Camera. The
	 * object will not render if any of its renderFlags are set or it is being
	 * actively filtered out by the Camera. This method should not be called
	 * directly. It is a utility function of the Render module.
	 *
	 * @since 0.0.0
	 *
	 * @param src The Game Object being rendered in this call.
	 * @param camera The Camera that is rendering the Game Object.
	 * @param parentMatrix This transform matrix is defined if the game object
	 * is nested
	 */
	void (*render) (Entity, Entity, TransformMatrix*);
};

}	// namespace Components
}	// namespace Zen

#endif
