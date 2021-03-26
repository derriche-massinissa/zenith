/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_DISPLAY_TYPES_COLOR_HPP
#define ZEN_DISPLAY_TYPES_COLOR_HPP

#include <cstdint>

namespace Zen {

/**
 * Holds a single color value and allows for easy modification and reading of it.
 *
 * @struct Color
 * @since 0.0.0
 */
struct Color{
	/**
	 * The value of this color in hexadecimal, not including the alpha channel.
	 *
	 * @since 0.0.0
	 */
	uint32_t hex = 0x000000;

	/**
	 * The value of this color in hexadecimal, including the alpha channel.
	 *
	 * @since 0.0.0
	 */
	uint32_t hex32 = 0x00000000;

	/**
	 * The red color value, in a range of 0-255.
	 *
	 * @since 0.0.0
	 */
	uint8_t red = 0;

	/**
	 * The green color value, in a range of 0-255.
	 *
	 * @since 0.0.0
	 */
	uint8_t green = 0;

	/**
	 * The blue color value, in a range of 0-255.
	 *
	 * @since 0.0.0
	 */
	uint8_t blue = 0;

	/**
	 * The alpha color value, in a range of 0-255.
	 *
	 * @since 0.0.0
	 */
	uint8_t alpha = 255;

	/**
	 * The hue color value, in a range of 0-1.
	 *
	 * @since 0.0.0
	 */
	double hue = 0.;

	/**
	 * The saturation color value, in a range of 0-1.
	 *
	 * @since 0.0.0
	 */
	double saturation = 0.;

	/**
	 * The lightness color value, in a range of 0-1.
	 *
	 * @since 0.0.0
	 */
	double lightness = 0.;

	/**
	 * An array containing the calculated color values for OpenGL use.
	 *
	 * @since 0.0.0
	 */
	double gl[4] {0., 0., 0., 1.};
};

}	// namespace Zen

#endif
