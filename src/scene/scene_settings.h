/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SCENES_SCENE_SETTINGS_H
#define ZEN_SCENES_SCENE_SETTINGS_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

// Forward declarations
#include "scene.fwd.h"

#include "../data.h"
#include "../const.h"
#include "../cameras/2d/camera_config.h"

namespace Zen {
namespace Scenes {

/**
 * The settings of a SceneSystems object.
 *
 * @class SceneSettings
 * @since 0.0.0
 */
class SceneSettings
{
public:
	/**
	 * @since 0.0.0
	 */
	SceneSettings (std::string key_, bool active_ = false, bool visible_ = true);

	SCENE status = SCENE::PENDING;

	std::string key;

	bool active = false;

	bool visible = true;

	bool isBooted = false;

	bool isTransition = false;

	Scene* transitionFrom = nullptr;

	Uint32 transitionDuration = 0;

	bool transitionAllowInput = true;

	Data data = {};

	std::vector<Cameras::Scene2D::CameraConfig> cameras;
};

}	// namespace Scenes
}	// namespace Zen

#endif
