/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TWEENS_TYPES_TWEENDATAGENCONFIG_HPP
#define ZEN_TWEENS_TYPES_TWEENDATAGENCONFIG_HPP

#include <SDL2/SDL_stdinc.h>
#include <functional>
#include "../../ecs/entity.hpp"

namespace Zen {

/**
 * @struct TweenDataGenConfig
 * @since 0.0.0
 */
struct TweenDataGenConfig
{
	/**
	 * Time in ms/frames before tween will start.
	 *
	 * @since 0.0.0
	 */
	std::function<Uint32(Entity, double, size_t, size_t)> delay = nullptr;

	/**
	 * Duration of the tween in ms/frames, excludes time for yoyo or repeats.
	 *
	 * @since 0.0.0
	 */
	std::function<Uint32(Entity, double, size_t, size_t)> duration = nullptr;

	/**
	 * Time in ms/frames the tween will pause before running the yoyo or starting a
	 * repeat.
	 *
	 * @since 0.0.0
	 */
	std::function<Uint32(Entity, double, size_t, size_t)> hold = nullptr;

	/**
	 * Number of times to repeat the tween. The tween will always run once
	 * regardless, so a repeat value of '1' will play the tween twice.
	 *
	 * @since 0.0.0
	 */
	std::function<int(Entity, double, size_t, size_t)> repeat = nullptr;

	/**
	 * Time in ms/frames before the repeat will start.
	 *
	 * @since 0.0.0
	 */
	std::function<Uint32(Entity, double, size_t, size_t)> repeatDelay = nullptr;
};

}	// namespace Zen

#endif
