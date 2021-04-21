/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_COMPONENTS_FRAME_HPP
#define ZEN_TEXTURES_COMPONENTS_FRAME_HPP

#include <string>
#include "../../ecs/entity.hpp"
#include "../frame_data.hpp"

namespace Zen {
namespace Components {

struct Frame
{
	/**
	 * The name of this Frame.
	 * The name is unique within the Texture.
	 *
	 * @since 0.0.0
	 */
	std::string name;

	/**
	 * The TextureSource this Frame is part of.
	 *
	 * @since 0.0.0
	 */
	Entity source = entt::null;

	/**
	 * The X rendering offset of this Frame, taking trim into account.
	 *
	 * @since 0.0.0
	 */
	int x = 0;

	/**
	 * The Y rendering offset of this Frame, taking trim into account.
	 *
	 * @since 0.0.0
	 */
	int y = 0;

	/**
	 * The rendering width of this Frame, taking trim into account.
	 *
	 * @since 0.0.0
	 */
	int width = 0;

	/**
	 * The rendering height of this Frame, taking trim into account.
	 *
	 * @since 0.0.0
	 */
	int height = 0;

	/**
	 * X position within the source image to cut from.
	 *
	 * @since 0.0.0
	 */
	int cutX = 0;

	/**
	 * Y position within the source image to cut from.
	 *
	 * @since 0.0.0
	 */
	int cutY = 0;

	/**
	 * The width of the area in the source image to cut.
	 *
	 * @since 0.0.0
	 */
	int cutWidth = 0;

	/**
	 * The height of the area in the source image to cut.
	 *
	 * @since 0.0.0
	 */
	int cutHeight = 0;

	/**
	 * Half the width, floored.
	 * Precalculated for the renderer.
	 *
	 * @since 0.0.0
	 */
	int halfWidth = 0;

	/**
	 * Half the height, floored.
	 * Precalculated for the renderer.
	 *
	 * @since 0.0.0
	 */
	int halfHeight = 0;

	/**
	 * The x center of this frame, floored.
	 *
	 * @since 0.0.0
	 */
	int centerX = 0;

	/**
	 * The y center of this frame, floored.
	 *
	 * @since 0.0.0
	 */
	int centerY = 0;

	/**
	 * The horizontal pivot point of this Frame.
	 *
	 * @since 0.0.0
	 */
	double pivotX = 0;

	/**
	 * The vertical pivot point of this Frame.
	 *
	 * @since 0.0.0
	 */
	double pivotY = 0;

	/**
	 * Does this Frame have a custom pivot point?
	 *
	 * @since 0.0.0
	 */
	bool customPivot = false;

	/**
	 * Is this frame rotated or not in the Texture?
	 * Rotation allows you to use rotated frames in texture atlas packing.
	 * It has nothing to do with Sprite rotation.
	 *
	 * @since 0.0.0
	 */
	bool rotated = false;

	/**
	 * OpenGL UV u0 value.
	 *
	 * @since 0.0.0
	 */
	double u0 = 0;

	/**
	 * OpenGL UV v0 value.
	 *
	 * @since 0.0.0
	 */
	double v0 = 0;

	/**
	 * OpenGL UV u1 value.
	 *
	 * @since 0.0.0
	 */
	double u1 = 0;

	/**
	 * OpenGL UV v1 value.
	 *
	 * @since 0.0.0
	 */
	double v1 = 0;

	/**
	 * The un-modified source frame, trim and UV data.
	 *
	 * @since 0.0.0
	 */
	FrameData data;
};

}	// namespace Components
}	// namespace Zen

#endif
