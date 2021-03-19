/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "scale_manager.h"

#include "../window/window.h"
#include "../core/game.h"

namespace Zen {
namespace Scale {

ScaleManager::ScaleManager(Game& game_)
	: game(game_), window(game_.window)
{}

ScaleManager::~ScaleManager()
{
	stopListeners();
}

void ScaleManager::preBoot ()
{
	game.events.once("boot", &ScaleManager::boot, this);
}

void ScaleManager::boot ()
{
	parseConfig();

	startListeners();
}

void ScaleManager::parseConfig ()
{
	gameSize.setSize(game.config.width, game.config.height);

	scaleMode = game.config.scaleMode;

	gameSize.setAspectMode(scaleMode);
	displaySize.setAspectMode(scaleMode);
}

void ScaleManager::setGameSize (int width_, int height_)
{
	if (scaleMode == SCALE_MODE::RESIZE)
		return;

	int previousWidth_ = gameSize.getWidth();
	int previousHeight_ = gameSize.getHeight();

	gameSize.resize(width_, height_);

	refresh();

	emit("resize", gameSize, displaySize, previousWidth_, previousHeight_);
}

void ScaleManager::resizeGame (int width_ ,int height_)
{
	int previousWidth_ = gameSize.getWidth();
	int previousHeight_ = gameSize.getHeight();

	gameSize.resize(width_, height_);

	emit("resize", gameSize, displaySize, previousWidth_, previousHeight_);
}

void ScaleManager::refresh ()
{
	updateScale();
	updateOffset();

	// Change the game size to fit the window if using RESIZE mode
	if (scaleMode == SCALE_MODE::RESIZE)
	{
		resizeGame(displaySize.getWidth(), displaySize.getHeight());
	}
}

void ScaleManager::updateScale ()
{
	// This will take the aspect mode into account
	displaySize.setSize(window.width(), window.height());

	if (scaleMode == SCALE_MODE::RESIZE)
	{
		displayScale.set(1, 1);
	}
	else
	{
		displayScale.set(
				gameSize.getWidth() / displaySize.getWidth(),
				gameSize.getHeight() / displaySize.getHeight()
				);
	}
}

void ScaleManager::updateOffset ()
{
	if (scaleMode == SCALE_MODE::RESIZE)
		displayOffset.set(0, 0);
	else
		displayOffset.set(
				(window.width() - displaySize.getWidth()) / 2.0,
				(window.height() - displaySize.getHeight()) / 2.0
				);
}

int ScaleManager::transformX (int windowX_)
{
	return (windowX_ - displayOffset.x) * displayScale.x;
}

int ScaleManager::transformY (int windowY_)
{
	return (windowY_ - displayOffset.y) * displayScale.y;
}

void ScaleManager::startListeners ()
{
	window.on("resize", &ScaleManager::onResize, this);
}

void ScaleManager::stopListeners ()
{
	window.off("resize", &ScaleManager::onResize, this);
}

void ScaleManager::onResize (int width, int height)
{
	refresh();
}

void ScaleManager::setScaleMode (SCALE_MODE sm)
{
	scaleMode = sm;
	gameSize.setAspectMode(sm);
	displaySize.setAspectMode(sm);
}

int ScaleManager::getWidth ()
{
	return gameSize.getWidth();
}

int ScaleManager::getHeight ()
{
	return gameSize.getHeight();
}

}	// namespace Scale
}	// namespace Zen
