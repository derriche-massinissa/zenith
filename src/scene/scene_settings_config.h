/**
 * @file		scene_settings_config.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef SCENE_SETTINGS_CONFIG_H
#define SCENE_SETTINGS_CONFIG_H

#include <string>

namespace Zen
{
	struct SceneSettingsConfig
	{
		std::string key;
		bool active;
		bool visible;
	};
}

#endif
