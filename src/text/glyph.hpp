/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXT_GLYPH_HPP
#define ZEN_TEXT_GLYPH_HPP

namespace Zen {

struct Glyph
{
	int width = 0,
		height = 0,
		bearingX = 0,
		bearingY = 0,
		advanceX = 0,
		advanceY = 0;

	// Position of the glyph in the atlas texture
	int cacheX = 0,
		cacheY = 0,
		cacheW = 0,
		cacheH = 0;
};

}	// namespace Zen

#endif
