/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_GAMEOBJECT_H
#define ZEN_GAMEOBJECTS_GAMEOBJECT_H

#include "../cameras/2d/camera.fwd.h"

namespace Zen {
namespace GameObjects {

class GameObject
{
public:
	int x;
	int y;
	int width;
	int height;
	int cameraFilter = 0;
	bool isParent = false;
	GameObject *parentContainer = nullptr;

	int getComponentMask ()
	{
		return 0b00000;
	}

	double getScrollFactorX ()
	{
		return 1;
	}

	double getScrollFactorY ()
	{
		return 1;
	}

	double getOriginX ()
	{
		return 0.5;
	}

	double getOriginY ()
	{
		return 0.5;
	}

	bool willRender (Cameras::Scene2D::Camera& camera_)
	{
		return true;
	}
};

}	// namespace GameObjects
}	// namespace Zen

#endif
