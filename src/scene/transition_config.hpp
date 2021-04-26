/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SCENE_TRANSITION_CONFIG_HPP
#define ZEN_SCENE_TRANSITION_CONFIG_HPP

#include <SDL2/SDL_types.h>
#include <string>
#include "../data.h"
#include "scene.fwd.hpp"

namespace Zen {

/**
 * @struct SceneTransitionConfig
 * @since 0.0.0
 */
struct SceneTransitionConfig
{
	/**
	 * @property target The Scene key to transition to.
	 *
	 * @since 0.0.0
	 */
	std::string target;

	/**
	 * @property duration The duration, in ms, for the transition to last.
	 *
	 * @since 0.0.0
	 */
	Uint32 duration = 1000;

	/**
	 * @property sleep Will the Scene responsible for the transition be sent to sleep
	 * on completion (`true`), or stopped? (`false`)
	 *
	 * @since 0.0.0
	 */
	bool sleep = false;

	/**
	 * @property remove Will the Scene responsible for the transition be removed from
	 * the SceneManager after the transition completes?
	 *
	 * @since 0.0.0
	 */
	bool remove = false;

	/**
	 * @property allowInput Will the Scenes Input system be able to process events
	 * while it is transitioning in or out?
	 *
	 * @since 0.0.0
	 */
	bool allowInput = false;

	/**
	 * @property moveAbove Move the target Scene to be above this one before the
	 * transition starts.
	 *
	 * @since 0.0.0
	 */
	bool moveAbove = false;

	/**
	 * @property moveBelow Move the target Scene to be below this one before the
	 * transition starts.
	 *
	 * @since 0.0.0
	 */
	bool moveBelow = false;

	/**
	 * @property onUpdate This callback is invoked every frame for the duration of the
	 * transition.
	 *
	 * @since 0.0.0
	 */
	void (Scene::* onUpdate) (double) = nullptr;

	/**
	 * @property onUpdateScope The context in which the callback is invoked.
	 *
	 * @since 0.0.0
	 */
	Scene* onUpdateScope = nullptr;

	/**
	 * @property data An object containing any data you wish to be passed to the target
	 * scene's init / create methods (if sleep is false) or to the target scene's wake
	 * event callback (if sleep is true).
	 *
	 * @since 0.0.0
	 */
	Data data;
};

}	// namespace Zen

#endif
