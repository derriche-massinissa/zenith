/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_COMPONENTS_TEXT_HPP
#define ZEN_COMPONENTS_TEXT_HPP

#include "../ecs/entity.hpp"
#include "../text/text_style.hpp"
#include <string>
#include <SDL2/SDL_ttf.h>

namespace Zen {
namespace Components {

struct Text
{
	/**
	 * The text content of this text object.
	 *
	 * @since 0.0.0
	 */
	std::string text;

	/**
	 * Style configuration of the text to use when rendering it, such as its
	 * color, line spacing, kerning, font family...
	 *
	 * @since 0.0.0
	 */
	TextStyle style;
};

}	// namespace Components
}	// namespace Zen

#endif
