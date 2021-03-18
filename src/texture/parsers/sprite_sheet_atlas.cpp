/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "sprite_sheet_atlas.h"

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
	TextureSource *source = texture->source[0];

	texture->add("__BASE", 0, 0, 0, source->width, source->height);

	int startFrame = config.startFrame;
	int endFrame = config.endFrame;
	int margin = config.margin;
	int spacing = config.spacing;

	int x = frame->cutX;
	int y = frame->cutY;

	int cutWidth = frame->cutWidth;
	int cutHeight = frame->cutHeight;
	int sheetWidth = frame->realWidth;
	int sheetHeight = frame->realHeight;

	int row = std::floor((sheetWidth - margin + spacing) / (frameWidth + spacing));
	int column = std::floor((sheetHeight - margin + spacing) / (frameHeight + spacing));
	int total = row * column;

	// Trim offsets
	
	int leftPad = frame.x;
	int leftWidth = frameWidth - leftPad;

	int rightWidth = frameWidth - ((sheetWidth - cutWidth) - leftPad);

	int topPad = frame.x;
	int topHeight = frameHeight - topPad;

	int bottomHeight = frameHeight - ((sheetHeight - cutHeight) - topPad);

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
	int sourceIndex = frame.sourceIndex;

	for (int sheetY = 0; sheetY < column; sheetY++)
	{
		bool topRow = (sheetY == 0);
		bool bottomRow = (sheetY == column - 1);

		for (int sheetX = 0; sheetX < row; sheetX++)
		{
			bool leftRow = (sheetX == 0);
			bool rightRow = (sheetX == row - 1);

			sheetFrame = texture->add(frameIndex, sourceIndex, x + frameX, y + frameY, frameWidth, frameHeight);

			if (leftRow || topRow || rightRow || bottomRow)
			{
				int destX = (leftRow) ? leftPad : 0;
				int destY = (topRow) ? topPad : 0;

				int trimWidth = 0;
				int trimHeight = 0;

				if (leftRow)
					trimWidth += frameWidth - leftWidth;

				if (rightRow)
					trimWidth += frameWidth - rightWidth;

				if (topRow)
					trimHeight += frameHeight - topHeight;

				if (bottomRow)
					trimHeight += frameHeight - bottomHeight;

				int destWidth = frameWidth - trimWidth;
				int destHeight = frameHeight - trimHeight;

				sheetFrame->cutWidth = destWidth;
				sheetFrame->cutHeight = destHeight;

				sheetFrame->setTrim(frameWidth, frameHeight, destX, destY, destWidth, destHeight);
			}

			frameX += spacing;

			if (leftRow)
				frameX += leftWidth;
			else if (rightRow)
				frameX += rightWidth;
			else
				frameX += frameWidth;

			frameIndex++;
		}

		frameX = margin;
		frameY += spacing;

		if (topRow)
			frameY += topHeight;
		else if (bottomRow)
			frameY += bottomHeight;
		else
			frameY += frameHeight;
	}
}

}	// namespace Textures
}	// namespace Zen
