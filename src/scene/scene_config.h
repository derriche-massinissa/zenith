/**
 * @file		scene_config.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef SCENE_CONFIG_H
#define SCENE_CONFIG_H

#include <string>
#include <memory>

// Forward declarations
#include "scene.fwd.h"

namespace Zen
{
	struct SceneConfig
	{
		std::string key;
		std::unique_ptr<Scene> scene;
		bool autoStart;
		Data data;

		SceneConfig (std::string k = "", std::unique_ptr<Scene> s = nullptr, bool a = false, Data d = {})
			: key(k)
			, scene(std::move(s))
			, autoStart(a)
			, data(d)
		{}
	};
}

// Declaration of previously forward declared elements
//#include "scene.h"

#endif
