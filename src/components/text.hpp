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

namespace Zen {
namespace Components {

struct Text
{
	/**
	 * The text content of this text object. As set by the user.
	 *
	 * @since 0.0.0
	 */
	std::string text;

	/**
	 * DO NOT EDIT, work with the `text` property instead.
	 *
	 * The text content of this text object. Formated and arranged for caching
	 * purposes for things like text wrapping and so on...
	 *
	 * @since 0.0.0
	 */
	std::string content;

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
