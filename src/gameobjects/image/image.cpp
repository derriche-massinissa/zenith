/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "image.h"
#include "../../scene/scene.h"

namespace Zen {
namespace GameObjects {

Image::Image (Scene* scene_, int x_, int y_, std::string texture_, std::string frame_)
	: GameObject (scene_)
{
	setTexture(texture_, frame_);
	setPosition(x_, y_);
	setSizeToFrame();
	setOriginFromFrame();
}

bool Image::hasComponent (int compMask_)
{
	return compMask_ | COMPONENT_MASK;
}

}	// namespace GameObjects
}	// namespace Zen
