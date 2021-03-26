/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_CROP_DATA_HPP
#define ZEN_TEXTURES_CROP_DATA_HPP

namespace Zen {

/**
 * @struct CropData
 * @since 0.0.0
 */
struct CropData
{
	double u0 = 0.,
		   v0 = 0.,
		   u1 = 0.,
		   v1 = 0.;

	int x = 0,
		y = 0;

	int cx = 0,
		cy = 0,
		cw = 0,
		ch = 0;

	int width = 0,
		height = 0;

	bool flipX = false,
		 flipY = false;
};

}	// namespace Zen

#endif
