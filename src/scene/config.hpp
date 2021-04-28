/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SCENE_CONFIG_HPP
#define ZEN_SCENE_CONFIG_HPP

#include <string>
#include <memory>
#include "../data.h"
#include "scene.fwd.hpp"

namespace Zen {

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

	SceneConfig ();

	SceneConfig (
			std::string key_,
			std::unique_ptr<Scene> scene_,
			bool autoStart_ = false,
			Data data_ = {});

	SceneConfig (
			std::string key_,
			std::unique_ptr<Scene>& scene_,
			bool autoStart_ = false,
			Data data_ = {});
};

}	// namespace Zen

#endif
