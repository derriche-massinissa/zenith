/**
 * @file		size.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "size.h"

namespace Zen {
namespace Structs {

Size::Size (unsigned int width, unsigned int height, double aspectMode)
{
	width_ = width;

	if (height == 0)
		height_ = width;

	aspectMode_ = aspectMode;

	aspectRatio_ = (height_ == 0) ? 1 : (1.0*width_ / height_);
}

Size& Size::setAspectMode (SCALE_MODE value)
{
	aspectMode_ = value;

	return setSize(width_, height_);
}

Size& Size::setSnap (int snapWidth, int snapHeight)
{
	snapTo_.set(setWidth, snapHeight);

	return setSize(width_, height_);
}

Size& Size::setMin (int width, int height)
{
	if (height == 0)
		height = width;

	minWidth_ = Math::clamp(width, 0, maxWidth_);
	minHeight_ = Math::clamp(height, 0, maxHeight_);

	return setSize(width_, height_);
}

Size& Size::setMax (int width, int height)
{
	if (height == 0)
		height = width;

	maxWidth_ = std::max(width, minWidth_);
	maxHeight_ = std::max(height, minHeight_);

	return setSize(width_, height_);
}

Size& Size::setSize (int width, int height)
{
	if (height == 0)
		height = width;

	switch (aspectMode_)
	{
		case SCALE_MODE::RESIZE:
			width_ = getNewWidth(Math::snapFloor(width, snapTo_.x));
			height_ = getNewHeight(Math::snapFloor(height, snapTo_.y));
			aspectRatio_ = (height_ == 0) ? 1.0 : (1.0*width_ / height_);
			break;

		case SCALE_MODE::WIDTH_CONTROLS_HEIGHT:
			height_ = getNewHeight((width_ / width) * height_);
			width_ = getNewWidth(Math::snapFloor(width, snapTo_.x));
			aspectRatio_ = (height_ == 0) ? 1.0 : (1.0*width_ / height_);
			break;

		case SCALE_MODE::HEIGHT_CONTROLS_WIDTH:
			width_ = getNewHeight((height_ / height) * width_);
			height_ = getNewWidth(Math::snapFloor(height, snapTo_.y));
			aspectRatio_ = (height_ == 0) ? 1.0 : (1.0*width_ / height_);
			break;

		case SCALE_MODE::FIT:
			constrain(width, height, true);
			break;

		case SCALE_MODE::ENVELOP:
			constrain(width, height, false);
			break;
	}

	return *this;
}

Size& Size::setAspectRatio (double ratio)
{
	aspectRatio_ = ratio;

	return setSize(width_, height_);
}

Size& Size::resize (int width, int height)
{
	width_ = getNewWidth(snapFloor(width, snapTo_.x));
	height_ = getNewHeight(snapFloor(height, snapTo_.y));
	aspectRatio_ = (height_ == 0) ? 1.0 : (1.0*width_ / height_);

	return *this;
}

int Size::getNewWidth (int value)
{
	return Math::clamp(value, minWidth_, maxWidth_);
}

int Size::getNewHeight (int value)
{
	return Math::clamp(value, minHeight_, maxHeight_);
}

Size& Size::constrain (int width, int height, bool fit)
{
	if (height == 0)
		height = width;

	width = getNewWidth(width);
	height = getNewWidth(height);

	double newRatio = (height == 0) ? 1.0 : (1.0*width / height);

	if ((fit && aspectRatio_ > newRatio) || (!fit && aspectRatio_ < newRatio))
	{
		// We need to change the height to fit the width

		width = snapFloor(width, snapTo_.x);

		height = width / aspectRatio_;

		if (snapTo_.y > 0)
		{
			height = snapFloor(height, snapTo_.y);

			// Reduce the width accordingly
			width = height * aspectRatio_;
		}
	}
	else if ((fit && aspectRatio_ < newRatio) || (!fit && aspectRatio_ > newRatio))
	{
		// We need to change the width to fit the height

		height = snapFloor(height, snapTo_.y);

		width = height * aspectRatio_;

		if (snapTo.x > 0)
		{
			width = snapFloor(width, setTo.x);

			// Reduce the height accordingly
			height = width * (1.0 / aspectRatio_);
		}
	}

	width_ = width;
	height_ = height;

	return *this;
}

Size& Size::fitTo (int width, int height)
{
	return constrain(width, height, true);
}

Size& Size::envelop (int width, int height)
{
	return constrain(width, height, false);
}

Size& Size::setWidth (int width)
{
	return setSize(width, height_);
}

Size& Size::setHeight (int height)
{
	return setSize(width_, height);
}

int Size::getWidth ()
{
	return width_;
}

int Size::getHeight ()
{
	return height_;
}

}	// namespace Structs
}	// namespace Zen
