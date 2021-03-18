/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_STRUCTS_SIZE_H
#define ZEN_STRUCTS_SIZE_H

#include "../const.h"
#include "../math/math.h"
#include "cmath"

namespace Zen {
namespace Structs {

class Size
{
public:
	Size (unsigned int width_ = 0, unsigned int height_ = 0, SCALE_MODE aspectMode_ = SCALE_MODE::RESIZE);

	Size& setAspectMode (SCALE_MODE value_ = SCALE_MODE::RESIZE);

	Size& setSnap (int snapWidth_ = 0, int snapHeight_ = 0);

	Size& setMin (int width_ = 0, int height_ = 0);

	Size& setMax (int width_ = 0, int height_ = 0);

	Size& setSize (int width_ = 0, int height_ = 0);

	Size& setAspectRatio (double ratio_);

	Size& resize (int width_, int height_);

	int getNewWidth (int value_);

	int getNewHeight (int value_);

	Size& constrain (int width_ = 0, int height_ = 0, bool fit_ = true);

	Size& fitTo (int width_, int height_);

	Size& envelop (int width_, int height_);

	Size& setWidth (int width_);

	Size& setHeight (int height_);

	int getWidth ();

	int getHeight ();

private:
	unsigned int width = 0;

	unsigned int height = 0;

	SCALE_MODE aspectMode = SCALE_MODE::RESIZE;

	double aspectRatio = 1.0;

	unsigned int minWidth = 0;

	unsigned int minHeight = 0;

	unsigned int maxWidth = 0;

	unsigned int maxHeight = 0;

	Math::Vector2 snapTo {0, 0};
};

}	// namespace Structs
}	// namespace Zen

#endif
