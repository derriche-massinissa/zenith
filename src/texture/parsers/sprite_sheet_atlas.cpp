/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "sprite_sheet_atlas.hpp"

#include "../../utils/messages.hpp"
#include "../../utils/assert.hpp"
#include "../systems/texture.hpp"
#include "../systems/frame.hpp"
#include "../components/source.hpp"
#include "../components/frame.hpp"

namespace Zen {

extern entt::registry g_registry;

void ParseSpriteSheetFromAtlas (Entity texture, Entity frame, SpriteSheetConfig config)
{
	if (config.frameWidth <= 0)
	{
		MessageError("SpriteSheet: Invalid frameWidth given!");
		return;
	}

	// Get the source component
	Components::TextureSource *source = nullptr;
	for (auto& entity : g_registry.view<Components::TextureSource>())
	{
		auto& src = g_registry.get<Components::TextureSource>(entity);
		if (src.texture == texture && src.index == 0)
		{
			source = &src;
			break;
		}
	}
	ZEN_ASSERT(source, "The requested texture source does not exist.");

	// Add in a __BASE entry (for the entire frame)
	AddFrame(texture, "__BASE", 0, 0, 0, source->width, source->height);

	auto& fr = g_registry.get<Components::Frame>(frame);

	int startFrame = config.startFrame;
	int endFrame = config.endFrame;
	int margin = config.margin;
	int spacing = config.spacing;

	int x = fr.cutX;
	int y = fr.cutY;

	int cutWidth = fr.cutWidth;
	int cutHeight = fr.cutHeight;
	int sheetWidth = GetFrameRealHeight(frame);
	int sheetHeight = GetFrameRealHeight(frame);

	int row = std::floor((sheetWidth - margin + spacing) / (config.frameWidth + spacing));
	int column = std::floor((sheetHeight - margin + spacing) / (config.frameHeight + spacing));
	int total = row * column;

	// Trim offsets

	int leftPad = fr.x;
	int leftWidth = config.frameWidth - leftPad;

	int rightWidth = config.frameWidth - ((sheetWidth - cutWidth) - leftPad);

	int topPad = fr.y;
	int topHeight = config.frameHeight - topPad;

	int bottomHeight = config.frameHeight - ((sheetHeight - cutHeight) - topPad);

	if (startFrame > total || startFrame < -total)
		startFrame = 0;

	if (startFrame < 0)
	{
		// Allow negative skipframes
		startFrame = total + startFrame;
	}

	if (endFrame != -1)
		total = startFrame + (endFrame + 1);

	Entity sheetFrame = entt::null;
	int frameX = margin;
	int frameY = margin;
	int frameIndex = 0;
	int sourceIndex = source->index;

	for (int sheetY = 0; sheetY < column; sheetY++)
	{
		bool topRow = (sheetY == 0);
		bool bottomRow = (sheetY == column - 1);

		for (int sheetX = 0; sheetX < row; sheetX++)
		{
			bool leftRow = (sheetX == 0);
			bool rightRow = (sheetX == row - 1);

			sheetFrame = AddFrame(texture, frameIndex, sourceIndex, x + frameX, y + frameY, config.frameWidth, config.frameHeight);
			auto& sfr = g_registry.get<Components::Frame>(sheetFrame);

			if (leftRow || topRow || rightRow || bottomRow)
			{
				int destX = (leftRow) ? leftPad : 0;
				int destY = (topRow) ? topPad : 0;

				int trimWidth = 0;
				int trimHeight = 0;

				if (leftRow)
					trimWidth += config.frameWidth - leftWidth;

				if (rightRow)
					trimWidth += config.frameWidth - rightWidth;

				if (topRow)
					trimHeight += config.frameHeight - topHeight;

				if (bottomRow)
					trimHeight += config.frameHeight - bottomHeight;

				int destWidth = config.frameWidth - trimWidth;
				int destHeight = config.frameHeight - trimHeight;

				sfr.cutWidth = destWidth;
				sfr.cutHeight = destHeight;

				SetFrameTrim(sheetFrame, config.frameWidth, config.frameHeight, destX, destY, destWidth, destHeight);
			}

			frameX += spacing;

			if (leftRow)
				frameX += leftWidth;
			else if (rightRow)
				frameX += rightWidth;
			else
				frameX += config.frameWidth;

			frameIndex++;
		}

		frameX = margin;
		frameY += spacing;

		if (topRow)
			frameY += topHeight;
		else if (bottomRow)
			frameY += bottomHeight;
		else
			frameY += config.frameHeight;
	}
}

}	// namespace Zen
