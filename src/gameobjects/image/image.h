/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_IMAGE_H
#define ZEN_GAMEOBJECTS_IMAGE_H

#include <string>
#include "../../defs.h"
#include "../../texture/crop_data.h"
#include "../../scene/scene.fwd.h"
#include "../gameobject.h"

namespace Zen {
namespace GameObjects {

class Image
	: public GameObject
{
public:
	/**
	 * @since 0.0.0
	 */
	Image (Scene* scene_, int x_, int y_, std::string texture_, std::string frame_);

	static const int COMPONENT_MASK =
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

	static bool hasComponent (int compMask_);

	Textures::CropData crop;
};

}	// namespace GameObjects
}	// namespace Zen

#endif
