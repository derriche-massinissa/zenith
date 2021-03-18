/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_DISPLAY_MASK_H
#define ZEN_DISPLAY_MASK_H

#include "../../renderer/renderer.fwd.h"
#include "../../gameobjects/gameobject.h"
#include "../../cameras/2d/camera.fwd.h"

namespace Zen {
namespace Display {
namespace Masks {

/**
 * @class Mask
 * @since 0.0.0
 */
class Mask
{
public:
	/**
	 * @since 0.0.0
	 */
	virtual void preRender (
			Renderer& renderer_,
			GameObjects::GameObject& maskedObject_,
			Cameras::Scene2D::Camera& camera_)
	{}

	/**
	 * @since 0.0.0
	 */
	virtual void postRender (
			Renderer& renderer_,
			GameObjects::GameObject& maskedObject_,
			Cameras::Scene2D::Camera& camera_)
	{}
};

}	// namespace Masks
}	// namespace Display
}	// namespace Zen

#endif
