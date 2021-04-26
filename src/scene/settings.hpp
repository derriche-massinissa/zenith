/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SCENES_SCENE_SETTINGS_HPP
#define ZEN_SCENES_SCENE_SETTINGS_HPP

#include <SDL2/SDL_stdinc.h>
#include <string>
#include "../data.h"
#include "../const.h"

#include "scene.fwd.hpp"

namespace Zen {

/**
 * The settings of a SceneSystems object.
 *
 * @class SceneSettings
 * @since 0.0.0
 */
struct SceneSettings
{
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
};

}	// namespace Zen

#endif
