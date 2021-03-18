/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

namespace Zen {
namespace GameObjects {

Image::Image (Scene* scene, int x, int y, Texture* texture, Frame* frame)
{
	setTexture(texture, frame);
	setPosition(x, y);
	setSizeToFrame();
	setOriginFromFrame();
}

bool hasComponent (int compMask_)
{
	return compMask_ | COMPONENT_MASK;
}

}	// namespace GameObjects
}	// namespace Zen
