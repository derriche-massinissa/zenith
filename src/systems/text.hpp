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

/**
 * Set the maximum width of a text entity before it's content gets wrapped. Set
 * to 0 or less to disable text wrapping.
 *
 * @since 0.0.0
 *
 * @param text The text entity to modify.
 * @param width The wrap width.
 */
void SetTextWrapWidth (Entity text, int width);

/**
 * Should advanced wrap be used for this text entity? Advanced wrap does
 * the following:
 * - Break words that are too long to fit on a single wrap line
 * - Removes whitespaces at the beginning and end of each line
 * - Collapses neighboring spaces into one
 *
 * @since 0.0.0
 *
 * @param text The text entity to modify.
 * @param advanced Flag to set advanced wrap to.
 */
void SetTextAdvancedWrap (Entity entity, bool advanced);

/**
 * How should this text entity be aligned? Can be one of TEXT_ALIGNMENT
 * possible values.
 *
 * @since 0.0.0
 *
 * @param text The text entity to modify.
 * @param alignment The text alignment type to use.
 */
void SetTextAlign (Entity text, TEXT_ALIGNMENT alignment);

/**
 * TODO
 * @todo
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
