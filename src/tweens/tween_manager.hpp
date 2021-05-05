/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_TWEENS_TWEENMANAGER_HPP
#define ZEN_TWEENS_TWEENMANAGER_HPP

#include <vector>
#include <deque>
#include <memory>
#include "../ecs/entity.hpp"
#include "../tweens/tween/tween.hpp"
#include "../scene/scene.fwd.hpp"

namespace Zen {

class TweenManager
{
private:
	/**
	 * the scene this TweenManager belongs to.
	 *
	 * @since 0.0.0
	 */
	Scene* scene = nullptr;

	/**
	 * The time scale of the TweenManager.
	 *
	 * This value scales the time delta between two frames, thus influencing the
	 * speed of time for all Tweens owned by this TweenManager.
	 *
	 * @since 0.0.0
	 */
	double timeScale = 1.;

	/**
	 * An array of Tweens which will be added to the Tween Manager at the start of
	 * the frame.
	 *
	 * @since 0.0.0
	 */
	std::deque<std::shared_ptr<Tween>> toAdd;

	/**
	 * An list of Tweens which will be removed from the TweenManager at the start
	 * of the frame.
	 *
	 * @since 0.0.0
	 */
	std::deque<std::shared_ptr<Tween>> toRemove;

	/**
	 * An array of Tweens pending to be later added to the TweenManager.
	 *
	 * @since 0.0.0
	 */
	std::deque<std::shared_ptr<Tween>> pending;

	/**
	 * An list of Tweens which are still incomplete and are actively processed by
	 * the TweenManager.
	 *
	 * @since 0.0.0
	 */
	std::deque<std::shared_ptr<Tween>> active;

	/**
	 * The number of Tweens which need to be processed by the TweenManager at the
	 * start of the frame.
	 *
	 * @since 0.0.0
	 */
	size_t toProcess = 0;

public:
	/**
	 * @since 0.0.0
	 */
	TweenManager (Scene *scene);

    /**
     * This method is called automatically by the Scene when it is starting up.
     * It is responsible for creating local systems, properties and listening for
	 * Scene events.
     * Do not invoke it directly.
     *
     * @since 0.0.0
     */
	void start ();

    /**
     * Create a Tween and add it to the active Tween list.
     *
     * @since 0.0.0
     *
     * @param config The configuration object for the Tween.
     *
     * @return The created Tween.
     */
	Tween* add (TweenConfig config);

    /**
     * Updates the TweenManager's internal lists at the start of the frame.
     *
     * This method will return immediately if no changes have been indicated.
     *
     * @since 0.0.0
     */
	void preUpdate (Uint32 time, Uint32 delta);

    /**
     * Updates all Tweens and Timelines of the Tween Manager.
     *
     * @since 0.0.0
     *
     * @param time The current time in milliseconds.
     * @param delta The delta time in ms since the last frame.
     */
	void update (Uint32 time, Uint32 delta);

    /**
     * Removes the given tween from the Tween Manager, regardless of its state
	 * (pending or active).
     *
     * @since 0.0.0
     *
     * @param tween A pointer to the Tween to be removed.
     */
	void remove (Tween *tween);

    /**
     * Checks if a Tween or Timeline is active and adds it to the Tween Manager at
	 * the start of the frame if it isn't.
     *
     * @since 0.0.0
     *
     * @param tween - The Tween to check.
     */
	void makeActive (Tween *tween);

    /**
     * Returns an array of all Tweens or Timelines in the Tween Manager which
	 * affect the given target or array of targets.
     *
     * Only the currently active tweens are tested. A tween that has completed and
	 * is awaiting removal will not be included in the results.
     *
     * If you wish to also search pending tweens, use the `includePending` flag.
     *
     * @since 0.0.0
     *
     * @param target The target to look for.
     * @param includePending Also check for pending tweens, not just active ones?
     *
     * @return A new array containing all Tweens and Timelines which affect the
	 * given target.
     */
	std::vector<Tween*> getTweensOf (Entity target, bool includePending = false);

    /**
     * Checks if the given entity is being affected by a playing Tween.
     *
     * @since 0.0.0
     *
     * @param entity Entity to check for
     *
     * @return returns if target tween object is active or not
     */
	bool isTweening (Entity entity);

    /**
     * Stops all Tweens in this Tween Manager. They will be removed at the start of
	 * the frame.
     *
     * @since 0.0.0
     */
    void killAll ();

    /**
     * Stops all Tweens which affect the given target. The Tweens will be removed
	 * from the Tween Manager at the start of the frame.
     *
     * @since 0.0.0
     *
     * @param target The target to look for.
     */
    void killTweensOf (Entity target);


    /**
     * Pauses all Tweens in this TweenManager.
     *
     * @since 0.0.0
     */
	void pauseAll ();

    /**
     * Resumes all Tweens in this TweenManager.
     *
     * @since 0.0.0
     */
	void resumeAll ();

    /**
     * Sets a new scale of the time delta for this Tween Manager.
     *
     * The time delta is the time elapsed between two consecutive frames and
	 * influences the speed of time for this TweenManager and all Tweens it owns.
	 *
	 * Values higher than 1 increase the speed of time, while values smaller than 1
	 * decrease it. A value of 0 freezes time and is effectively equivalent to
	 * pausing all Tweens.
     *
     * @since 0.0.0
     *
     * @param value The new scale of the time delta, where 1 is the normal speed.
     */
    void setGlobalTimeScale (double value);
};

}	// namespace Zen

#endif
