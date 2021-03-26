/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_FRAME_DATA_HPP
#define ZEN_TEXTURES_FRAME_DATA_HPP

#include "../geom/types/rectangle.hpp"
#include "../structs/types/size.hpp"

namespace Zen {

/*
	FrameData data = {
		{	// cut
			0,	// x
			0,	// y
			0,	// w
			0,	// h
			0,	// r
			0	// b
		},
		false,	// trim
		{	// sourceSize
			0,	// w
			0	// h
		},
		{	// spriteSourceSize
			0,	// x
			0,	// y
			0,	// w
			0,	// h
			0,	// r
			0	// b
		},
		0,	// radius
		{	// drawImage
			0,	// x
			0,	// y
			0,	// width
			0	// height
		}
		*/
struct FrameData
{
	Rectangle cut;

	bool trim = false;

	Size sourceSize;

	Rectangle spriteSourceSize;

	double radius = 0.0;

	Rectangle drawImage;
};

}	// namespace Zen

#endif
