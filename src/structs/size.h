/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_STRUCTS_SIZE_H
#define ZEN_STRUCTS_SIZE_H

#include <cmath>
#include "../const.h"
#include "../math/vector2.h"

namespace Zen {
namespace Structs {

class Size
{
public:
	Size (unsigned int width_ = 0, unsigned int height_ = 0, SCALE_MODE aspectMode_ = SCALE_MODE::RESIZE);

	Size& setAspectMode (SCALE_MODE value_ = SCALE_MODE::RESIZE);

	Size& setSnap (unsigned int snapWidth_ = 0, unsigned int snapHeight_ = 0);

	Size& setMin (unsigned int width_ = 0, unsigned int height_ = 0);

	Size& setMax (unsigned int width_ = 0, unsigned int height_ = 0);

	Size& setSize (unsigned int width_ = 0, unsigned int height_ = 0);

	Size& setAspectRatio (double ratio_);

	Size& resize (unsigned int width_, unsigned int height_);

	unsigned int getNewWidth (unsigned int value_);

	unsigned int getNewHeight (unsigned int value_);

	Size& constrain (unsigned int width_ = 0, unsigned int height_ = 0, bool fit_ = true);

	Size& fitTo (unsigned int width_, unsigned int height_);

	Size& envelop (unsigned int width_, unsigned int height_);

	Size& setWidth (unsigned int width_);

	Size& setHeight (unsigned int height_);

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
