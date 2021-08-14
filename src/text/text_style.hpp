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
};

}	// namespace Zen

#endif
