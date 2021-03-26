/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "size.hpp"

#include "../math/clamp.hpp"
#include "../math/snap/snap_floor.hpp"

#include <limits>
#include <algorithm>

namespace Zen {

Size CreateSize (double width, double height, SCALE_MODE aspectMode)
{
	width = std::max(width, 0.);
	height = std::max(height, 0.);

	Size size;

	size.maxWidth = std::numeric_limits<unsigned int>::max();
	size.maxHeight = std::numeric_limits<unsigned int>::max();

	size.width = width;

	if (height == 0)
		size.height = width;
	else
		size.height = height;

	size.aspectMode = aspectMode;

	size.aspectRatio = (size.height == 0) ? 1.0 : (size.width / size.height);

	return size;
}

void SetAspectMode (Size *size, SCALE_MODE value)
{
	size->aspectMode = value;

	SetSize(size, size->width, size->height);
}

void SetSnap (Size *size, double snapWidth, double snapHeight)
{
	size->snapTo[0] = snapWidth;
	size->snapTo[1] = snapHeight;

	SetSize(size, size->width, size->height);
}

void SetMin (Size *size, double width, double height)
{
	if (height == 0)
		height = width;

	size->minWidth = Math::Clamp(width, 0, size->maxWidth);
	size->minHeight = Math::Clamp(height, 0, size->maxHeight);

	SetSize(size, size->width, size->height);
}

void SetMax (Size *size, double width, double height)
{
	if (height == 0)
		height = width;

	size->maxWidth = std::max(width, size->minWidth);
	size->maxHeight = std::max(height, size->minHeight);

	SetSize(size, size->width, size->height);
}

void SetSize (Size *size, double width, double height)
{
	if (height == 0)
		height = width;

	switch (size->aspectMode)
	{
		case SCALE_MODE::RESIZE:
			size->width = GetNewWidth(*size, Math::SnapFloor(width, size->snapTo[0]));
			size->height = GetNewHeight(*size, Math::SnapFloor(height, size->snapTo[1]));
			size->aspectRatio = (size->height == 0) ? 1.0 : (size->width / size->height);
			break;

		case SCALE_MODE::WIDTH_CONTROLS_HEIGHT:
			size->width = GetNewWidth(*size, Math::SnapFloor(width, size->snapTo[0]));
			size->height = GetNewHeight(*size, size->width * (1.0 / size->aspectRatio));
			//width = getNewWidth(Math::Snap::floor(width_, snapTo.x));
			//height = getNewHeight(((double)width_ / (double)width) * (double)height);
			//aspectRatio = (height == 0) ? 1.0 : ((double)width / (double)height);
			break;

		case SCALE_MODE::HEIGHT_CONTROLS_WIDTH:
			size->height = GetNewHeight(*size, Math::SnapFloor(height, size->snapTo[1]));
			size->width = GetNewWidth(*size, size->height * size->aspectRatio);
			//height = getNewHeight(Math::Snap::floor(height_, snapTo.y));
			//width = getNewWidth(((double)height_ / (double)height) * (double)width);
			//aspectRatio = (height == 0) ? 1.0 : ((double)width / (double)height);
			break;

		case SCALE_MODE::FIT:
			Constrain(size, width, height, true);
			break;

		case SCALE_MODE::ENVELOP:
			Constrain(size, width, height, false);
			break;
	}
}

void SetAspectRatio (Size *size, double ratio)
{
	size->aspectRatio = ratio;

	SetSize(size, size->width, size->height);
}

void Resize (Size *size, double width, double height)
{
	size->width = GetNewWidth(*size, Math::SnapFloor(width, size->snapTo[0]));

	size->height = GetNewHeight(*size, Math::SnapFloor(height, size->snapTo[1]));

	size->aspectRatio = (size->height == 0) ? 1.0 : (size->width / size->height);
}

double GetNewWidth (Size size, double value)
{
	return Math::Clamp(value, size.minWidth, size.maxWidth);
}

double GetNewHeight (Size size, double value)
{
	return Math::Clamp(value, size.minHeight, size.maxHeight);
}

void Constrain (Size *size, double width, double height, bool fit)
{
	if (height == 0)
		height = width;

	width = GetNewWidth(*size, width);
	height = GetNewWidth(*size, height);

	double newRatio = (height == 0) ? 1.0 : (width / height);

	if ((fit && size->aspectRatio > newRatio) || (!fit && size->aspectRatio < newRatio))
	{
		// We need to change the height to fit the width

		width = Math::SnapFloor(width, size->snapTo[0]);

		height = width / size->aspectRatio;

		if (size->snapTo[1] > 0)
		{
			height = Math::SnapFloor(height, size->snapTo[1]);

			// Reduce the width accordingly
			width = height * size->aspectRatio;
		}
	}
	else if ((fit && size->aspectRatio < newRatio) || (!fit && size->aspectRatio > newRatio))
	{
		// We need to change the width to fit the height

		height = Math::SnapFloor(height, size->snapTo[1]);

		width = height * size->aspectRatio;

		if (size->snapTo[0] > 0)
		{
			width = Math::SnapFloor(width, size->snapTo[0]);

			// Reduce the height accordingly
			height = width * (1.0 / size->aspectRatio);
		}
	}

	size->width = width;
	size->height = height;
}

void FitTo (Size *size, double width, double height)
{
	Constrain(size, width, height, true);
}

void Envelop (Size *size, double width, double height)
{
	Constrain(size, width, height, false);
}

void SetWidth (Size *size, double width)
{
	SetSize(size, width, size->height);
}

void SetHeight (Size *size, double height)
{
	SetSize(size, size->width, height);
}

}	// namespace Zen
