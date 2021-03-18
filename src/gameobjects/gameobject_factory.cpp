/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../scene/scene.h"
#include "image/image.h"

#include <memory>

namespace Zen {
namespace GameObjects {

GameObjectFactory::GameObjectFactory (Scene* scene_)
	: scene (scene_)
{}

void GameObjectFactory::boot ()
{}

Image& GameObjectFactory::image (int x, int y, std::string key, std::string frame)
{
	return *static_cast<Image*> (
		scene->children.add(std::move( std::make_unique<Image>(scene, x, y, key, frame) ))
	);
}

}	//namespace GameObjects
}	//namespace Zen
