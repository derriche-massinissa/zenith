/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_IMAGE_h
#define ZEN_GAMEOBJECTS_IMAGE_h

#include "../components/alpha.h"
#include "../components/blend_mode.h"
#include "../components/depth.h"
#include "../components/get_bounds.h"
#include "../components/mask.h"
#include "../components/origin.h"
#include "../components/scroll_factor.h"
#include "../components/size.h"
#include "../components/texture_crop.h"
#include "../components/tint.h"
#include "../components/transform.h"
#include "../components/visible.h"

namespace Zen {
namespace GameObjects {

class Image
	: public GameObject
	, public Components::Alpha<Image>
	, public Components::BlendMode<Image>
	, public Components::Depth<Image>
	, public Components::Flip<Image>
	, public Components::GetBounds<Image>
	, public Components::Mask<Image>
	, public Components::Origin<Image>
	, public Components::ScrollFactor<Image>
	, public Components::Size<Image>
	, public Components::TextureCrop<Image>
	, public Components::Tint<Image>
	, public Components::Transform<Image>
	, public Components::Visible<Image>
{
public:
	const int COMPONENT_MASK =
		COMPONENT_MASK_ALPHA			|
		COMPONENT_MASK_BLENDMODE		|
		COMPONENT_MASK_DEPTH			|
		COMPONENT_MASK_FLIP				|
		COMPONENT_MASK_BOUNDS			|
		COMPONENT_MASK_MASK				|
		COMPONENT_MASK_ORIGIN			|
		COMPONENT_MASK_SCROLLFACTOR		|
		COMPONENT_MASK_SIZE				|
		COMPONENT_MASK_TEXTURECROP		|
		COMPONENT_MASK_TINT				|
		COMPONENT_MASK_TRANSFORM		|
		COMPONENT_MASK_VISIBLE			;

	bool hasComponent (int compMask_);

	Image (Scene* scene, int x, int y, Texture* texture, Frame* frame);

	Textures::CropData crop;
};

}	// namespace GameObjects
}	// namespace Zen

#endif
