/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../background_color.hpp"

#include "../../components/background_color.hpp"
#include "../../components/transparent.hpp"
#include "../../utils/assert.hpp"
#include "../../display/color.hpp"

namespace Zen {

extern entt::registry g_registry;

void SetBackgroundColor (Entity entity, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	auto [bgc, transparent] = g_registry.try_get<Components::BackgroundColor, Components::Transparent>(entity);
	ZEN_ASSERT(bgc, "The entity has no 'BackgroundColor' component.");

	SetTo(&bgc->color, red, green, blue, alpha);

	if (!transparent)
		return;

	if (alpha == 0)
		transparent->value = true;
	else
		transparent->value = false;
}

void SetBackgroundColor (Entity entity, uint32_t color)
{
	auto [bgc, transparent] = g_registry.try_get<Components::BackgroundColor, Components::Transparent>(entity);
	ZEN_ASSERT(bgc, "The entity has no 'BackgroundColor' component.");

	SetHex(&bgc->color, color);

	if (!transparent)
		return;

	if (bgc->color.alpha == 0)
		transparent->value = true;
	else
		transparent->value = false;
}

void SetBackgroundColor (Entity entity, Color color)
{
	auto [bgc, transparent] = g_registry.try_get<Components::BackgroundColor, Components::Transparent>(entity);
	ZEN_ASSERT(bgc, "The entity has no 'BackgroundColor' component.");

	bgc->color = color;

	if (!transparent)
		return;

	if (bgc->color.alpha == 0)
		transparent->value = true;
	else
		transparent->value = false;
}

Color GetBackgroundColor (Entity entity)
{
	auto bgc = g_registry.try_get<Components::BackgroundColor>(entity);
	ZEN_ASSERT(bgc, "The entity has no 'BackgroundColor' component.");

	return bgc->color;
}

}	// namespace Zen
