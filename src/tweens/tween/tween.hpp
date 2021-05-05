/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TWEEN_HPP
#define ZEN_TWEEN_HPP

#include "../../ecs/entity.hpp"
#include <SDL2/SDL_stdinc.h>
#include <functional>
#include <vector>
#include <string>
#include "const.hpp"
#include "../../event/event_emitter.hpp"
#include "../types/tween_config.hpp"
#include "tween_data.hpp"

namespace Zen {

class Tween : public EventEmitter
{
public:
	/**
	 * An array of TweenData objects, each containing a unique action and target
	 * being tweened.
	 *
	 * @since 0.0.0
	 */
	std::vector<TweenData> data;

	/**
	 * An array of target/s this Tween is operating on.
	 *
	 * @since 0.0.0
	 */
	std::vector<Entity> targets;

	/**
	 * Scales the time applied to this Tween. A value of 1 runs in real-time. A
	 * value of 0.5 runs 50% slower, and so on.
	 * Value isn't used when calculating total duration of the tween, it's a
	 * run-time delta adjustment only.
	 *
	 * @since 0.0.0
	 */
	double timeScale = 1.;

	/**
	 * Loop this tween? Can be -1 for an infinite loop, or an integer.
	 * When enabled it will play through ALL TweenDatas again. Use TweenData.repeat
	 * to loop a single element.
	 *
	 * @since 0.0.0
	 */
	int loop = 0;

	/**
	 * Time in ms/frames before the tween loops.
	 *
	 * @since 0.0.0
	 */
	Uint32 loopDelay = 0;

	/**
	 * How many loops are left to run?
	 *
	 * @since 0.0.0
	 */
	size_t loopCounter = 0;

	/**
	 * Time in ms/frames before the 'onStart' event fires.
	 * This is the shortest `delay` value across all of the TweenDatas of this
	 * Tween.
	 *
	 * @since 0.0.0
	 */
	Uint32 startDelay = 0;

	/**
	 * Has this Tween started playback yet?
	 * This boolean is toggled when the Tween leaves the 'delayed' state and starts
	 * running.
	 *
	 * @since 0.0.0
	 */
	bool hasStarted = false;

	/**
	 * Is this Tween currently seeking?
	 * This boolean is toggled in the `Tween.seek` method.
	 * When a tween is seeking it will not dispatch any events or callbacks.
	 *
	 * @since 0.0.0
	 */
	bool isSeeking = false;

	/**
	 * Time in ms/frames before the 'onComplete' event fires. This never fires if
	 * loop = -1 (as it never completes)
	 *
	 * @since 0.0.0
	 */
	Uint32 completeDelay = 0;

	/**
	 * Countdown timer (used by timeline offset, loopDelay and completeDelay)
	 *
	 * @since 0.0.0
	 */
	Uint32 countdown = 0;

	/**
	 * Set only if this Tween is part of a Timeline.
	 *
	 * @since 0.0.0
	 */
	Uint32 offset = 0;

	/**
	 * Set only if this Tween is part of a Timeline. The calculated offset amount.
	 *
	 * @since 0.0.0
	 */
	Uint32 calculatedOffset = 0;

	/**
	 * The current state of the tween
	 *
	 * @since 0.0.0
	 */
	TWEEN state = TWEEN::PENDING_ADD;

	/**
	 * The state of the tween when it was paused (used by Resume)
	 *
	 * @since 0.0.0
	 */
	TWEEN pausedState = TWEEN::INIT;

	/**
	 * Does the Tween start off paused? (if so it needs to be started with
	 * Tween.play)
	 *
	 * @since 0.0.0
	 */
	bool paused = false;

	/**
	 * Elapsed time in ms/frames of this run through the Tween.
	 *
	 * @since 0.0.0
	 */
	Uint32 elapsed = 0;

	/**
	 * Total elapsed time in ms/frames of the entire Tween, including looping.
	 *
	 * @since 0.0.0
	 */
	Uint32 totalElapsed = 0;

	/**
	 * Time in ms/frames for the whole Tween to play through once, excluding loop amounts and loop delays.
	 *
	 * @since 0.0.0
	 */
	Uint32 duration = 0;

	/**
	 * Value between 0 and 1. The amount through the Tween, excluding loops.
	 *
	 * @since 0.0.0
	 */
	double progress = 0;

	/**
	 * Time in ms/frames for the Tween to complete (including looping)
	 *
	 * @since 0.0.0
	 */
	Uint32 totalDuration = 0;

	/**
	 * Value between 0 and 1. The amount through the entire Tween, including
	 * looping.
	 *
	 * @since 0.0.0
	 */
	double totalProgress = 0;

	/**
	 * The different callbacks of this tween.
	 *
	 * @since 0.0.0
	 */
	struct {
		/**
         * When the Tween is moved from the pending to the active list, even if
		 * playback paused.
		 *
		 * @since 0.0.0
		 */
		std::function<void(std::vector<Entity>)> onActive = nullptr;

		/**
         * When the Tween finishes playback fully. Never invoked if tween is set to
		 * repeat infinitely.
		 *
		 * @since 0.0.0
		 */
		std::function<void(std::vector<Entity>)> onComplete = nullptr;

		/**
         * When a Tween loops. This happens _after_ the `loopDelay` expires, if set.
		 *
		 * @since 0.0.0
		 */
		std::function<void(std::vector<Entity>)> onLoop = nullptr;

		/**
         * When a TweenData repeats playback. This happens _after_ the
		 * `repeatDelay` expires, if set.
		 *
		 * @since 0.0.0
		 */
		std::function<void(std::vector<Entity>)> onRepeat = nullptr;

		/**
         * When the Tween starts playing after a delayed state. Will happen at the
		 * same time as `onActive` if it has no delay.
		 *
		 * @since 0.0.0
		 */
		std::function<void(std::vector<Entity>)> onStart = nullptr;

		/**
		 * When a TweenData is stopped.
		 *
		 * @since 0.0.0
		 */
		std::function<void(std::vector<Entity>)> onStop = nullptr;

		/**
         * When a TweenData updates a property on a source target during playback.
		 *
		 * @since 0.0.0
		 */
		std::function<void(std::vector<Entity>)> onUpdate = nullptr;

		/**
         * When a TweenData starts a yoyo. This happens _after_ the `hold` delay
		 * expires, if set.
		 *
		 * @since 0.0.0
		 */
		std::function<void(std::vector<Entity>)> onYoyo = nullptr;
	} callbacks;

	/**
	 * @since 0.0.0
	 */
	Tween (TweenConfig config);

    /**
     * Returns the current value of the specified Tween Data.
     *
     * @since 0.0.0
     *
     * @param index The TweenData to return the value from.
     *
     * @return The value of the requested Tween Data.
     */
	double getValue (int index = 0);

    /**
     * Set the scale the time applied to this Tween. A value of 1 runs in
	 * real-time. A value of 0.5 runs 50% slower, and so on.
     *
     * @since 0.0.0
     *
     * @param value The scale factor for timescale.
     *
     * @return This Tween instance.
     */
    Tween& setTimeScale (double value);

    /**
     * Returns the scale of the time applied to this Tween.
     *
     * @since 0.0.0
     *
     * @return The timescale of this tween (between 0 and 1)
     */
    double getTimeScale ();

    /**
     * Checks if the Tween is currently active.
     *
     * @since 0.0.0
     *
     * @return `true` if the Tween is active, otherwise `false`.
     */
    bool isPlaying ();

    /**
     * Checks if the Tween is currently paused.
     *
     * @since 0.0.0
     *
     * @return `true` if the Tween is paused, otherwise `false`.
     */
    bool isPaused ();

    /**
     * See if this Tween is currently acting upon the given target.
     *
     * @since 0.0.0
     *
     * @param target The target to check against this Tween.
     *
     * @return `true` if the given target is a target of this Tween, otherwise
	 * `false`.
     */
    bool hasTarget (Entity target);

    /**
     * Updates the 'end' value of the tween action across all matching targets.
     *
     * Calling this does not adjust the duration of the tween, or the current
	 * progress.
     *
     * You can optionally tell it to set the 'start' value to be the current value
	 * (before the change).
     *
     * @since 0.0.0
     *
     * @param value The new value of the action.
     * @param startToCurrent Should this change set the start value to be the
	 * current value?
     *
     * @return This Tween instance.
     */
    Tween& updateTo (double value, bool startToCurrent = false);

    /**
     * Restarts the tween from the beginning.
     *
     * @since 0.0.0
     *
     * @return This Tween instance.
     */
    Tween& restart ();

    /**
     * Internal method that calculates the overall duration of the Tween.
     *
     * @since 0.0.0
     */
    void calcDuration ();

    /**
     * Called by TweenManager.preUpdate as part of its loop to check pending and
	 * active tweens.
     * Should not be called directly.
     *
     * @since 0.0.0
     *
     * @return `true` if this Tween should be moved from the pending list to the
	 * active list by the Tween Manager.
     */
    bool init ();

    /**
     * Internal method that makes this Tween active within the TweenManager
     * and emits the onActive event and callback.
     *
     * @since 0.0.0
     */
    void makeActive ();

    /**
     * Internal method that advances to the next state of the Tween during playback.
     *
     * @since 0.0.0
     */
    void nextState ();

    /**
     * Pauses the Tween immediately. Use `resume` to continue playback.
     *
     * @since 0.0.0
     *
     * @return This Tween instance.
     */
    Tween& pause ();

    /**
     * Starts a Tween playing.
     *
     * You only need to call this method if you have configured the tween to be
	 * paused on creation.
     *
     * If the Tween is already playing, calling this method again will have no
	 * effect. If you wish to restart the Tween, use Tween::restart instead.
     *
     * Calling this method after the Tween has completed will start the Tween
	 * playing again from the start. This is the same as calling Tween::seek(0)
	 * and then Tween::play().
     *
     * @since 0.0.0
     *
     * @return This Tween instance.
     */
    Tween& play ();

    /**
     * Internal method that resets all of the TweenData, including the progress
	 * and elapsed values.
     *
     * @since 0.0.0
     *
     * @param resetFromLoop Has this method been called as part of a loop?
     */
    void resetTweenData (bool resetFromLoop);

    /**
     * Resumes the playback of a previously paused Tween.
     *
     * @since 0.0.0
     *
     * @return This Tween instance.
     */
    Tween& resume ();

    /**
     * Seeks to a specific point in the Tween.
     *
     * **Note:** Be careful when seeking a Tween that repeats or loops forever,
     * or that has an unusually long total duration, as it can be __very__ slow.
     *
     * The given position is a value between 0 and 1 which represents how far
	 * through the Tween to seek to. A value of 0.5 would seek to half-way through
	 * the Tween, where-as a value of zero would seek to the start.
     *
     * Note that the seek takes the entire duration of the Tween into account,
	 * including delays, loops and repeats. For example, a Tween that lasts for 2
	 * seconds, but that loops 3 times, would have a total duration of 6 seconds,
	 * so seeking to 0.5 would seek to 3 seconds into the Tween, as that's half-way
	 * through its _entire_ duration.
     *
     * Seeking works by resetting the Tween to its initial values and then
	 * iterating through the Tween at `delta` jumps per step. The longer the Tween,
	 * the longer this can take.
     *
     * @since 0.0.0
     *
     * @param toPosition A value between 0 and 1 which represents the progress
	 * point to seek to.
     * @param delta The size of each step when seeking through the Tween. A
	 * higher value completes faster but at a cost of less precision.
     *
     * @return This Tween instance.
     */
    Tween& seek (double toPosition, double delta = 16.6);

    /**
     * Flags the Tween as being complete, whatever stage of progress it is at.
     *
     * If an onComplete callback has been defined it will automatically invoke it,
	 * unless a `delay` argument is provided, in which case the Tween will delay
	 * for that period of time before calling the callback.
     *
     * If you don't need a delay, or have an onComplete callback, then call
	 * `Tween::stop` instead.
     *
     * @since 0.0.0
     *
     * @param delay The time to wait before invoking the complete callback. If zero
	 * it will fire immediately.
     *
     * @return This Tween instance.
     */
    Tween& complete (Uint32 delay = 0);

    /**
     * Immediately removes this Tween from the TweenManager and all of its internal
	 * arrays, no matter what stage it as it. Then sets the tween state to
	 * `REMOVED`.
     *
     * @since 0.0.0
     *
     * @return This Tween instance.
     */
    Tween& remove ();

    /**
     * Stops the Tween immediately, whatever stage of progress it is at and flags
	 * it for removal by the TweenManager.
     *
     * @since 0.0.0
     *
     * @param resetTo If you want to seek the tween provide a value between 0 and 1.
     *
     * @return This Tween instance.
     */
    Tween& stop (double resetTo = -1);

    /**
     * Internal method that advances the Tween based on the time values.
     *
     * @since 0.0.0
     *
     * @param timestamp The current time, since SDL started.
     * @param delta The delta time in ms since the last frame.
     *
     * @return `true` if this Tween has finished and should be removed from the
	 * TweenManager, otherwise returns `false`.
     */
	bool update (Uint32 time, Uint32 delta);

    /**
     * Internal method that will emit a TweenData based Event and invoke the given
	 * callback.
     *
     * @since 0.0.0
     *
     * @param event The Event to be dispatched.
     * @param callback The callback to be invoked. Can be `nullptr` to skip
	 * invocation.
     * @param tweenData The TweenData object that caused this event.
     */
	void dispatchTweenDataEvent (std::string event,
			std::function<void(std::vector<Entity>)> callback,
			TweenData& tweenData);

    /**
     * Internal method that will emit a Tween based Event and invoke the given
	 * callback.
     *
     * @since 0.0.0
     *
     * @param event The Event to be dispatched.
     * @param callback The callback to be invoked. Can be `nullptr` to skip
	 * invocation.
     */
	void dispatchTweenEvent (std::string event,
			std::function<void(std::vector<Entity>)> callback = nullptr);

    /**
     * Internal method used as part of the playback process that sets a tween to
	 * play in reverse.
     *
     * @since 0.0.0
     *
     * @param tweenData The TweenData property to update.
     * @param diff Any extra time that needs to be accounted for in the elapsed and
	 * progress values.
     *
     * @return The state of this Tween.
     */
	TWEEN setStateFromEnd (TweenData *tweenData, Uint32 diff);

    /**
     * Internal method used as part of the playback process that sets a tween to
	 * play from the start.
     *
     * @since 0.0.0
     *
     * @param tweenData The TweenData property to update.
     * @param diff Any extra time that needs to be accounted for in the elapsed and
	 * progress values.
     *
     * @return The state of this Tween.
     */
    TWEEN setStateFromStart (TweenData *tweenData, Uint32 diff);

    /**
     * Internal method that advances the TweenData based on the time value given.
     *
     * @since 0.0.0
     *
     * @param tweenData The TweenData property to update.
     * @param delta Either a value in ms, or 1 if Tween.useFrames is true.
     *
     * @return `true` if the tween is not complete (e.g., playing), or `false` if
	 * the tween is complete.
     */
    bool updateTweenData (TweenData *tweenData, Uint32 delta);
};

}	// namespace Zen

#endif
