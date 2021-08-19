/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../text.hpp"

#include <vector>
#include "../../utils/assert.hpp"
#include "../../utils/messages.hpp"
#include "../../components/text.hpp"
#include "../../text/text_manager.hpp"

namespace Zen {

extern entt::registry g_registry;
extern TextManager g_text;

bool IsText (Entity entity)
{
	return g_registry.has<Components::Text>(entity);
}

void SetText (Entity entity, std::string content)
{
	auto text = g_registry.try_get<Components::Text>(entity);
	ZEN_ASSERT(text, "The entity has no 'Text' component");

	text->text = content;

	// Update inner properties and cache of the text manager
	g_text.scanText(entity);
}

void SetTextStyle (Entity entity, TextStyle style)
{
	auto text = g_registry.try_get<Components::Text>(entity);
	ZEN_ASSERT(text, "The entity has no 'Text' component");

	text->style = style;

	// Update inner properties and cache of the text manager
	g_text.scanText(entity);
}

void SetFontFamily(Entity entity, std::string fontFamily)
{
	auto text = g_registry.try_get<Components::Text>(entity);
	ZEN_ASSERT(text, "The entity has no 'Text' component");

	text->style.fontFamily = fontFamily;

	// Update inner properties and cache of the text manager
	g_text.scanText(entity);
}

void SetTextColor (Entity entity, int color)
{
	auto text = g_registry.try_get<Components::Text>(entity);
	ZEN_ASSERT(text, "The entity has no 'Text' component");

	text->style.color = color;
}

void SetFontSize (Entity entity, int size)
{
	if (size <= 0) {
		MessageError("Font size should be larger than zero");
		return;
	}

	auto text = g_registry.try_get<Components::Text>(entity);
	ZEN_ASSERT(text, "The entity has no 'Text' component");

	text->style.fontSize = size;

	// Update inner properties and cache of the text manager
	g_text.scanText(entity);
}

void SetTextDecoration (Entity entity, TEXT_DECORATION decoration)
{
	auto text = g_registry.try_get<Components::Text>(entity);
	ZEN_ASSERT(text, "The entity has no 'Text' component");

	text->style.decoration = decoration;
}

void SetTextOutline (Entity entity, int outlineWidth)
{
	auto text = g_registry.try_get<Components::Text>(entity);
	ZEN_ASSERT(text, "The entity has no 'Text' component");

	text->style.outline = outlineWidth;
}

void SetTextPadding (Entity entity, int padding)
{
	auto text = g_registry.try_get<Components::Text>(entity);
	ZEN_ASSERT(text, "The entity has no 'Text' component");

	text->style.paddingTop = padding;
	text->style.paddingBottom = padding;
	text->style.paddingLeft = padding;
	text->style.paddingRight = padding;
}

void SetTextPaddingTop (Entity entity, int padding)
{
	auto text = g_registry.try_get<Components::Text>(entity);
	ZEN_ASSERT(text, "The entity has no 'Text' component");

	text->style.paddingTop = padding;
}

void SetTextPaddingBottom (Entity entity, int padding)
{
	auto text = g_registry.try_get<Components::Text>(entity);
	ZEN_ASSERT(text, "The entity has no 'Text' component");

	text->style.paddingBottom = padding;
}

void SetTextPaddingLeft (Entity entity, int padding)
{
	auto text = g_registry.try_get<Components::Text>(entity);
	ZEN_ASSERT(text, "The entity has no 'Text' component");

	text->style.paddingLeft = padding;
}

void SetTextPaddingRight (Entity entity, int padding)
{
	auto text = g_registry.try_get<Components::Text>(entity);
	ZEN_ASSERT(text, "The entity has no 'Text' component");

	text->style.paddingRight = padding;
}

void SetTextWrapWidth (Entity entity, int width)
{
	auto text = g_registry.try_get<Components::Text>(entity);
	ZEN_ASSERT(text, "The entity has no 'Text' component");

	text->style.wrapWidth = width;

	// Update inner properties and cache of the text manager
	g_text.scanText(entity);
}

void SetTextAdvancedWrap (Entity entity, bool advanced)
{
	auto text = g_registry.try_get<Components::Text>(entity);
	ZEN_ASSERT(text, "The entity has no 'Text' component");

	text->style.advancedWrap = advanced;

	// Update inner properties and cache of the text manager
	g_text.scanText(entity);
}

void SetTextAlign (Entity entity, TEXT_ALIGNMENT alignment)
{
	auto text = g_registry.try_get<Components::Text>(entity);
	ZEN_ASSERT(text, "The entity has no 'Text' component");

	text->style.alignment = alignment;
}

void SetTextBackgroundColor (Entity entity, int color)
{
	auto text = g_registry.try_get<Components::Text>(entity);
	ZEN_ASSERT(text, "The entity has no 'Text' component");

	text->style.backgroundColor = color;
}

std::string GetText(Entity entity)
{
	auto text = g_registry.try_get<Components::Text>(entity);
	ZEN_ASSERT(text, "The entity has no 'Text' component");

	return text->text;
}

}	// namespace Zen
