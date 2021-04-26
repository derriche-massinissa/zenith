/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_GAMEOBJECTFACTORY_H
#define ZEN_GAMEOBJECTS_GAMEOBJECTFACTORY_H

#include <string>
#include "../scene/scene.fwd.hpp"
#include "../ecs/entity.hpp"
//#include "image/image.fwd.h"

namespace Zen {

class GameObjectFactory
{
public:
	GameObjectFactory (Scene* scene_);

	Scene* scene;

	void boot ();

	Entity image (double x, double y, std::string key, std::string frame = "");
};

}	//namespace Zen

#endif
