/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_GAMEOBJECT_H
#define ZEN_GAMEOBJECTS_GAMEOBJECT_H

#include "../cameras/2d/camera.fwd.h"
#include "../texture/frame.h"
#include "../texture/crop_data.h"
#include "components/transform_matrix.h"

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

	double getScaleX ()
	{
		return 1.0;
	}

	double getScaleY ()
	{
		return 1.0;
	}

	bool willRender (Cameras::Scene2D::Camera& camera_)
	{
		return true;
	}

	double getAlpha ()
	{
		return 1.0;
	}

	int getDisplayOriginX ()
	{
		return 50;
	}

	int getDisplayOriginY ()
	{
		return 50;
	}

	GameObject *mask = nullptr;

	bool isCropped = false;
	bool flipX = false;
	bool flipY = false;
	double rotation = 0.0;

	Textures::CropData crop;

	double scrollFactorX = 1.0;
	double scrollFactorY = 1.0;

	Textures::Frame *frame = nullptr;

	Components::TransformMatrix *parentMatrix_ = nullptr;
};

}	// namespace GameObjects
}	// namespace Zen

#endif
