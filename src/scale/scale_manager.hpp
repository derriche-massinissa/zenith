/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SCALE_SCALE_MANAGER_HPP
#define ZEN_SCALE_SCALE_MANAGER_HPP

#include "../const.h"
#include "../structs/types/size.hpp"
#include "../event/event_emitter.hpp"
#include "../math/types/vector2.hpp"

#include "../window/window.fwd.hpp"
#include "../core/game.fwd.hpp"

namespace Zen {

/**
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
 * @class ScaleManager
 * @since 0.0.0
 */
class ScaleManager : public Events::EventEmitter
{
public:
	/**
	 * @since 0.0.0
	 *
	 * @param game_ - The Game instance.
	 */
	ScaleManager (Game& game_);

	/**
	 * @since 0.0.0
	 */
	~ScaleManager ();

	/**
	 * A reference to the Game instance.
	 *
	 * @since 0.0.0
	 */
	Game& game;

	/**
	 * A reference to the Window instance.
	 *
	 * @since 0.0.0
	 */
	Window& window;

	/**
	 * The Game Size component.
	 *
	 * The un-modified game size, as requested in the game config (the raw
	 * width / height), as used for world bounds, cameras, etc.
	 *
	 * @since 0.0.0
	 */
	Size gameSize;

	/**
	 * The Display Size component.
	 *
	 * The display size (Typically the window's size), factoring in the
	 * scale mode, window size and other values.
	 *
	 * @since 0.0.0
	 */
	Size displaySize;

	/**
	 * The game scale mode.
	 *
	 * @since 0.0.0
	 */
	SCALE_MODE scaleMode = SCALE_MODE::RESIZE;

	/**
	 * The scale factor between the gameSize and the window dimensions.
	 *
	 * @since 0.0.0
	 */
	Math::Vector2 displayScale {1.0, 1.0};

	/**
	 * The offset of the game view.
	 *
	 * Only used when using a SCALE_MODE that
	 * keeps a fixed aspect ratio. This is used to center the game view in the
	 * middle of the window.
	 *
	 * @since 0.0.0
	 */
	Math::Vector2 displayOffset {0.0, 0.0};

	/* TODO
	 * The current device orientation.
	 *
	 * @since 0.0.0
	ORIENTATION orientation_ = ORIENTATION::LANDSCAPE;
	 */

	/**
	 * The dirty state of the Scale Manager.
	 *
	 * Set if there is a change between the Window size and current size.
	 *
	 * @since 0.0.0
	 */
	bool dirty = false;

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

	void setGameSize (int width_, int height_);

	void refresh ();

	void updateScale ();

	void updateOffset ();

	void startListeners ();

	void stopListeners ();

	void onResize (int width_, int height_);

	void setScaleMode (SCALE_MODE sm_);

	int transformX (int windowX_);

	int transformY (int windowY_);

	int getWidth ();

	int getHeight ();

};	// class ScaleManager

}	// namespace Zen

#endif
