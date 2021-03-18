/**
 * @file		scene_transition_config.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef SCENE_TRANSITION_CONFIG_H
#define SCENE_TRANSITION_CONFIG_H

#include <functional>
#include <string>
#include "../data.h"

namespace Zen
{
	struct SceneTransitionConfig
	{
		std::string target;
		Uint32 duration;
		bool sleep;
		bool remove;
		bool allowInput;
		bool moveAbove;
		bool moveBelow;
		std::function<void(Data)> onUpdate;
		Data data;
	};
}

#endif
