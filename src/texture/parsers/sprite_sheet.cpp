/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "sprite_sheet.h"

#include "../../messages.h"
#include "../texture.h"
#include "../texture_source.h"

namespace Zen {
namespace Textures {

void parseSpriteSheet (
		Texture *texture,
		int sourceIndex,
		int x,
		int y,
		int height,
		int width,
		SpriteSheetConfig config)
{
	if (config.frameWidth <= 0)
	{
		messageError("SpriteSheet: Invalid frameWidth given!");
		return;
	}

	// Add in a __BASE entry (for the entire atlas)
	TextureSource& source = texture->source[sourceIndex];

	texture->add("__BASE", sourceIndex, 0, 0, source.width, source.height);

	int startFrame = config.startFrame;
	int endFrame = config.endFrame;
	int margin = config.margin;
	int spacing = config.spacing;

	int row = std::floor((width - margin + spacing) / (config.frameWidth + spacing));
	int column = std::floor((height - margin + spacing) / (config.frameHeight + spacing));
	int total = row * column;

	if (total == 0)
		messageWarning("SpriteSheet frame dimensions will result in zero frame for texture: ", texture->key);

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

		texture->add(i, sourceIndex, x + fx, y + fy, config.frameWidth - ax , config.frameHeight - ay);

		fx += config.frameWidth + spacing;

		if (fx + config.frameWidth > width)
		{
			fx = margin;
			fy += config.frameHeight + spacing;
		}
	}
}

}	// namespace Textures
}	// namespace Zen
