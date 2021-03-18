/**
 * @file		texture.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "texture.h"

namespace Zen {
namespace Textures {

Texture::Texture(TextureManager& manager, std::string key, std::vector<std::string> sources)
{
	// Load the Sources
	for (auto it : sources)
		source_.emplace_back(*this, it);
}

Texture::~Texture()
{
}

Frame* Texture::add (std::string name, int sourceIndex, int x, int y, int width, int height)
{
	if (has(name))
		return nullptr;

	Frame frame (*this, name, sourceIndex, x, y, width, height);

	frames_.emplace(name, frame);

	//  Set the first frame of the Texture (other than __BASE)
	//  This is used to ensure we don't spam the display with entire
	//  atlases of sprite sheets, but instead just the first frame of them
	//  should the dev incorrectly specify the frame index
	if (firstFrame_ == "__BASE")
		firstFrame_ = name;

	frameTotal_++;

	return &frames_.find(name)->second;
}

bool Texture::remove (std::string name)
{
	if (!has(name))
		return false;

	frames_.erase(frames_.find(name));

	frameTotal_--;

	return true;
}

bool Texture::has (std::string name)
{
	return (frames_.find(name) != frames_.end());
}

Frame* Texture::get (std::string name)
{
	if (name.empty())
		name = firstFrame_;

	Frame* frame = &frames_.find(name)->second;

	if (!frame) {
		messageWarning("Texture missing: ", name);

		frame = &frames_.find(firstFrame_)->second;
	}

	return frame;
}

Frame* Texture::get (int index)
{
	Frame* frame = &frames_.at(index)->second;

	if (!frame) {
		messageWarning("Texture missing at index: ", index);

		frame = &frames_.find(firstFrame_)->second;
	}

	return frame;
}

int Texture::getTextureSourceIndex (TextureSource& source)
{
	for (int i = 0; i < source_.size(); i++) {
		if (&source[i] == &source)
			return i;
	}

	return -1;
}

std::vector<Frame*> Texture::getFramesFromTextureSource (int sourceIndex, bool includeBase)
{
	std::vector<Frame*> out;

	for (auto it = frames_.begin(); it != frames_.end(); it++) {
		if (it->first == "__BASE" && !includeBase)
			continue;

		if (it->second.sourceIndex == sourceIndex)
			out.emplace_back(&it->second);
	}

	return out;
}

std::vector<std::string> Texture::getFrameNames (bool includeBase)
{
	std::vector<std::string> out;

	for (auto it = frames_.begin(); it != frames_.end(); it++) {
		if (it->first == "__BASE" && !includeBase)
			continue;

		out.emplace_back(it->first);
	}

	return out;
}

GameObjects::RenderTexture Texture::getSourceImage (std::string name)
{
}

}	// namespace Textures
}	// namespace Zen
