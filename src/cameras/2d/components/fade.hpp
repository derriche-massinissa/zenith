/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_CAMERAS_SCENE2D_COMPONENTS_FADE_HPP
#define ZEN_CAMERAS_SCENE2D_COMPONENTS_FADE_HPP

#include "../../../ecs/entity.hpp"

namespace Zen {
namespace Components {

/**
 * Camera fade effect.
 *
 * This effect will fade the camera viewport to the given color, over the duration
 * specified.
 *
 * Only the camera viewport is faded. None of the objects it is displaying are
 * impacted, i.e. their colors do not change.
 *
 * The effect will dispatch several events on the Camera itself and you can also
 * specify an `onUpdate` callback, which is invoked each frame for the duration of
 * the effect, if required.
 *
 * @struct EffectFade
 * @since 0.0.0
 */
struct EffectFade
{
	/**
	 * The camera this effect belongs to.
	 *
	 * @since 0.0.0
	 */
	Entity camera = entt::null;

	/**
	 * Is the effect actively running?
	 *
	 * @since 0.0.0
	 */
	bool isRunning = false;

	/**
	 * Has this effect finished running?
	 *
	 * @since 0.0.0
	 */
	bool isComplete = false;

	/**
	 * The direction of the fade.
	 * `true` = fade out (transparent to color)
	 * `false` = fade in (color to transparent)
	 *
	 * @since 0.0.0
	 */
	bool direction = true;

	/**
	 * The duratioon of the effect, in milliseconds.
	 *
	 * @since 0.0.0
	 */
	double duration = 0;

	/**
	 * The value of the red color channel the camera will use for the fade effect.
	 * A value between 0 and 255.
	 *
	 * @since 0.0.0
	 */
	int red = 0;

	/**
	 * The value of the green color channel the camera will use for the fade effect.
	 * A value between 0 and 255.
	 *
	 * @since 0.0.0
	 */
	int green = 0;

	/**
	 * The value of the blue color channel the camera will use for the fade effect.
	 * A value between 0 and 255.
	 *
	 * @since 0.0.0
	 */
	int blue = 0;

	/**
	 * The value of the alpha channel used during the fade effect.
	 * A value between 0 and 1.
	 *
	 * @since 0.0.0
	 */
	double alpha = 0;

	/**
	 * If this effect is running this holds the current percentage of the progress,
	 * a value between 0 and 1.
	 *
	 * @since 0.0.0
	 */
	double progress = 0;

	/**
	 * Effect elapsed timer, in milliseconds.
	 *
	 * @since 0.0.0
	 */
	int elapsed = 0;

	/**
	 * This callback is invoked every frame for the duration of the effect.
     * It is sent two arguments: A reference to the camera and a progress amount
	 * between 0 and 1 indicating how complete the effect is.
	 *
	 * @since 0.0.0
	 */
	std::function<void(Entity, double)> onUpdate = nullptr;
};

}	// namespace Components
}	// namespace Zen

#endif
