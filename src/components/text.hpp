/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_COMPONENTS_TEXT_HPP
#define ZEN_COMPONENTS_TEXT_HPP

#include "../ecs/entity.hpp"
#include <string>
#include <SDL2/SDL_ttf.h>

namespace Zen {
namespace Components {

struct Text
{
	std::string text;

	std::string font;

	int size = 12;

	int color = 0xffffff;

	int style = TTF_STYLE_NORMAL;

	int outline = 0;

	int hinting = TTF_HINTING_NORMAL;

	int kerning = 1;

	/**
	 * The render mode of this text.
	 * 0. Solid: Indexed, dirty, fast render, fast blit.
	 * 1. Shaded: Indexed, nice, slow render, fast blit, __no transparency__.
	 * 2. Blended: ARGB, very nice, slow render, slow blit.
	 */
	int mode = 2;

	double lineSpacing = 0;

	double paddingTop, paddingLeft, paddingBottom, paddingRight;

	bool wrap = false;

	double horizontalBound = 0;
};

}	// namespace Components
}	// namespace Zen

#endif
