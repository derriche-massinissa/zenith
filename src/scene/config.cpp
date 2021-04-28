/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "config.hpp"

#include "scene.hpp"

namespace Zen {

SceneConfig::SceneConfig ()
	: key ("")
	, scene (nullptr)
	, autoStart (false)
	, data ({})
	{}

SceneConfig::SceneConfig (
		std::string key_,
		std::unique_ptr<Scene> scene_,
		bool autoStart_,
		Data data_)
	: key (key_)
	, scene (std::move(scene_))
	, autoStart (autoStart_)
	, data (data_)
	{}

SceneConfig::SceneConfig (
		std::string key_,
		std::unique_ptr<Scene>& scene_,
		bool autoStart_,
		Data data_)
	: key (key_)
	, scene (std::move(scene_))
	, autoStart (autoStart_)
	, data (data_)
	{}

}	// namespace Zen
