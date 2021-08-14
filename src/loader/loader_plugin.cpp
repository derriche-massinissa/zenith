/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "loader_plugin.hpp"

#include <fstream>
#include "json/json.hpp"

#include "../utils/messages.hpp"
#include "../event/event_emitter.hpp"

#include "../texture/texture_manager.hpp"
#include "../scene/scene.hpp"
#include "../scene/scene_manager.hpp"
#include "../cameras/2d/camera_manager.hpp"
#include "../core/config.hpp"
#include "../audio/audio_manager.hpp"
#include "../input/input_manager.hpp"
#include "../text/text_manager.hpp"

namespace Zen {

extern GameConfig *g_config;
extern AudioManager g_audio;
extern TextManager g_text;
extern InputManager g_input;
extern TextureManager g_texture;
extern SceneManager g_scene;

LoaderPlugin::LoaderPlugin (Scene* scene_)
	: scene (scene_)
	, prefix (g_config->loaderPrefix)
	, path (g_config->loaderPath)
{}

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

LoaderPlugin& LoaderPlugin::image (std::string key_, std::string path_, bool alphaCache_)
{
	path_ = path + path_;

	g_texture.addImage(key_, path_);

	if (alphaCache_)
		g_texture.createAlphaCache(key_);

	return *this;
}

LoaderPlugin& LoaderPlugin::cursor (std::string key_, std::string path_, int hotX_, int hotY_)
{
	path_ = path + path_;

	g_input.createColorCursor(key_, path_, hotX_, hotY_);

	return *this;
}

LoaderPlugin& LoaderPlugin::atlas (std::string key_, std::string texturePath_, std::string atlasPath_)
{
	texturePath_ = path + texturePath_;
	atlasPath_ = path + atlasPath_;

	g_texture.addAtlas(key_, texturePath_, atlasPath_);

	return *this;
}

LoaderPlugin& LoaderPlugin::multiatlas (std::string key_, std::string atlasPath_, std::string path_)
{
	atlasPath_ = path + atlasPath_;
	path_ = path + path_;

	std::vector<std::string> sources_;

	// Open data file
	std::ifstream file_ (atlasPath_);

	if (!file_.is_open()) {
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

	g_texture.addAtlas(key_, sources_, atlasPath_);

	return *this;
}

LoaderPlugin& LoaderPlugin::spritesheet (std::string key_, std::string path_, SpriteSheetConfig config_)
{
	path_ = path + path_;

	g_texture.addSpriteSheet(key_, path_, config_);

	return *this;
}

LoaderPlugin& LoaderPlugin::audioShort (std::string key_, std::string path_)
{
	path_ = path + path_;

	g_audio.addAudioShort(key_, path_);

	return *this;
}

LoaderPlugin& LoaderPlugin::audioStream (std::string key_, std::string path_)
{
	path_ = path + path_;

	g_audio.addAudioStream(key_, path_);

	return *this;
}

LoaderPlugin& LoaderPlugin::font (std::string key_, std::string path_)
{
	path_ = path + path_;

	g_text.addFont(key_, path_);

	return *this;
}

void LoaderPlugin::reset ()
{
	setPath(g_config->loaderPath);
	setPrefix(g_config->loaderPrefix);
}

}	// namespace Zen
