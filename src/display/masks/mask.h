/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_DISPLAY_MASK_H
#define ZEN_DISPLAY_MASK_H

namespace Zen {
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
			Camera& camera_)
	{}

	/**
	 * @since 0.0.0
	 */
	virtual void postRender (
			Renderer& renderer_,
			GameObjects::GameObject& maskedObject_,
			Camera& camera_)
	{}
};

}	// namespace Masks
}	// namespace Zen

#endif
