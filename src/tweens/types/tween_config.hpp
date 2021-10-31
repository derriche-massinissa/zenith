/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TWEENS_TWEEN_CONFIG_HPP
#define ZEN_TWEENS_TWEEN_CONFIG_HPP

#include "../../ecs/entity.hpp"
#include <SDL2/SDL_stdinc.h>
#include <functional>
#include <string>
#include "../tween/const.hpp"

namespace Zen {

struct TweenEntryConfig
{
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
	 * What the property will be set to immediately when this tween becomes active.
	 *
	 * @since 0.0.0
	 */
	std::function<double(Entity, double, size_t, size_t)> getActive = nullptr;

	/**
	 * What the property will be at the END of the Tween.
	 *
	 * @since 0.0.0
	 */
	std::function<double(Entity, double, size_t, size_t)> getEnd = nullptr;

	/**
	 * What the property will be at the START of the Tween.
	 *
	 * @since 0.0.0
	 */
	std::function<double(Entity, double, size_t, size_t)> getStart = nullptr;

	/**
	 * The ease function this tween uses. (Predifined function)
	 *
	 * @since 0.0.0
	 */
	TWEEN easing = TWEEN::LINEAR;

	/**
	 * A custom ease function to use for this tween.
	 *
	 * @since 0.0.0
	 */
	std::function<double(double)> ease = nullptr;

	/**
	 * Time in ms/frames before tween will start.
	 *
	 * @since 0.0.0
	 */
	Uint32 delay = 0;

	/**
	 * Duration of the tween in ms/frames.
	 *
	 * @since 0.0.0
	 */
	Uint32 duration = 0;

	/**
	 * Determines whether the tween should return back to its start value after
	 * hold has expired.
	 *
	 * @since 0.0.0
	 */
	bool yoyo = false;

	/**
	 * Time in ms/frames the tween will pause before repeating or returning to its
	 * starting value if yoyo is set to true.
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
	 * Should toggleFlipX be called when yoyo or repeat happens?
	 *
	 * @since 0.0.0
	 */
	bool flipX = false;

	/**
	 * Should toggleFlipY be called when yoyo or repeat happens?
	 *
	 * @since 0.0.0
	 */
	bool flipY = false;
};

/**
 * @struct TweenConfig
 * @since 0.0.0
 *
 * Example
 * TweenConfig config {
 *     .targets = {},
 *     .delay = 0,
 *     .duration = 1000,
 *     .ease = 'Power0',
 *     .hold = 0,
 *     .repeat = 0,
 *     .repeatDelay = 0,
 *     .yoyo = false,
 *     .flipX = false,
 *     .flipY = false
 * };
 */
struct TweenConfig
{
	/**
	 * A list of entities, to run the tween on.
	 *
	 * @since 0.0.0
	 */
	std::vector<Entity> targets {};

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
	 * The number of milliseconds to delay before the tween will start.
	 *
	 * @since 0.0.0
	 */
	Uint32 delay = 0;

	/**
	 * The duration of the tween in milliseconds.
	 *
	 * @since 0.0.0
	 */
	Uint32 duration = 1000;

	/**
	 * The predefined easing equation to use for the tween.
	 *
	 * @since 0.0.0
	 */
	TWEEN easing = TWEEN::LINEAR;

	/**
	 * A custom easing function to use for the tween.
	 *
	 * @since 0.0.0
	 */
	std::function<double(double)> ease = nullptr;

	/**
	 * The number of milliseconds to hold the tween for before yoyo'ing.
	 *
	 * @since 0.0.0
	 */
	Uint32 hold = 0;

	/**
	 * The number of times each property tween repeats.
	 *
	 * @since 0.0.0
	 */
	int repeat = 0;

	/**
	 * The number of milliseconds to pause before a repeat.
	 *
	 * @since 0.0.0
	 */
	Uint32 repeatDelay = 0;

	/**
	 * Should the tween complete, then reverse the values incrementally to get back
	 * to the starting tween values? The reverse tweening will also take `duration`
	 * milliseconds to complete.
	 *
	 * @since 0.0.0
	 */
	bool yoyo = false;

	/**
	 * Horizontally flip the target of the Tween when it completes (before it
	 * yoyos, if set to do so). Only works for targets that support the `flipX`
	 * property.
	 *
	 * @since 0.0.0
	 */
	bool flipX = false;

	/**
	 * Vertically flip the target of the Tween when it completes (before it yoyos,
	 * if set to do so). Only works for targets that support the `flipY` property.
	 *
	 * @since 0.0.0
	 */
	bool flipY = false;

	/**
	 * The time the tween will wait before the onComplete event is dispatched once
	 * it has completed, in ms.
	 *
	 * @since 0.0.0
	 */
	Uint32 completeDelay = 0;

	/**
	 * The number of times the tween will repeat. (A value of 1 means the tween
	 * will play twice, as it repeated once.) The first loop starts after every
	 * property tween has completed once.
	 *
	 * @since 0.0.0
	 */
	int loop = 0;

	/**
	 * The time the tween will pause before starting either a yoyo or returning to
	 * the start for a repeat.
	 *
	 * @since 0.0.0
	 */
	Uint32 loopDelay = 0;

	/**
	 * Does the tween start in a paused state (true) or playing (false)?
	 *
	 * @since 0.0.0
	 */
	bool paused = false;

	/**
	 * The tweening action.
	 *
	 * @since 0.0.0
	 */
	std::function<void(Entity, double)> action = [] (Entity, double) -> void {};

	/**
	 * A function to call when the tween completes.
	 *
	 * @since 0.0.0
	 */
	std::function<void(std::vector<Entity>)> onComplete = nullptr;

	/**
	 * A function to call each time the tween loops.
	 *
	 * @since 0.0.0
	 */
	std::function<void(std::vector<Entity>)> onLoop = nullptr;

	/**
	 * A function to call each time the tween repeats. Called once per entry per
	 * target.
	 *
	 * @since 0.0.0
	 */
	std::function<void(std::vector<Entity>)> onRepeat = nullptr;

	/**
	 * A function to call when the tween starts playback, after any delays have
	 * expired.
	 *
	 * @since 0.0.0
	 */
	std::function<void(std::vector<Entity>)> onStart = nullptr;

	/**
	 * A function to call when the tween is stopped.
	 *
	 * @since 0.0.0
	 */
	std::function<void(std::vector<Entity>)> onStop = nullptr;

	/**
	 * A function to call each time the tween steps. Called once per entry per
	 * target.
	 *
	 * @since 0.0.0
	 */
	std::function<void(std::vector<Entity>)> onUpdate = nullptr;

	/**
	 * A function to call each time the tween yoyos. Called once per entry per
	 * target.
	 *
	 * @since 0.0.0
	 */
	std::function<void(std::vector<Entity>)> onYoyo = nullptr;

	/**
	 * A function to call when the tween becomes active within the TweenManager.
	 *
	 * @since 0.0.0
	 */
	std::function<void(std::vector<Entity>)> onActive = nullptr;

	/**
	 * @since 0.0.0
	 */
	std::vector<TweenEntryConfig> entries {};
};

}	// namespace Zen

#endif
