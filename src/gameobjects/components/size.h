/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_COMPONENTS_SIZE_H
#define ZEN_GAMEOBJECTS_COMPONENTS_SIZE_H

#include "../../texture/frame.h"

namespace Zen {
namespace GameObjects {
namespace Components {

template <typename T>
class Size
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

	T& setDisplayWidth (int value_)
	{
		scaleX = value_ / This()->frame.getRealWidth();

		return *This();
	}

	int getDisplayWidth ()
	{
		return std::abs(scaleX * This()->frame->getRealWidth());
	}

	T& setDisplayHeight (int value_)
	{
		scaleY = value_ / This()->frame.getRealHeight();

		return *This();
	}

	int getDisplayHeight ()
	{
		return std::abs(scaleY * This()->frame->getRealHeight());
	}

	T& setSizeToFrame (Textures::Frame* frame_ = nullptr)
	{
		if (!frame_)
			frame_ = This()->frame;

		width = frame_->getRealWidth();
		height = frame_->getRealHeight();

		return *This();
	}

	T& setSize (int width_, int height_)
	{
		width = width_;
		height = height_;

		return *This();
	}

	T& setDisplaySize (int width_, int height_)
	{
		setDisplayWidth(width_);
		setDisplayHeight(height_);

		return *This();
	}

	int width = 0;
	int height = 0;
	double scaleX = 1.0;
	double scaleY = 1.0;
};

}	// namespace Components
}	// namespace GameObjects
}	// namespace Zen

#endif
