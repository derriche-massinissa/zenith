/**
 * @file		frame_data.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef FRAME_DATA_h
#define FRAME_DATA_h

#include "../geom/rectangle.h"

namespace Zen {
namespace Textures {

struct FrameData
{
	Geom::Rectangle cut;
	bool trim;
	Geom::Rectangle spriteSourceSize;
	float radius;
	Geom::Rectangle drawImage;
};

}	// namespace Textures
}	// namespace Zen

#endif
