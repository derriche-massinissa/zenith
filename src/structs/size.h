/**
 * @file		size.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef SIZE_H
#define SIZE_H

#include "../const.h"
#include "../math/math.h"
#include "cmath"

namespace Zen {
namespace Structs {

class Size
{
public:
	Size (unsigned int width = 0, unsigned int height = 0, double aspectMode = SCALE_MODE::NONE);

	Size& setAspectMode (SCALE_MODE value = ASPECT_MODE::NONE);

	Size& setSnap (int snapWidth = 0, int snapHeight = 0);

	Size& setMin (int width = 0, int height = 0);

	Size& setMax (int width = 0, int height = 0);

	Size& setSize (int width = 0, int height = 0);

	Size& setAspectRatio (double ratio);

	Size& resize (int width, int height);

	int getNewWidth (int value);

	int getNewHeight (int value);

	Size& constrain (int width = 0, int height = 0, bool fit = true);

	Size& fitTo (int width, int height);

	Size& envelop (int width, int height);

	Size& setWidth (int width);

	Size& setHeight (int height);

	int getWidth ();

	int getHeight ();

private:
	unsigned int width_ = 0;

	unsigned int height_ = 0;

	ASPECT_MODE aspectMode_ = ASPECT_MODE::NONE;

	double aspectRatio_ = 1.0;

	unsigned int minWidth_ = 0;

	unsigned int minHeight_ = 0;

	unsigned int maxWidth_ = 0;

	unsigned int maxHeight_ = 0;

	Math::Vector2 snapTo_ {0, 0};
};

}	// namespace Structs
}	// namespace Zen

#endif
