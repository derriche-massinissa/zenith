/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "scene_settings.h"

namespace Zen {
namespace Scenes {

SceneSettings::SceneSettings (std::string key_, bool active_, bool visible_)
	: key (key_), active (active_), visible (visible_)
{}

}	// namespace Scenes
}	// namespace Zen
