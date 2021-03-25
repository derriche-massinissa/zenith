/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "frame.hpp"

#include <cmath>
#include "entt/entt.hpp"
#include "../components/frame.hpp"
#include "../components/source.hpp"
#include "../../utils/assert.hpp"

namespace Zen {

extern entt::registry g_registry;

CreateFrame (Entity texture, const char* name, Entity source, int x, int y, int width, int height)
{
	auto frame = g_registry.create();
	auto& fc = g_registry.emplace<FrameComponent>(
			frame,
			FrameComponent{
				.name = name,
				.source = source,
				.x = x,
				.y = y,
				.width = width,
				.height = height,
				.halfWidth = width/2,
				.halfHeight = height/2,
				.centerX = width/2,
				.centerY = height/2
			);
}

void SetFrameSize (Entity entity, int width, int height, int x = 0, int y = 0)
{
	auto frame = g_registry.try_get<FrameComponent>(entity);
	ZEN_ASSERT(frame, "The entity has no 'Frame' component.");
	
	frame->cutX = x;
	frame->cutY = y;
	frame->cutWidth = width;
	frame->cutHeight = height;

	frame->width = width;
	frame->height = height;

	frame->halfWidth = width / 2.0;
	frame->halfHeight = height / 2.0;

	frame->centerX = width / 2.0;
	frame->centerY = height / 2.0;

	frame->data.cut.x = x;
	frame->data.cut.y = y;
	frame->data.cut.width = width;
	frame->data.cut.height = height;

	SetSize(frame->data.sourceSize, width, height);

	frame->data.radius = 0.5 * std::sqrt(width * width + height * height);

	frame->data.drawImage.x = x;
	frame->data.drawImage.y = y;
	frame->data.drawImage.width = width;
	frame->data.drawImage.height = height;
}

void SetFrameTrim (Entity frame, int actualWidth, int actualHeight, int destX, int destY, int destWidth, int destHeight)
{
	auto frame = g_registry.try_get<FrameComponent>(entity);
	ZEN_ASSERT(frame, "The entity has no 'Frame' component.");
	
	//  Store actual values

	frame->data.trim = true;

	frame->data.sourceSize.width = actualWidth;
	frame->data.sourceSize.height = actualHeight;

	frame->data.spriteSourceSize.x = destX;
	frame->data.spriteSourceSize.y = destY;
	frame->data.spriteSourceSize.width = destWidth;
	frame->data.spriteSourceSize.height = destHeight;

	//  Adjust properties
	frame->x = destX;
	frame->y = destY;

	frame->width = destWidth;
	frame->height = destHeight;

	frame->halfWidth = destWidth / 2.0;
	frame->halfHeight = destHeight / 2.0;

	frame->centerX = destWidth / 2.0;
	frame->centerY = destHeight / 2.0;
}

CropData SetFrameCropUVs (Entity frame, CropData crop, int x, int y, int width, int height, bool flipX, bool flipY)
{
	auto frame = g_registry.try_get<FrameComponent>(entity);
	ZEN_ASSERT(frame, "The entity has no 'Frame' component.");

	//  Clamp the input values

	int cx = frame->cutX;
	int cy = frame->cutY;
	int cw = frame->cutWidth;
	int ch = frame->cutHeight;
	int rw = frame->data.sourceSize.width;
	int rh = frame->data.sourceSize.height;

	x = Math::Clamp(x, 0, rw);
	y = Math::Clamp(y, 0, rh);

	width = Math::Clamp(width, 0, rw - x);
	height = Math::Clamp(height, 0, rh - y);

	int ox = cx + x;
	int oy = cy + y;
	int ow = width;
	int oh = height;

	if (frame->data.trim)
	{
		auto& ss = frame->data.spriteSourceSize;

		// Need to check for intersection between the cut area and the
		// crop area.
		// If there is none, we set UV to be empty, otherwise set it to be
		// the intersection area.

		width = Math::Clamp(width, 0, cw - x);
		height = Math::Clamp(height, 0, ch - y);

		int cropRight = x + width;
		int cropBottom = y + height;

		bool intersects = !((ss.x + ss.width) < x || (ss.y + ss.height) < y || ss.x > cropRight || ss.y > cropBottom);

		if (intersects)
		{
			int ix = std::max(ss.x, x);
			int iy = std::max(ss.y, y);
			int iw = std::min((ss.x + ss.width), cropRight) - ix;
			int ih = std::min((ss.y + ss.height), cropBottom) - iy;

			ow = iw;
			oh = ih;

			if (flipX)
			{
				ox = cx + (cw - (ix - ss.x) - iw);
			}
			else
			{
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
		}
		else
		{
			ox = 0;
			oy = 0;
			ow = 0;
			oh = 0;
		}
	}
	else
	{
		if (flipX)
		{
			ox = cx + (cw - x - width);
		}

		if (flipY)
		{
			oy = cy + (ch - y - height);
		}
	}

	int tw = source->width;
	int th = source->height;

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

CropData UpdateFrameCropUVs (Entity frame, CropData crop, bool flipX, bool flipY)
{
	return SetCropUVs(frame, crop, crop.x, crop.y, crop.width, crop.height, flipX, flipY);
}

void SetFrameUVs (Entity entity, int width, int height, double u0, double v0, double u1, double v1)
{
	auto frame = g_registry.try_get<FrameComponent>(entity);
	ZEN_ASSERT(frame, "The entity has no 'Frame' component.");

	//  Canvas data

	auto& cd = frame->data.drawImage;

	cd.width = width;
	cd.height = height;

	//  WebGL data

	frame->u0 = u0_;
	frame->v0 = v0_;

	frame->u1 = u1_;
	frame->v1 = v1_;
}

void UpdateFrameUVs (Entity entity)
{
	auto frame = g_registry.try_get<FrameComponent>(entity);
	ZEN_ASSERT(frame, "The entity has no 'Frame' component.");

	int cx = frame->cutX;
	int cy = frame->cutY;
	int cw = frame->cutWidth;
	int ch = frame->cutHeight;

	//  Canvas data

	auto& cd = data.drawImage;

	cd.width = cw;
	cd.height = ch;

	//  WebGL data

	auto source = g_registry.try_get<TextureSourceComponent>(frame->source);
	ZEN_ASSERT(source, "The entity has no 'TextureSource' component.");

	int tw = source->width;
	int th = source->height;

	u0 = cx / tw;
	v0 = cy / th;

	u1 = (cx + cw) / tw;
	v1 = (cy + ch) / th;
}

void UpdateFrameUVsInverted (Entity frame)
{
	auto frame = g_registry.try_get<FrameComponent>(entity);
	ZEN_ASSERT(frame, "The entity has no 'Frame' component.");

	auto source = g_registry.try_get<TextureSourceComponent>(frame->source);
	ZEN_ASSERT(source, "The entity has no 'TextureSource' component.");

	int tw = source->width;
	int th = source->height;

	frame->u0 = (frame->cutX + frame->cutHeight) / tw;
	frame->v0 = frame->cutY / th;

	frame->u1 = frame->cutX / tw;
	frame->v1 = (frame->cutY + frame->cutWidth) / th;
}

int GetFrameRealWidth (Entity frame)
{
	auto frame = g_registry.try_get<FrameComponent>(entity);
	ZEN_ASSERT(frame, "The entity has no 'Frame' component.");

	return frame->data.sourceSize.width;
}

int GetFrameRealHeight (Entity frame)
{
	auto frame = g_registry.try_get<FrameComponent>(entity);
	ZEN_ASSERT(frame, "The entity has no 'Frame' component.");

	return frame->data.sourceSize.height;
}

double GetFrameRadius (Entity frame)
{
	auto frame = g_registry.try_get<FrameComponent>(entity);
	ZEN_ASSERT(frame, "The entity has no 'Frame' component.");

	return frame->data.radius;
}

bool IsFrameTrimmed (Entity frame)
{
	auto frame = g_registry.try_get<FrameComponent>(entity);
	ZEN_ASSERT(frame, "The entity has no 'Frame' component.");

	return frame->data.trim;
}

Geom::Rectangle GetFrameDrawImageData (Entity frame)
{
	auto frame = g_registry.try_get<FrameComponent>(entity);
	ZEN_ASSERT(frame, "The entity has no 'Frame' component.");

	return frame->data.drawImage;
}

}	// namespace Zen
