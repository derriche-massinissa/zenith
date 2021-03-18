/**
 * @file		frame.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "frame.h"

#include "texture.h"
#include "texture_source.h"
#include "../geom/rectangle.h"
#include "../math/clamp.h"

namespace Zen {
namespace Textures {

Frame::Frame (Texture* texture_, std::string name_, int sourceIndex_, int x_, int y_, int width_, int height_)
    : name (name_)
    , sourceIndex (sourceIndex_)
    , x (x_)
    , y (y_)
    , width (width_)
    , height (height_)
    , source (&texture_->source[sourceIndex_])
	, texture (texture_)
{
	setSize(width_, height_, x_, y_);
}

Frame& Frame::setSize (int width_, int height_, int x_, int y_)
{
	cutX = x_;
	cutY = y_;
	cutWidth = width_;
	cutHeight = height_;

	width = width_;
	height = height_;

	halfWidth = width_ * 0.5;
	halfHeight = height_ * 0.5;

	centerX = width_ / 2.0;
	centerY = height_ / 2.0;

	data.cut.x = x_;
	data.cut.y = y_;
	data.cut.width = width_;
	data.cut.height = height_;
	data.cut.setRight(x_ + width_);
	data.cut.setBottom(y_ + height_);

	data.sourceSize.setWidth(width_);
	data.sourceSize.setHeight(height_);

	data.radius = 0.5 * std::sqrt(width_ * width_ + height_ * height_);

	data.drawImage.x = x_;
	data.drawImage.y = y_;
	data.drawImage.width = width_;
	data.drawImage.height = height_;

	return updateUVs();
}

Frame& Frame::setTrim (int actualWidth_, int actualHeight_, int destX_, int destY_, int destWidth_, int destHeight_)
{
	//  Store actual values

	data.trim = true;

	data.sourceSize.setWidth(actualWidth_);
	data.sourceSize.setHeight(actualHeight_);

	data.spriteSourceSize.x = destX_;
	data.spriteSourceSize.y = destY_;
	data.spriteSourceSize.width = destWidth_;
	data.spriteSourceSize.height = destHeight_;
	data.spriteSourceSize.setRight(destX_ + destWidth_);
	data.spriteSourceSize.setBottom(destY_ + destHeight_);

	//  Adjust properties
	x = destX_;
	y = destY_;

	width = destWidth_;
	height = destHeight_;

	halfWidth = destWidth_ * 0.5;
	halfHeight = destHeight_ * 0.5;

	centerX = destWidth_ / 2.0;
	centerY = destHeight_ / 2.0;

	return updateUVs();
}

CropData Frame::setCropUVs (CropData crop_, int x_, int y_, int width_, int height_, bool flipX_, bool flipY_)
{
	//  Clamp the input values

	int cx_ = cutX;
	int cy_ = cutY;
	int cw_ = cutWidth;
	int ch_ = cutHeight;
	int rw_ = getRealWidth();
	int rh_ = getRealHeight();

	x_ = Math::clamp(x_, 0, rw_);
	y_ = Math::clamp(y_, 0, rh_);

	width_ = Math::clamp(width_, 0, rw_ - x_);
	height_ = Math::clamp(height_, 0, rh_ - y_);

	int ox_ = cx_ + x_;
	int oy_ = cy_ + y_;
	int ow_ = width_;
	int oh_ = height_;

	if (data.trim)
	{
		auto& ss_ = data.spriteSourceSize;

		// Need to check for intersection between the cut area and the
		// crop area.
		// If there is none, we set UV to be empty, otherwise set it to be
		// the intersection area.

		width_ = Math::clamp(width_, 0, cw_ - x_);
		height_ = Math::clamp(height_, 0, ch_ - y_);

		int cropRight_ = x_ + width_;
		int cropBottom_ = y_ + height_;

		bool intersects_ = !(ss_.getRight() < x_ || ss_.getBottom() < y_ || ss_.x > cropRight_ || ss_.y > cropBottom_);

		if (intersects_) {
			int ix_ = std::max(ss_.x, x_);
			int iy_ = std::max(ss_.y, y_);
			int iw_ = std::min(ss_.getRight(), cropRight_) - ix_;
			int ih_ = std::min(ss_.getBottom(), cropBottom_) - iy_;

			ow_ = iw_;
			oh_ = ih_;

			if (flipX_) {
				ox_ = cx_ + (cw_ - (ix_ - ss_.x) - iw_);
			} else {
				ox_ = cx_ + (ix_ - ss_.x);
			}

			if (flipY_) {
				oy_ = cy_ + (ch_ - (iy_ - ss_.y) - ih_);
			} else {
				oy_ = cy_ + (iy_ - ss_.y);
			}

			x_ = ix_;
			y_ = iy_;

			width_ = iw_;
			height_ = ih_;
		} else {
			ox_ = 0;
			oy_ = 0;
			ow_ = 0;
			oh_ = 0;
		}
	} else {
		if (flipX_) {
			ox_ = cx_ + (cw_ - x_ - width_);
		}

		if (flipY_)
		{
			oy_ = cy_ + (ch_ - y_ - height_);
		}
	}

	int tw_ = source->width;
	int th_ = source->height;

	//  Map the given coordinates into UV space, clamping to the 0-1 range.

	crop_.u0 = std::max(0, ox_ / tw_);
	crop_.v0 = std::max(0, oy_ / th_);
	crop_.u1 = std::min(1, (ox_ + ow_) / tw_);
	crop_.v1 = std::min(1, (oy_ + oh_) / th_);

	crop_.x = x_;
	crop_.y = y_;

	crop_.cx = ox_;
	crop_.cy = oy_;
	crop_.cw = ow_;
	crop_.ch = oh_;

	crop_.width = width_;
	crop_.height = height_;

	crop_.flipX = flipX_;
	crop_.flipY = flipY_;

	return crop_;
}

CropData Frame::updateCropUVs (CropData crop_, bool flipX_, bool flipY_)
{
	return setCropUVs(crop_, crop_.x, crop_.y, crop_.width, crop_.height, flipX_, flipY_);
}

Frame& Frame::setUVs (int width_, int height_, double u0_, double v0_, double u1_, double v1_)
{
	//  Canvas data

	auto& cd_ = data.drawImage;

	cd_.width = width_;
	cd_.height = height_;

	//  WebGL data
      
	u0 = u0_;
	v0 = v0_;

	u1 = u1_;
	v1 = v1_;

	return *this;
}

Frame& Frame::updateUVs ()
{
	int cx_ = cutX;
	int cy_ = cutY;
	int cw_ = cutWidth;
	int ch_ = cutHeight;

	//  Canvas data

	auto& cd_ = data.drawImage;

	cd_.width = cw_;
	cd_.height = ch_;

	//  WebGL data

	int tw_ = source->width;
	int th_ = source->height;

	u0 = cx_ / tw_;
	v0 = cy_ / th_;

	u1 = (cx_ + cw_) / tw_;
	v1 = (cy_ + ch_) / th_;

	return *this;
}

Frame& Frame::updateUVsInverted ()
{
	int tw_ = source->width;
	int th_ = source->height;

	u0 = (cutX + cutHeight) / tw_;
	v0 = cutY / th_;

	u1 = cutX / tw_;
	v1 = (cutY + cutWidth) / th_;

	return *this;
}

Frame Frame::clone()
{
	return *this;
}

int Frame::getRealWidth ()
{
	return data.sourceSize.getWidth();
}

int Frame::getRealHeight ()
{
	return data.sourceSize.getHeight();
}

double Frame::getRadius ()
{
	return data.radius;
}

bool Frame::isTrimmed ()
{
	return data.trim;
}

Geom::Rectangle Frame::getDrawImageData ()
{
	return data.drawImage;
}

}	// namespace Textures
} 	// namespace Zen
