/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "scale_manager.hpp"

#include "../window/window.hpp"
#include "../structs/size.hpp"
#include "../core/config.hpp"
#include "../window/window.hpp"

namespace Zen {

extern EventEmitter g_event;
extern Window g_window;

ScaleManager::~ScaleManager()
{
	stopListeners();
}

void ScaleManager::boot (GameConfig *cfg)
{
	config = cfg;

	parseConfig();

	startListeners();
}

void ScaleManager::parseConfig ()
{
	gameSize = CreateSize(config->width, config->height, config->scaleMode);
	displaySize = CreateSize(config->width, config->height, config->scaleMode);

	scaleMode = config->scaleMode;

	/*
	SetAspectMode(&gameSize, scaleMode);
	SetAspectMode(&displaySize, scaleMode);
	*/
}

void ScaleManager::setGameSize (int width_, int height_)
{
	if (scaleMode == SCALE_MODE::RESIZE)
		return;

	int previousWidth_ = gameSize.width;
	int previousHeight_ = gameSize.height;

	Resize(&gameSize, width_, height_);

	refresh();

	emit("resize", gameSize, displaySize, previousWidth_, previousHeight_);
}

void ScaleManager::refresh ()
{
	updateScale();
	updateOffset();
}

void ScaleManager::updateScale ()
{
	// This will take the aspect mode into account
	SetSize(&displaySize, g_window.width(), g_window.height());

	if (scaleMode == SCALE_MODE::RESIZE)
	{
		displayScale.x = 1.;
		displayScale.y = 1.;
	}
	else
	{
		displayScale.x = displaySize.width / gameSize.width;
		displayScale.y = displaySize.height / gameSize.height;
	}
}

void ScaleManager::updateOffset ()
{
	if (scaleMode == SCALE_MODE::RESIZE)
	{
		displayOffset.x = 0.;
		displayOffset.y = 0.;
	}
	else
	{
		displayOffset.x = (g_window.width() - displaySize.width) / 2.;
		displayOffset.y = (g_window.height() - displaySize.height) / 2.;
	}
}

int ScaleManager::transformX (int windowX_)
{
	return (windowX_ - displayOffset.x) / displayScale.x;
}

int ScaleManager::transformY (int windowY_)
{
	return (windowY_ - displayOffset.y) / displayScale.y;
}

void ScaleManager::startListeners ()
{
	g_window.on("resize", &ScaleManager::onResize, this);
}

void ScaleManager::stopListeners ()
{
	g_window.off("resize", &ScaleManager::onResize, this);
}

void ScaleManager::onResize (int width_, int height_)
{
	refresh();

	// Change the game size to fit the window if using RESIZE mode
	if (scaleMode == SCALE_MODE::RESIZE)
	{
		int previousWidth_ = gameSize.width;
		int previousHeight_ = gameSize.height;

		Resize(&gameSize, width_, height_);

		emit("resize", gameSize, displaySize, previousWidth_, previousHeight_);
	}
	else
	{
		emit("resize", gameSize, displaySize, gameSize.width, gameSize.height);
	}

}

void ScaleManager::setScaleMode (SCALE_MODE sm)
{
	scaleMode = sm;
	SetAspectMode(&gameSize, sm);
	SetAspectMode(&displaySize, sm);
}

int ScaleManager::getWidth ()
{
	return gameSize.width;
}

int ScaleManager::getHeight ()
{
	return gameSize.height;
}

}	// namespace Zen
