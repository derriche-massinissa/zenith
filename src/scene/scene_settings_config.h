/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SCENES_SCENE_SETTINGS_CONFIG_H
#define ZEN_SCENES_SCENE_SETTINGS_CONFIG_H

#include <string>

namespace Zen {
namespace Scenes {

struct SceneSettingsConfig
{
	std::string key;
	bool active;
	bool visible;
};

}	// namespace Scenes
}	// namespace Zen

#endif
