/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https:*opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TWEENS_TWEENDATA_HPP
#define ZEN_TWEENS_TWEENDATA_HPP

#include <SDL2/SDL_stdinc.h>
#include <functional>
#include "../../ecs/entity.hpp"
#include "const.hpp"
#include "../types/tween_data_gen_config.hpp"

namespace Zen {
/**
 * A single TweenData struct that describes the tween data for a unique property
 * of a unique target. A single Tween consists of multiple TweenDatas, depending on
 * how many properties are being changed by the Tween.
 *
 * @struct TweenData
 * @since 0.0.0
 */
struct TweenData
{
	/**
	 * The target to tween
	 *
	 * @since 0.0.0
	 */
	Entity target = entt::null;

	/**
	 * The index of the target within the tween targets array
	 *
	 * @since 0.0.0
	 */
	size_t index = 0;

	/**
	 * What the tween value should start from.
	 *
	 * @since 0.0.0
	 */
	double from = 0;

	/**
	 * What the tween value should end at.
	 *
	 * @since 0.0.0
	 */
	double to = 0;


	/**
	 * The tweening action.
	 *
	 * @since 0.0.0
	 */
	std::function<void(Entity, double)> action = [] (Entity, double) -> void {};

	/**
	 * What to set the property to the moment the TweenData is invoked.
	 *
	 * @since 0.0.0
	 */
	std::function<double(Entity, double, size_t, size_t)> getActiveValue = nullptr;

	/**
	 * The returned value sets what the property will be at the START of the Tween.
	 *
	 * @since 0.0.0
	 */
	std::function<double(Entity, double, size_t, size_t)> getStartValue = nullptr;

	/**
	 * The returned value sets what the property will be at the END of the Tween.
	 *
	 * @since 0.0.0
	 */
	std::function<double(Entity, double, size_t, size_t)> getEndValue = nullptr;

	/**
	 * The ease function this tween uses.
	 *
	 * @since 0.0.0
	 */
	std::function<double(double)> ease = [] (double value) { return value; };

	/**
	 * Duration of the tween in ms/frames, excludes time for yoyo or repeats.
	 *
	 * @since 0.0.0
	 */
	Uint32 duration = 1000;

	/**
	 * The total calculated duration of this TweenData (based on duration, repeat,
	 * delay and yoyo)
	 *
	 * @since 0.0.0
	 */
	Uint32 totalDuration = 0;

	/**
	 * Time in ms/frames before tween will start.
	 *
	 * @since 0.0.0
	 */
	Uint32 delay = 0;

	/**
	 * Cause the tween to return back to its start value after hold has expired.
	 *
	 * @since 0.0.0
	 */
	bool yoyo = false;

	/**
	 * Time in ms/frames the tween will pause before running the yoyo or starting a
	 * repeat.
	 *
	 * @since 0.0.0
	 */
	Uint32 hold = 0;

	/**
	 * Number of times to repeat the tween. The tween will always run once
	 * regardless, so a repeat value of '1' will play the tween twice.
	 *
	 * @since 0.0.0
	 */
	int repeat = 0;

	/**
	 * Time in ms/frames before the repeat will start.
	 *
	 * @since 0.0.0
	 */
	Uint32 repeatDelay = 0;

	/**
	 * Automatically call toggleFlipX when the TweenData yoyos or repeats.
	 *
	 * @since 0.0.0
	 */
	bool flipX = false;

	/**
	 * Automatically call toggleFlipY when the TweenData yoyos or repeats.
	 *
	 * @since 0.0.0
	 */
	bool flipY = false;

	/**
	 * Between 0 and 1 showing completion of this TweenData.
	 *
	 * @since 0.0.0
	 */
	double progress = 0;

	/**
	 * Delta counter.
	 *
	 * @since 0.0.0
	 */
	Uint32 elapsed = 0;

	/**
	 * How many repeats are left to run?
	 *
	 * @since 0.0.0
	 */
	size_t repeatCounter = 0;

	/**
	 * Ease Value Data
	 *
	 * @since 0.0.0
	 */
	double start = 0;

	/**
	 * Ease Value Data
	 *
	 * @since 0.0.0
	 */
	double previous = 0;

	/**
	 * Ease Value Data
	 *
	 * @since 0.0.0
	 */
	double current = 0;

	/**
	 * Ease Value Data
	 *
	 * @since 0.0.0
	 */
	double end = 0;

	/**
	 * Time Duration 1
	 *
	 * @since 0.0.0
	 */
	Uint32 t1 = 0;

	/**
	 * Time Duration 2
	 *
	 * @since 0.0.0
	 */
	Uint32 t2 = 0;

	/**
	 * LoadValue generation functions
	 *
	 * @since 0.0.0
	 */
	TweenDataGenConfig gen;

	/**
	 * Initial state
	 *
	 * @since 0.0.0
	 */
	TWEEN state = TWEEN::CREATED;
};

}	// namespace Zen

#endif
