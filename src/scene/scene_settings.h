/**
 * @file		scene_settings.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef SCENE_SETTINGS_H
#define SCENE_SETTINGS_H

#include <SDL2/SDL.h>
#include <memory>
#include <string>

// Forward declarations
#include "scene.fwd.h"

#include "../data.h"
#include "../const.h"

namespace Zen
{
	class SceneSettings
	{
	public:
		SceneSettings (std::string k, bool a = true, bool v = true);
		~SceneSettings ();

		SCENE status;

		std::string key;

		bool active;

		bool visible;

		bool isBooted;

		bool isTransition;

		Scene* transitionFrom;

		Uint32 transitionDuration;

		bool transitionAllowInput;

		Data data = {};
	};
}

// Declarations of the forward declared elements
//#include "scene.h"

#endif
