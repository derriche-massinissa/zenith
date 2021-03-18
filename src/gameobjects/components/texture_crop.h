/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_COMPONENT_TEXTURE_CROP_H
#define ZEN_GAMEOBJECTS_COMPONENT_TEXTURE_CROP_H

#include <string>
#define FLAG 8 // 1000

#include "../../geom/rectangle.h"

namespace Zen {
namespace GameObjects {
namespace Components {

template <typename T>
class Tint
{
public:
	/**
	 * Returns a cast `this` pointer, with the type of the descendant class.
	 *
	 * @since 0.0.0
	 * @return Pointer to this instance of T.
	 */
	T* This ()
	{
		return static_cast<T*>(this);
	}

	T& setCrop (int x_ = -1, int y_ = -1, int width_ = -1, int height_ = -1)
	{
		if (x < 0)
		{
			isCropped = false;
		}
		else if (frame)
		{
			frame->setCropUVs(This()->crop, x_, y_, width_, height_, This()->flipX, This()->flipY);

			isCropped = true;
		}

		return *This();
	}

	T& setCrop (Geom::Rectangle rect_)
	{
		if (frame)
		{
			frame->setCropUVs(This()->crop, rect_.x_, rect_.y_, rect_.width_, rect_.height_, This()->flipX, This()->flipY);

			isCropped = true;
		}
	}

	T& setTexture (std::string key, std::string frame)
	{
		texture = This()->scene->textures.get(key);

		return setFrame(frame);
	}

	T& setFrame (std::string frame_, bool updateSize_ = true, bool updateOrigin_ = true)
	{
		frame = texture.get(frame_);

		if (!frame.cutWidth || !frame.cutHeight)
			This()->renderFlags &= ~FLAG;
		else
			This()->renderFlags |= FLAG;

		if constexpr (This()->hasComponent(COMPONENT_MASK_SIZE))
		{
			if (updateSize_)
				This()->setSizeToFrame();
		}

		if constexpr (This()->hasComponent(COMPONENT_MASK_ORIGIN))
		{
			if (updateSize_)
			{
				if (frame.customPivot)
				{
					This()->setOrigin(frame.pivotX, frame.pivotY);
				}
				else
				{
					This()->updateDisplayOrigin();
				}
			}
		}

		if (isCropped)
			frame.updateCropUVs(This()->crop, flipX, flipY);

		return *This();
	}

	T& resetCropObject ()
	{
		return {
			// u0, v0, u1, v1
			0, 0, 0, 0,
				// x, y
				0, 0,
				// cx, cy, cw, ch
				0, 0, 0, 0,
				// width, height
				0, 0,
				// flipX, flipY
				false, false
		};
	}

	Textures::Texture* texture = nullptr;
	Textures::Frame* frame = nullptr;
	bool isCropped = false;
};

}	// namespace Components
}	// namespace Textures
}	// namespace Zen

#undef FLAG

#endif
