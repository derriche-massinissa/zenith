/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "size.h"

namespace Zen {
namespace Structs {

Size::Size (unsigned int width_, unsigned int height_, double aspectMode_)
{
	width = width_;

	if (height_ == 0)
		height = width_;

	aspectMode = aspectMode_;

	aspectRatio = (height == 0) ? 1 : (1.f*width / height);
}

Size& Size::setAspectMode (SCALE_MODE value_)
{
	aspectMode = value_;

	return setSize(width, height);
}

Size& Size::setSnap (int snapWidth_, int snapHeight_)
{
	snapTo.set(setWidth_, snapHeight_);

	return setSize(width, height);
}

Size& Size::setMin (int width_, int height_)
{
	if (height_ == 0)
		height_ = width_;

	minWidth = Math::clamp(width_, 0, maxWidth);
	minHeight = Math::clamp(height_, 0, maxHeight);

	return setSize(width, height);
}

Size& Size::setMax (int width_, int height_)
{
	if (height_ == 0)
		height_ = width_;

	maxWidth = std::max(width_, minWidth);
	maxHeight = std::max(height_, minHeight);

	return setSize(width, height);
}

Size& Size::setSize (int width_, int height_)
{
	if (height_ == 0)
		height_ = width_;

	switch (aspectMode)
	{
		case SCALE_MODE::RESIZE:
			width = getNewWidth(Math::snapFloor(width_, snapTo.x));
			height = getNewHeight(Math::snapFloor(height_, snapTo.y));
			aspectRatio = (height == 0) ? 1.0 : (1.f*width / height);
			break;

		case SCALE_MODE::WIDTH_CONTROLS_HEIGHT:
			height = getNewHeight((width / width_) * height);
			width = getNewWidth(Math::snapFloor(width_, snapTo.x));
			aspectRatio = (height == 0) ? 1.0 : (1.f*width / height);
			break;

		case SCALE_MODE::HEIGHT_CONTROLS_WIDTH:
			width = getNewHeight((height / height_) * width);
			height = getNewWidth(Math::snapFloor(height_, snapTo.y));
			aspectRatio = (height == 0) ? 1.0 : (1.f*width / height);
			break;

		case SCALE_MODE::FIT:
			constrain(width_, height_, true);
			break;

		case SCALE_MODE::ENVELOP:
			constrain(width_, height_, false);
			break;
	}

	return *this;
}

Size& Size::setAspectRatio (double ratio_)
{
	aspectRatio = ratio_;

	return setSize(width, height);
}

Size& Size::resize (int width_, int height_)
{
	width = getNewWidth(snapFloor(width_, snapTo.x));
	height = getNewHeight(snapFloor(height_, snapTo.y));
	aspectRatio = (height == 0) ? 1.0 : (1.f*width / height);

	return *this;
}

int Size::getNewWidth (int value_)
{
	return Math::clamp(value_, minWidth, maxWidth);
}

int Size::getNewHeight (int value_)
{
	return Math::clamp(value_, minHeight, maxHeight);
}

Size& Size::constrain (int width_, int height_, bool fit_)
{
	if (height_ == 0)
		height_ = width_;

	width_ = getNewWidth(width_);
	height_ = getNewWidth(height_);

	double newRatio_ = (height_ == 0) ? 1.0 : (1.f*width_ / height_);

	if ((fit_ && aspectRatio > newRatio_) || (!fit_ && aspectRatio < newRatio_))
	{
		// We need to change the height to fit the width

		width_ = snapFloor(width_, snapTo.x);

		height_ = width_ / aspectRatio;

		if (snapTo.y > 0)
		{
			height_ = snapFloor(height_, snapTo.y);

			// Reduce the width accordingly
			width_ = height_ * aspectRatio;
		}
	}
	else if ((fit_ && aspectRatio < newRatio_) || (!fit_ && aspectRatio > newRatio_))
	{
		// We need to change the width to fit the height

		height_ = snapFloor(height_, snapTo.y);

		width_ = height_ * aspectRatio;

		if (snapTo.x > 0)
		{
			width_ = snapFloor(width_, setTo.x);

			// Reduce the height accordingly
			height_ = width_ * (1.f / aspectRatio);
		}
	}

	width = width_;
	height = height_;

	return *this;
}

Size& Size::fitTo (int width_, int height_)
{
	return constrain(width_, height_, true);
}

Size& Size::envelop (int width_, int height_)
{
	return constrain(width_, height_, false);
}

Size& Size::setWidth (int width_)
{
	return setSize(width_, height);
}

Size& Size::setHeight (int height_)
{
	return setSize(width, height_);
}

int Size::getWidth ()
{
	return width;
}

int Size::getHeight ()
{
	return height;
}

}	// namespace Structs
}	// namespace Zen
