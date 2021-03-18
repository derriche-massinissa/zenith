/**
 * @file		mask.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef MASK_H
#define MASK_H

namespace Zen {
namespace Masks {

class Mask
{
public:
	virtual void preRender (
			Renderer& renderer,
			GameObjects::GameObject& maskedObject,
			Camera& camera)
	{}

	virtual void postRender (
			Renderer& renderer,
			GameObjects::GameObject& maskedObject,
			Camera& camera)
	{}
};

}	// namespace Masks
}	// namespace Zen

#endif
