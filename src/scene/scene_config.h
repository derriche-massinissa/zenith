/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SCENES_SCENE_CONFIG_H
#define ZEN_SCENES_SCENE_CONFIG_H

#include <string>
#include <memory>

// Forward declarations
#include "scene.fwd.h"

namespace Zen {
namespace Scenes {

/**
 * A structure to store scenes and data about them, before adding them to
 * the manager.
 *
 * @struct SceneConfig
 * @since 0.0.0
 */
struct SceneConfig
{
	std::string key;
	std::unique_ptr<Scene> scene;
	bool autoStart;
	Data data;

	SceneConfig (
			std::string key_ = "",
			std::unique_ptr<Scene> scene_ = nullptr,
			bool autoStart_ = false,
			Data data_ = {})
		: key (key_)
		, scene (std::move(scene_))
		, autoStart (autoStart_)
		, data (data_)
		{}
};

}	// namespace Scenes
}	// namespace Zen

#endif
