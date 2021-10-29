/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXT_STYLE_HPP
#define ZEN_TEXT_STYLE_HPP

#include <SDL2/SDL.h>
#include <string>
#include "const.hpp"

namespace Zen {

struct TextStyle
{
	/**
	 * The font family key, as stored in the TextManager.
	 *
	 * @since 0.0.0
	 */
	std::string fontFamily;

	/**
	 * The color of the text.
	 *
	 * @since 0.0.0
	 */
	int color = 0x000000;

	/**
	 * The size of the font to use, in pixels.
	 *
	 * @since 0.0.0
	 */
	int fontSize = 12;

	/**
	 * The style of the glyphs to use. Can be one or more of the following
	 * (Multiple by 'OR'ing them together):
	 * - NORMAL
	 * - BOLD
	 * - ITALIC
	 * - UNDERLINE
	 * - STRIKETHROUGH
	 *
	 * @since 0.0.0
	 */
	TEXT_DECORATION decoration = TEXT_DECORATION::NORMAL;

	/**
	 * The outline around each character.
	 *
	 * @since 0.0.0
	 */
	int outline = 0;

	int kerning = 1;

	/**
	 * Custom line spacing for this text object. If left or set to -1, the
	 * default line spacing for the font will be used.
	 *
	 * @since 0.0.0
	 */
	double lineSpacing = -1;

	double paddingTop = 0,
		   paddingLeft = 0,
		   paddingBottom = 0,
		   paddingRight = 0;

	TEXT_ALIGNMENT alignment = TEXT_ALIGNMENT::LEFT;

	int backgroundColor = -1;

	int wrapWidth = 0;

	bool advancedWrap = false;

	/**
	 * The base size of this text object. If set to other than -1, this text's
	 * font size will always be kept at the closest multiple of the baseSize.
	 *
	 * i.e. if set to 8 and size is 14, it gets set to 16.
	 *
	 * This is useful for pixel art fonts.
	 *
	 * @since 0.0.0
	 */
	int baseSize = -1;
};

}	// namespace Zen

#endif
