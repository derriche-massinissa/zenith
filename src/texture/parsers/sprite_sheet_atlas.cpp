/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "sprite_sheet_atlas.h"

#include "../../messages.h"
#include "../texture.h"
#include "../texture_source.h"
#include "../frame.h"

namespace Zen {
namespace Textures {

void parseSpriteSheetFromAtlas (Texture *texture, Frame *frame, SpriteSheetConfig config)
{
	if (config.frameWidth <= 0)
	{
		messageError("SpriteSheet: Invalid frameWidth given!");
		return;
	}

	// Add in a __BASE entry (for the entire frame)
	TextureSource& source = texture->source[0];

	texture->add("__BASE", 0, 0, 0, source.width, source.height);

	int startFrame = config.startFrame;
	int endFrame = config.endFrame;
	int margin = config.margin;
	int spacing = config.spacing;

	int x = frame->cutX;
	int y = frame->cutY;

	int cutWidth = frame->cutWidth;
	int cutHeight = frame->cutHeight;
	int sheetWidth = frame->getRealWidth();
	int sheetHeight = frame->getRealHeight();

	int row = std::floor((sheetWidth - margin + spacing) / (config.frameWidth + spacing));
	int column = std::floor((sheetHeight - margin + spacing) / (config.frameHeight + spacing));
	int total = row * column;

	// Trim offsets
	
	int leftPad = frame->x;
	int leftWidth = config.frameWidth - leftPad;

	int rightWidth = config.frameWidth - ((sheetWidth - cutWidth) - leftPad);

	int topPad = frame->y;
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

	Frame* sheetFrame = nullptr;
	int frameX = margin;
	int frameY = margin;
	int frameIndex = 0;
	int sourceIndex = frame->sourceIndex;

	for (int sheetY = 0; sheetY < column; sheetY++)
	{
		bool topRow = (sheetY == 0);
		bool bottomRow = (sheetY == column - 1);

		for (int sheetX = 0; sheetX < row; sheetX++)
		{
			bool leftRow = (sheetX == 0);
			bool rightRow = (sheetX == row - 1);

			sheetFrame = texture->add(frameIndex, sourceIndex, x + frameX, y + frameY, config.frameWidth, config.frameHeight);

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

				sheetFrame->cutWidth = destWidth;
				sheetFrame->cutHeight = destHeight;

				sheetFrame->setTrim(config.frameWidth, config.frameHeight, destX, destY, destWidth, destHeight);
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

}	// namespace Textures
}	// namespace Zen
