/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_DISPLAY_TYPES_COLORMATRIX_HPP
#define ZEN_DISPLAY_TYPES_COLORMATRIX_HPP

#include <array>

namespace Zen {

/**
 * The ColorMatrix class creates a 5x4 matrix that can be used in shaders and
 * graphics operations. It provides methods required to modify the color values,
 * such as adjusting the brightness, setting a sepia tone, hue rotation and more.
 *
 * @struct ColorMatrix
 * @since 0.0.0
 */
struct ColorMatrix {
	/**
	 * Internal array.
	 *
	 * @since 0.0.0
	 */
	std::array<double, 20> matrix = {
		1, 0, 0, 0, 0,
		0, 1, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 1, 0
	};

	/**
	 * The value that determines how much of the original color is used
	 * when mixing the colors. A value between 0 (all original) and 1 (all final)
	 *
	 * @since 0.0.0
	 */
	double alpha = 1.;

	/**
	 * Is the ColorMatrix array dirty?
	 *
	 * @since 0.0.0
	 */
	bool dirty = true;

	/**
	 * The matrix data 
	 *
	 * @since 0.0.0
	 */
	std::array<double, 20> data = {
		1, 0, 0, 0, 0,
		0, 1, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 1, 0
	};
};

}	// namespace Zen

#endif
