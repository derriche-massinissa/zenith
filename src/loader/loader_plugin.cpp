/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "loader_plugin.h"

#include <fstream>
#include "json/json.hpp"

#include "../utils/messages.hpp"
#include "../event/event_emitter.hpp"

#include "../texture/texture_manager.hpp"
#include "../scene/scene.h"
#include "../scene/scene_manager.h"
#include "../cameras/2d/camera_manager.hpp"
#include "../core/config.hpp"

namespace Zen {
namespace Loader {

LoaderPlugin::LoaderPlugin (Scene* scene_)
	: scene (scene_)
	, textureManager (scene_->textures)
	, sceneManager (scene_->game.scene)
	, prefix (scene_->game.config.loaderPrefix)
	, path (scene_->game.config.loaderPath)
{}

LoaderPlugin::~LoaderPlugin ()
{
	shutdown();
}

void LoaderPlugin::pluginStart ()
{
	lShutdown = scene->sys.events.once("shutdown", &LoaderPlugin::shutdown, this);
}

LoaderPlugin& LoaderPlugin::setPath (std::string path_)
{
	if (!path_.empty() && path_.back() != '/')
		path_.append("/");

	path = path_;

	return *this;
}

LoaderPlugin& LoaderPlugin::setPrefix (std::string prefix_)
{
	prefix = prefix_;

	return *this;
}

LoaderPlugin& LoaderPlugin::image (std::string key_, std::string path_)
{
	textureManager.addImage(key_, path_);

	return *this;
}

LoaderPlugin& LoaderPlugin::atlas (std::string key_, std::string texturePath_, std::string atlasPath_)
{
	textureManager.addAtlas(key_, texturePath_, atlasPath_);

	return *this;
}

LoaderPlugin& LoaderPlugin::multiatlas (std::string key_, std::string atlasPath_, std::string path_)
{
	std::vector<std::string> sources_;

	// Open data file
	std::fstream file_ (atlasPath_, std::ios::in);

	if (!file_)
	{
		MessageError("Atlas file failed to open: ", atlasPath_);
		return *this;
	}

	// Create a JSON object
	nlohmann::json data_;
	file_ >> data_;

	// Close file
	file_.close();

	for (const auto& textureFile_ : data_["textures"])
		sources_.emplace_back(path_ + textureFile_["image"].get<std::string>());

	textureManager.addAtlas(key_, sources_, atlasPath_);

	return *this;
}

LoaderPlugin& LoaderPlugin::spritesheet (std::string key_, std::string path_, SpriteSheetConfig config_)
{
	textureManager.addSpriteSheet(key_, path_, config_);

	return *this;
}

LoaderPlugin& LoaderPlugin::audio (std::string key_, std::string path_)
{
	MessageWarning("Audio is not yet implemented!");

	return *this;
}

LoaderPlugin& LoaderPlugin::font (std::string key_, std::string path_)
{
	MessageWarning("Fonts and text are not yet implemented!");

	return *this;
}

void LoaderPlugin::reset ()
{
	setPath(scene->game.config.loaderPath);
	setPrefix(scene->game.config.loaderPrefix);
}

void LoaderPlugin::shutdown ()
{
	scene->sys.events.off(lShutdown);
}

}	// namespace Loader
}	// namespace Zen
