/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "config.hpp"

#include "../display/color.hpp"

namespace Zen {

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

GameConfig& GameConfig::setMinWidth (int mw)
{
	minWidth = mw;

	return *this;
}

GameConfig& GameConfig::setMaxWidth (int mw)
{
	maxWidth = mw;

	return *this;
}

GameConfig& GameConfig::setMinHeight (int mh)
{
	minHeight = mh;

	return *this;
}

GameConfig& GameConfig::setMaxHeight (int mh)
{
	maxHeight = mh;

	return *this;
}

GameConfig& GameConfig::setHiddenDelta (unsigned int delta)
{
	hiddenDelta = delta;

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
	renderConfig.antialias = flag;

	return *this;
}

GameConfig& GameConfig::setPixelArt (bool flag)
{
	renderConfig.pixelArt = flag;

	if (flag)
		setAntialiasing(true);
	else
		setAntialiasing(false);

	return *this;
}

GameConfig& GameConfig::setRoundPixels (bool flag)
{
	renderConfig.roundPixels = flag;

	return *this;
}

GameConfig& GameConfig::setClearBeforeRender (bool flag)
{
	renderConfig.clearBeforeRender = flag;

	return *this;
}

GameConfig& GameConfig::setPreserveDrawingBuffer (bool flag)
{
	renderConfig.preserveDrawingBuffer = flag;

	return *this;
}

GameConfig& GameConfig::setBatchSize (int size)
{
	if (size <= 0) size = 1;

	renderConfig.batchSize = size;

	return *this;
}

GameConfig& GameConfig::setMaxTextures (int number)
{
	if (number <= 0) number = -1;

	renderConfig.maxTextures = number;

	return *this;
}

GameConfig& GameConfig::setMaxTextureSize (int size)
{
	if (size <= 0) size = -1;

	renderConfig.maxTextureSize = size;

	return *this;
}

GameConfig& GameConfig::setMipMapFilter (GLenum filter)
{
	renderConfig.mipmapFilter = filter;

	return *this;
}

GameConfig& GameConfig::setBackgroundColor (unsigned int color)
{
	SetHex(&renderConfig.backgroundColor, color);

	return *this;
}

GameConfig& GameConfig::setBackgroundColor (
			unsigned int red, unsigned int green, unsigned int blue)
{
	SetTo(&renderConfig.backgroundColor, red, green, blue);

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

}	// namespace Zen
