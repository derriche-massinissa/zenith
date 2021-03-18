/**
 * @file		bitmap_mask.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef MASK_H
#define MASK_H

#include "mask.h"

namespace Zen {
namespace Masks {

class BitmapMask : public Mask
{
public:
	BitmapMask (GameObjects:GameObject& renderable);

	GameObjects::GameObject *maskObject_ = nullptr;

	void preRender (
			Renderer *renderer,
			GameObjects::GameObject *maskedObject,
			Camera *camera);

	void postRender (
			Renderer *renderer,
			GameObjects::GameObject *maskedObject = nullptr,
			Camera *camera = nullptr);
};

}	// namespace Masks
}	// namespace Zen

#endif
