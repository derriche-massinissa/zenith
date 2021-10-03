/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_COMPONENTS_MASK_HPP
#define ZEN_COMPONENTS_MASK_HPP

#include "../ecs/entity.hpp"
#include "../renderer/types/gl_types.hpp"

namespace Zen {
namespace Components {

struct Mask
{
	//////////
	// Bitmask
	//////////
	Entity bitmapMask = entt::null; // renderable

	/**
	 * The texture used for the masks framebuffer.
	 *
	 * @since 0.0.0
	 */
	GL_texture maskTexture = 0;

	/**
	 * The texture used for the main framebuffer.
	 *
	 * @since 0.0.0
	 */
	GL_texture mainTexture = 0;

	/**
	 * Whether the Bitmap Mask is dirty and needs to be updated.
	 *
	 * @since 0.0.0
	 */
	bool dirty = true;

	/**
	 * The framebuffer to which a masked Game Object is rendered.
	 *
	 * @since 0.0.0
	 */
	GL_fbo mainFramebuffer = 0;

	/**
	 * The framebuffer to which the Bitmap Mask's masking Game Object is rendered.
	 *
	 * @since 0.0.0
	 */
	GL_fbo maskFramebuffer = 0;

	/**
	 * Whether to invert the masks alpha.
	 *
	 * If `true`, the alpha of the masking pixel will be inverted before it's
	 * multiplied with the masked pixel.
	 * Essentially, this means that a masked area will be visible only if the
	 * corresponding area in the mask is invisible.
	 *
	 * @since 0.0.0
	 */
	bool invertAlpha = false;

	/**
	 * Is this mask a stencil mask?
	 *
	 * @since 0.0.0
	 */
	bool isStencil = false;

	///////////
	// Geometry
	///////////
	/**
	 * The Graphics object which describes the Geometry Mask.
	 *
	 * @since 0.0.0
	 */
	Entity geometryMask = entt::null;

	/**
	 * The current stencil level.
	 *
	 * @since 0.0.0
	 */
	int level = 0;

	///////////
	// Misc
	///////////
	/**
	 * Is this mask a bitmap mask? Otherwise it is a geometry mask.
	 * @since 0.0.0
	 */
	bool isBitmap = true;
};

}	// namespace Components
}	// namespace Zen

#endif
