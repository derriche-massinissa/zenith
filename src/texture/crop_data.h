/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_CROP_DATA_H
#define ZEN_TEXTURES_CROP_DATA_H

namespace Zen {
namespace Textures {

/**
 * @struct CropData
 * @since 0.0.0
 */
struct CropData
{
	double u0 = 0;
	double v0 = 0;
	double u1 = 0;
	double v1 = 0;

	int x = 0;
	int y = 0;

	int cx = 0;
	int cy = 0;
	int cw = 0;
	int ch = 0;

	int width = 0;
	int height = 0;

	bool flipX = false;
	bool flipY = false;
};

}	// namespace Textures
}	// namespace Zen

#endif
