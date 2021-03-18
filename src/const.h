/**
 * @file		const.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef CONST_H
#define CONST_H

namespace Zen
{
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
		NONE,
		WIDTH_CONTROLS_HEIGHT,
		HEIGHT_CONTROLS_WIDTH,
		FIT,
		ENVELOP,
		RESIZE
	};

	/**
	 * The aspect modes used by the Size class.
	 *
	 * @since 0.0.0
	 */
	enum class ASPECT_MODE {
		NONE,
		WIDTH_CONTROLS_HEIGHT,
		HEIGHT_CONTROLS_WIDTH,
		FIT,
		ENVELOP
	}
}

#endif
