/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_CONST_H
#define ZEN_CONST_H

namespace Zen {

/**
 * All the Scene states.
 *
 * @since 0.0.0
 */
enum class SCENE {
	PENDING,
	INIT,
	START,
	LOADING,
	CREATING,
	RUNNING,
	PAUSED,
	SLEEPING,
	SHUTDOWN,
	DESTROYED
};

enum class WINDOW {
	HIDDEN,
	VISIBLE,
	BLURRED,
	FOCUSED
};

/**
 * The elements to cleanup once the window closed.
 *
 * @since 0.0.0
 */
enum class CLEANUP {
	SDL,
	IMG,
	MIX,
	TTF
};

/**
 * The scale modes.
 *
 * @since 0.0.0
 */
enum class SCALE_MODE {
	RESIZE,
	WIDTH_CONTROLS_HEIGHT,
	HEIGHT_CONTROLS_WIDTH,
	FIT,
	ENVELOP
};

enum class BLEND_MODE {
	NORMAL,
	ADD,
	MULTIPLY,
	SCREEN,
	OVERLAY,
	DARKEN,
	LIGHTEN,
	COLOR_DODGE,
	COLOR_BURN,
	HARD_LIGHT,
	SOFT_LIGHT,
	DIFFERENCE,
	EXCLUSION,
	HUE,
	SATURATION,
	COLOR,
	LUMINOSITY,
	ERASE,
	SOURCE_IN,
	SOURCE_OUT,
	SOURCE_ATOP,
	DESTINATION_OVER,
	DESTINATION_IN,
	DESTINATION_OUT,
	DESTINATION_ATOP,
	LIGHTER,
	COPY,
	XOR
}

}	// namespace Zen

#endif
