/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SCENES_SCENE_TRANSITION_CONFIG_H
#define ZEN_SCENES_SCENE_TRANSITION_CONFIG_H

#include <functional>
#include <string>
#include "../data.h"

namespace Zen {
namespace Scenes {

/**
 * @struct SceneTransitionConfig
 * @since 0.0.0
 *
 * @property target The Scene key to transition to.
 * @property duration The duration, in ms, for the transition to last.
 * @property sleep Will the Scene responsible for the transition be sent to sleep 
 * on completion (`true`), or stopped? (`false`)
 * @property remove Will the Scene responsible for the transition be removed from 
 * the Scene Manager after the transition completes?
 * @property allowInput Will the Scenes Input system be able to process events 
 * while it is transitioning in or out?
 * @property moveAbove Move the target Scene to be above this one before the 
 * transition starts.
 * @property moveBelow Move the target Scene to be below this one before the 
 * transition starts.
 * @property onUpdate This callback is invoked every frame for the duration of the 
 * transition.
 * @property onUpdateScope The context in which the callback is invoked.
 * @property data An object containing any data you wish to be passed to the target 
 * scene's init / create methods (if sleep is false) or to the target scene's wake 
 * event callback (if sleep is true).
 */
struct SceneTransitionConfig
{
	std::string target;

	Uint32 duration = 1000;

	bool sleep = false;

	bool remove = false;

	bool allowInput = false;

	bool moveAbove = false;

	bool moveBelow = false;
	
	std::function<void(double)> onUpdate = nullptr;

	void (Scene::* onUpdate) (double) = nullptr;

	Scene* onUpdateScope = nullptr;
	
	Data data;
};

}	// namespace Scenes
}	// namespace Zen

#endif
