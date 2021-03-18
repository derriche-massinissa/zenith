/**
 * @file		loader_plugin.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "loader_plugin.h"

namespace Zen {
namespace Loader {

LoaderPlugin::LoaderPlugin ();

void pluginStart ();

LoaderPlugin& LoaderPlugin::setPath (std::string path = "");

LoaderPlugin& LoaderPlugin::setPrefix (std::string prefix = "");

LoaderPlugin& LoaderPlugin::image (std::string key, std::string path)
{
	textureManager_.addImage(key, path);

	return *this;
}

LoaderPlugin& LoaderPlugin::atlas (std::string key, std::string texturePath, std::string atlasPath)
{
	textureManager_.addAtlas(key, texturePath, atlasPath);

	return *this;
}

LoaderPlugin& LoaderPlugin::multiatlas (std::string key, std::string atlasPath, std::string path = "")
{
	std::vector<std::string> sources;

	// Open data file
	std::fstream file (atlasPath, std::ios::in);

	if (!file) {
		messageError("Atlas file failed to open: ", atlasPath);
		return *this;
	}

	// Create a JSON object
	nlohmann::json data;
	file >> data;

	// Close file
	file.close();

	for (const auto& fileName : data["textures"])
		sources.emplace_back(path + fileName);

	textureManager_.addAtlas(key, sources, atlasPath);

	return *this;
}

LoaderPlugin& LoaderPlugin::audio (std::string key, std::string path)
{
	return *this;
}

LoaderPlugin& LoaderPlugin::font (std::string key, std::string path)
{
	return *this;
}

}	// namespace Loader
}	// namespace Zen
