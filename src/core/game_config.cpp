/**
 * @file		game_config.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "game_config.h"

namespace Zen {
namespace Core {

GameConfig& GameConfig::setWidth (int w)
{
	width = w;

	return *this;
}

GameConfig& GameConfig::setHeight (int h)
{
	height = h;

	return *this;
}

GameConfig& GameConfig::setScaleMode (SCALE_MODE sm)
{
	scaleMode = sm;

	return *this;
}

GameConfig& GameConfig::setFullscreen (bool flag)
{
	fullscreen = flag;

	return *this;
}

GameConfig& GameConfig::setTitle (std::string t)
{
	title = t;

	return *this;
}

GameConfig& GameConfig::setVersion (std::string v)
{
	version = v;

	return *this;
}

GameConfig& GameConfig::setKeyboardInput (bool flag)
{
	inputKeyboard = flag;

	return *this;
}

GameConfig& GameConfig::setMouseInput (bool flag)
{
	inputMouse = flag;

	return *this;
}

GameConfig& GameConfig::setAntialiasing (bool flag)
{
	antialias = flag;

	return *this;
}

GameConfig& GameConfig::setPixelArt (bool flag)
{
	pixelArt = flag;

	if (flag)
		setAntialiasing(true);
	else
		setAntialiasing(false);

	return *this;
}

GameConfig& GameConfig::setBackgroundColor (unsigned int color)
{
	backgroundColor.setFromHex(color);

	return *this;
}
GameConfig& clearBeforeRender (bool flag)
{
	clearBeforeRender = flag;

	return *this;
}

GameConfig& GameConfig::setBackgroundColor (
			unsigned int red, unsigned int green, unsigned int blue)
{
	backgroundColor.setTo(red, green, blue);

	return *this;
}

GameConfig& GameConfig::setLoaderPath (std::string path)
{
	loaderPath = path;

	return *this;
}

GameConfig& GameConfig::setLoaderPrefix (std::string prefix)
{
	loaderPrefix = prefix;

	return *this;
}

}	// namespace Core
}	// namespace Zen
