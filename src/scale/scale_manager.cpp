/**
 * @file		scale_manager.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "scale_manager.h"

namespace Zen {
namespace Scale {

ScaleManager::ScaleManager(Game& game)
	: game_(game), window_(game.window)
{}

ScaleManager::~ScaleManager()
{
	stopListeners();
}

void ScaleManager::preBoot ()
{
	parseConfig(game_.config);

	game.events.once("SYS_BOOT", &ScaleManager::boot, this);
}

void ScaleManager::boot ()
{
	startListeners();
}

void ScaleManager::parseConfig ()
{
	gameSize_.setSize(game_.config.width, game_.config.height);

	scaleMode_ = game_.config.scaleMode;

	gameSize_.setAspectMode(scaleMode_);
	displaySize_.setAspectMode(scaleMode_);
}

void ScaleManager::setGameSize (int width, int height)
{
	int previousWidth = gameSize_.getWidth();
	int previousHeight = gameSize_.getHeight();

	gameSize_.resize(width, height);

	refresh(previousWidth, previousHeight);

	emit("resize", gameSize_.getWidth(), gameSize_.getHeight(), previousWidth, previousHeight);
}

void ScaleManager::resize (int width ,int height)
{
	setGameSize(width, height);
}

void ScaleManager::refresh ()
{
	updateScale();
	updateOffset();
}

void ScaleManager::updateScale ()
{
	displaySize_.setSize(window_.width(), window_.height());

	if (scaleMode_ == SCALE_MODE::RESIZE)
		displayScale_.set(1, 1);
	else
		displayScale_.set(
				gameSize_.getWidth() / displaySize_.getWidth(),
				gameSize_.getHeight() / displaySize_.getHeight()
				);
}

void ScaleManager::updateOffset ()
{
	if (scaleMode_ == SCALE_MODE::RESIZE)
		displayOffset_.set(0, 0);
	else
		displayOffset_.set(
				(window_.width() - displaySize_.getWidth()) / 2.0,
				(window_.height() - displaySize_.getHeight()) / 2.0
				);
}

int ScaleManager::transformX (int windowX)
{
	return (windowX - displayOffset_.x) * displayScale_.x;
}

int ScaleManager::transformY (int windowY)
{
	return (windowY - displayOffset_.y) * displayScale_.y;
}

void ScaleManager::startListeners ()
{
	window_.on("resize", &ScaleManager::onResize, this);
}

void ScaleManager::stopListeners ()
{
	window_.off("resize", &ScaleManager::onResize, this);
}

void ScaleManager::onResize (int width, int height)
{
	refresh();
}

void ScaleManager::setScaleMode (SCALE_MODE sm)
{
	scaleMode_ = sm;
	gameSize_.setAspectMode(sm);
	displaySize_.setAspectMode(sm);
}

}	// namespace Scale
}	// namespace Zen
