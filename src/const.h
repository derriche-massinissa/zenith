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

	enum class CLEANUP {
		SDL,
		IMG,
		MIX,
		TTF
	};

	enum class SCALE_MODE {
		FIT,
		RESIZE
	};
}

#endif
