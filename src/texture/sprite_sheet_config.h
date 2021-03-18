/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TEXTURES_SPRITE_SHEET_CONFIG_H
#define ZEN_TEXTURES_SPRITE_SHEET_CONFIG_H

#include <string>

namespace Zen {
namespace Textures {

/**
 * @since 0.0.0
 *
 * @property atlas The key of the Texture Atlas in which this Sprite Sheet can be
 * found.
 * @property frame The key of the Texture Atlas Frame in which this Sprite Sheet 
 * can be found.
 * @property frameWidth The fixed width of each frame.
 * @property frameHeight The fixed height of each frame. If not set it will use the 
 * frameWidth as the height.
 * @property startFrame Skip a number of frames. Useful when there are multiple 
 * sprite sheets in one Texture.
 * @property endFrame The total number of frames to extract from the Sprite 
 * Sheet. The default value of -1 means "extract all frames".
 * @property margin If the frames have been drawn with a margin, specify the amount 
 * here.
 * @property spacing If the frames have been drawn with spacing between them, 
 * specify the amount here.
 */
struct SpriteSheetConfig
{
	std::string atlas = "";
	std::string frame = "";

	int frameWidth = 0;
	int frameHeight = 0;
	int startFrame = 0;
	int endFrame = -1;
	int margin = 0;
	int spacing = 0;
};

}	//namespace Textures
}	//namespace Zen

#endif
