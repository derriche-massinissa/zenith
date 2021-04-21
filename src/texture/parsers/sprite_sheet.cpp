/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "sprite_sheet.hpp"

#include "../../utils/messages.hpp"
#include "../../utils/assert.hpp"
#include "../systems/texture.hpp"
#include "../systems/frame.hpp"
#include "../components/source.hpp"
#include "../components/frame.hpp"
#include "../components/texture.hpp"

namespace Zen {

extern entt::registry g_registry;

int ParseSpriteSheet (
		Entity texture,
		int sourceIndex,
		int x,
		int y,
		int width,
		int height,
		SpriteSheetConfig config)
{
	if (config.frameWidth <= 0)
	{
		MessageError("SpriteSheet: Invalid frameWidth given!");
		return -1;
	}

	// Get the source component
	Components::TextureSource *source = nullptr;
	for (auto& entity : g_registry.view<Components::TextureSource>())
	{
		auto& src = g_registry.get<Components::TextureSource>(entity);
		if (src.texture == texture && src.index == sourceIndex)
		{
			source = &src;
			break;
		}
	}
	ZEN_ASSERT(source, "The requested texture source does not exist.");

	// Add in a __BASE entry (for the entire atlas)
	AddFrame(texture, "__BASE", source->index, 0, 0, source->width, source->height);

	int startFrame = config.startFrame;
	int endFrame = config.endFrame;
	int margin = config.margin;
	int spacing = config.spacing;

	int row = std::floor((width - margin + spacing) / (config.frameWidth + spacing));
	int column = std::floor((height - margin + spacing) / (config.frameHeight + spacing));
	int total = row * column;

	if (total == 0)
	{
		auto& tx = g_registry.get<Components::Texture>(texture);
		MessageWarning("SpriteSheet frame dimensions will result in zero frame for texture: ", tx.key);
	}

	if (startFrame > total || startFrame < -total)
		startFrame = 0;

	if (startFrame < 0)
	{
		// Allow negative skipframes
		startFrame = total + startFrame;
	}

	if (endFrame != -1)
		total = startFrame + (endFrame + 1);

	int fx = margin;
	int fy = margin;
	int ax = 0;
	int ay = 0;

	for (int i = 0; i < total; i++)
	{
		ax = 0;
		ay = 0;

		int w = fx + config.frameWidth;
		int h = fy + config.frameHeight;

		if (w > width)
			ax = w - width;

		if (h > height)
			ay = h - height;

		AddFrame(texture, i, sourceIndex, x + fx, y + fy, config.frameWidth - ax , config.frameHeight - ay);

		fx += config.frameWidth + spacing;

		if (fx + config.frameWidth > width)
		{
			fx = margin;
			fy += config.frameHeight + spacing;
		}
	}

	return 0;
}

}	// namespace Zen
