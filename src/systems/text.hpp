/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_TEXT_HPP
#define ZEN_SYSTEMS_TEXT_HPP

#include "../ecs/entity.hpp"
#include <string>
#include "../text/text_style.hpp"

namespace Zen {

bool IsText (Entity entity);

/**
 */
void SetText (Entity text, std::string content);

void SetTextStyle (Entity text, TextStyle style);

void SetFontFamily(Entity text, std::string fontFamily);

void SetTextColor (Entity text, int color);

void SetFontSize (Entity text, int size);

/**
 * Can be 'OR'ed together.
 *
 * @since 0.0.0
 */
void SetTextDecoration (Entity text, TEXT_DECORATION decoration);

void SetTextOutline (Entity text, int outlineWidth);

void SetLineSpacing (Entity text, int spacing);

void SetTextPadding (Entity text, int padding);
void SetTextPaddingTop (Entity text, int padding);
void SetTextPaddingBottom (Entity text, int padding);
void SetTextPaddingLeft (Entity text, int padding);
void SetTextPaddingRight (Entity text, int padding);

void SetTextWrap (Entity text, bool wrap);

/**
 * @since 0.0.0
 *
 * @param text The text entity to modify.
 * @param bound The maximum width of this text object in pixels. 0 is unbounded.
 */
void SetTextHorizontalBound (Entity text, int bound = 0);

void SetTextAlign (Entity text, TEXT_ALIGNMENT alignment);

/**
 * @since 0.0.0
 *
 * @param text The text entity to modify.
 * @param color The background color to set. -1 in no background.
 */
void SetTextBackgroundColor (Entity text, int color = -1);

// Getters

/**
 * @since 0.0.0
 *
 * @param text The text entity to get the value from.
 */
std::string GetText(Entity text);

}

#endif
