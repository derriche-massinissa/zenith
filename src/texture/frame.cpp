/**
 * @file		frame.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "frame.h"

namespace Zen {
namespace Textures {
{
Frame::Frame (Texture& texture, std::string name, int sourceIndex, int x, int y, int width, int height)
	: texture_(texture)
	  , name_(name)
	  , sourceIndex_(sourceIndex)
	  , x_(x)
	  , y_(y)
	  , width_(width)
	  , height_(height)
	  , source_(texture.source_[sourceIndex])
{
	setSize(width, height, x, y);
}

Frame& Frame::setSize (int width, int height, int x = 0, int y = 0)
{
	cutX_ = x;
	cutY_ = y;
	cutWidth_ = width;
	cutHeight_ = height;

	width_ = width;
	height_ = height;

	halfWidth_ = width * 0.5;
	halfHeight_ = height * 0.5;

	centerX_ = width / 2.0;
	centerY_ = height / 2.0;

	data_.cut.x = x;
	data_.cut.y = y;
	data_.cut.w = width;
	data_.cut.h = height;
	data_.cut.r = x + width;
	data_.cut.b = y + height;

	data.sourceSize.w = width;
	data.sourceSize.h = height;

	data.radius = 0.5 * std::sqrt(width * width + height * height);

	data_.drawImage.x = x;
	data_.drawImage.y = y;
	data_.drawImage.width = width;
	data_.drawImage.height = height;

	return updateUVs();
}

Frame& Frame::setTrim (int actualWidth, int actualHeight, int destX, int destY, int destWidth, int destHeight)
{
	//  Store actual values

	data_.trim = true;

	data_.sourceSize.w = actualWidth;
	data_.sourceSize.h = actualHeight;

	data_.spriteSourceSize.x = destX;
	data_.spriteSourceSize.y = destY;
	data_.spriteSourceSize.w = destWidth;
	data_.spriteSourceSize.h = destHeight;
	data_.spriteSourceSize.r = destX + destWidth;
	data_.spriteSourceSize.b = destY + destHeight;

	//  Adjust properties
	x_ = destX;
	y_ = destY;

	width_ = destWidth;
	height_ = destHeight;

	halfWidth_ = destWidth * 0.5;
	halfHeight_ = destHeight * 0.5;

	centerX_ = destWidth / 2.0;
	centerY_ = destHeight / 2.0;

	return updateUVs();
}

CropData Frame::setCropUVs (CropData crop, int x, int y, int width, int height, bool flipX, bool flipY)
{
	//  Clamp the input values

	int cx = cutX_;
	int cy = cutY_;
	int cw = cutWidth_;
	int ch = cutHeight_;
	int rw = realWidth_;
	int rh = realHeight_;

	x = Math::clamp(x, 0, rw);
	y = Math::clamp(y, 0, rh);

	width = Math::clamp(width, 0, rw - x);
	height = Math::clamp(height, 0, rh - y);

	int ox = cx + x;
	int oy = cy + y;
	int ow = width;
	int oh = height;

	if (data_.trim)
	{
		auto& ss = data_.spriteSourceSize;

		// Need to check for intersection between the cut area and the
		// crop area.
		// If there is none, we set UV to be empty, otherwise set it to be
		// the intersection area.

		width = Math::clamp(width, 0, cw - x);
		height = Math::clamp(height, 0, ch - y);

		int cropRight = x + width;
		int cropBottom = y + height;

		bool intersects = !(ss.r < x || ss.b < y || ss.x > cropRight || ss.y > cropBottom);

		if (intersects) {
			int ix = std::max(ss.x, x);
			int iy = std::max(ss.y, y);
			int iw = std::min(ss.r, cropRight) - ix;
			int ih = std::min(ss.b, cropBottom) - iy;

			ow = iw;
			oh = ih;

			if (flipX) {
				ox = cx + (cw - (ix - ss.x) - iw);
			} else {
				ox = cx + (ix - ss.x);
			}

			if (flipY) {
				oy = cy + (ch - (iy - ss.y) - ih);
			} else {
				oy = cy + (iy - ss.y);
			}

			x = ix;
			y = iy;

			width = iw;
			height = ih;
		} else {
			ox = 0;
			oy = 0;
			ow = 0;
			oh = 0;
		}
	} else {
		if (flipX) {
			ox = cx + (cw - x - width);
		}

		if (flipY)
		{
			oy = cy + (ch - y - height);
		}
	}

	int tw = source.width;
	int th = source.height;

	//  Map the given coordinates into UV space, clamping to the 0-1 range.

	crop.u0 = std::max(0, ox / tw);
	crop.v0 = std::max(0, oy / th);
	crop.u1 = std::min(1, (ox + ow) / tw);
	crop.v1 = std::min(1, (oy + oh) / th);

	crop.x = x;
	crop.y = y;

	crop.cx = ox;
	crop.cy = oy;
	crop.cw = ow;
	crop.ch = oh;

	crop.width = width;
	crop.height = height;

	crop.flipX = flipX;
	crop.flipY = flipY;

	return crop;
}

CropData Frame::updateCropUVs (CropData crop, bool flipX, bool flipY)
{
	return setCropUVs(crop, crop.x, crop.y, crop.width, crop.height, flipX, flipY);
}

Frame& Frame::setUVs (int width, int height, double u0, double v0, double u1, double v1)
{
	//  Canvas data

	auto& cd = data_.drawImage;

	cd.width = width;
	cd.height = height;

	//  WebGL data
      
	u0_ = u0;
	v0_ = v0;

	u1_ = u1;
	v1_ = v1;

	return *this;
}

Frame& Frame::updateUVs ()
{
	int cx = cutX_;
	int cy = cutY_;
	int cw = cutWidth_;
	int ch = cutHeight_;

	//  Canvas data

	auto& cd = data_.drawImage;

	cd.width = cw;
	cd.height = ch;

	//  WebGL data

	int tw = source.width;
	int th = source.height;

	u0_ = cx / tw;
	v0_ = cy / th;

	u1_ = (cx + cw) / tw;
	v1_ = (cy + ch) / th;

	return *this;
}

Frame& Frame::updateUVsInverted ()
{
	int tw = source.width;
	int th = source.height;

	u0_ = (cutX_ + cutHeight_) / tw;
	v0_ = cutY_ / th;

	u1_ = cutX_ / tw;
	v1_ = (cutY_ + cutWidth_) / th;

	return *this;
}

Frame Frame::clone()
{
	return *this;
}

int Frame::getRealWidth ()
{
	return data_.sourceSize.w;
}

int Frame::getRealHeight ()
{
	return data_.sourceSize.h;
}

double Frame::getRadius ()
{
	return data_.radius;
}

bool Frame::isTrimmed ()
{
	return data_.trim;
}

FrameDataDrawImage Frame::getDrawImageData ()
{
	return data_.drawImage;
}

}	// namespace Textures
} 	// namespace Zen
