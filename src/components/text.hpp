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
#include <set>

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
	std::vector<std::uint32_t> content;

	/**
	 * Style configuration of the text to use when rendering it, such as its
	 * color, line spacing, kerning, font family...
	 *
	 * @since 0.0.0
	 */
	TextStyle style;

	/**
	 * Last display scale this text was scanned at.
	 * Used to update the glyph atlas when not in SCALE_MODE::RESIZE scale mode.
	 *
	 * @since 0.0.0
	 */
	double lastDisplayScale = 0;

	/**
	 * The scaled font size of this text object, updated automatically from the
	 * style.fontSize, and scaled accordingly with the scale mode.
	 *
	 * @since 0.0.0
	 */
	int fontSize = 12;

	/**
	 * The last font size value that was updated.
	 *
	 * @since 0.0.0
	 */
	int lastFontSize = 12;

	/**
	 * @since 0.0.0
	 */
	std::set<int> scannedSizes;
};

}	// namespace Components
}	// namespace Zen

#endif
