/**
 * @file		scale_manager.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef SCALE_MANAGER_H
#define SCALE_MANAGER_H

#include "../const.h"
#include "../structs/size.h"
#include "../event/event_emitter.h"
#include "../math/math.h"
#include "../math/vector2.h"

namespace Zen {
namespace Scale {

/**
 * @class ScaleManager
 * The Scale Manager handles the scaling, resizing and alignment of the
 * game in the window.
 *
 * The way scaling is handled is by setting the game dimensions to a
 * fixed size. The Scale Manager will then look at the available space
 * within the window and scale the game accordingly. Scaling is handled
 * by stretching the game up or down, leaving the actual dimentions
 * of the game untouched.
 *
 * ### Scale Modes
 *
 * The way the scaling is handled is determined by the "scaleMode"
 * property. The default is "NONE", which prevents the scaling of the game
 * at all.
 *
 * @since 0.0.0
 */
class ScaleManager : public EventEmitter
{
public:
	/**
	 * @since 0.0.0
	 *
	 * @param game - The Game instance.
	 */
	ScaleManager (Game& game);

	/**
	 * @since 0.0.0
	 */
	~ScaleManager (Game& game);

	/**
	 * A reference to the Game instance.
	 *
	 * @since 0.0.0
	 */
	Game& game_;

	/**
	 * A reference to the Window instance.
	 *
	 * @since 0.0.0
	 */
	Window& window_;

	/**
	 * The Game Size component.
	 *
	 * The un-modified game size, as requested in the game config (the raw
	 * width / height), as used for world bounds, cameras, etc.
	 *
	 * @since 0.0.0
	 */
	Structs::Size gameSize_;

	/**
	 * The Display Size component.
	 *
	 * The display size (Typically the window's size), factoring in the
	 * scale mode, window size and other values.
	 *
	 * @since 0.0.0
	 */
	Structs::Size displaySize_;

	/**
	 * The game scale mode.
	 *
	 * @since 0.0.0
	 */
	SCALE_MODE scaleMode_ = MODE::NONE;

	/**
	 * The scale factor between the baseSize and the canvasBounds.
	 *
	 * @since 0.0.0
	 */
	Math::Vector2 displayScale_ {1.0, 1.0};

	Math::Vector2 displayOffset_ {1.0, 1.0};

	/**
	 * The current device orientation.
	 *
	 * @since 0.0.0
	 */
	ORIENTATION orientation_ = ORIENTATION::LANDSCAPE;

	/**
	 * Is the window fullscreen?
	 *
	 * @since 0.0.0
	 */
	bool fullscreen_;

	/**
	 * The dirty state of the Scale Manager.
	 *
	 * Set if there is a change between the Window size and current size.
	 *
	 * @since 0.0.0
	 */
	bool dirty_ = false;

	/**
	 * Called _before_ the window object is initialized and opened.
	 *
	 * @since 0.0.0
	 */
	void preBoot ();

    /**
     * The Boot handler is called by Game when it first starts up.
     * The renderer is available by now and the window is open.
     *
     * @since 0.0.0
     */
    void boot ();

    /**
     * Parses the game configuration to set-up the scale defaults.
     *
     * @since 0.0.0
     *
     * @param config The Game configuration object.
     */
    void parseConfig ();

	void setGameSize (int width, int height);

	void resize (int width ,int height);

	void refresh (int previousWidth, int previousHeight);

	void updateScale ();

	void updateOffset ();

	int transformX (int windowX);

	int transformY (int windowY);

	void startListeners ();

	void stopListeners ();

	void onResize (int width, int height);

};	// class ScaleManager
}	// namespace Scale
}	// namespace Zen

#endif
