/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_DISPLAY_BITMAP_MASK_H
#define ZEN_DISPLAY_BITMAP_MASK_H

#include "mask.h"

namespace Zen {
namespace Display {
namespace Masks {

/**
 * @since 0.0.0
 */
class BitmapMask : public Mask
{
public:
	/**
	 * @since 0.0.0
	 */
	BitmapMask (GameObjects::GameObject& renderable_);

	/**
	 * @since 0.0.0
	 */
	GameObjects::GameObject *maskObject = nullptr;

	/**
	 * @since 0.0.0
	 */
	void preRender (
			Renderer *renderer_,
			GameObjects::GameObject *maskedObject_,
			Cameras::Scene2D::Camera *camera_);

	/**
	 * @since 0.0.0
	 */
	void postRender (
			Renderer *renderer_,
			GameObjects::GameObject *maskedObject_ = nullptr,
			Cameras::Scene2D::Camera *camera_ = nullptr);
};

}	// namespace Masks
}	// namespace Display
}	// namespace Zen

#endif
