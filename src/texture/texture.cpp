/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "texture.h"

#include "texture_manager.h"

namespace Zen {
namespace Textures {

Texture::Texture(TextureManager& manager_, std::string key_, std::vector<std::string> sources_)
	: manager (&manager_)
{
	// Load the Sources
	for (auto& source_ : sources_)
		source.emplace_back(this, source_);
}

Frame* Texture::add (std::string name_, int sourceIndex_, int x_, int y_, int width_, int height_)
{
	if (has(name_))
		return nullptr;

	Frame frame_ (this, name_, sourceIndex_, x_, y_, width_, height_);

	frames.emplace(name_, frame_);

	//  Set the first frame of the Texture (other than __BASE)
	//  This is used to ensure we don't spam the display with entire
	//  atlases of sprite sheets, but instead just the first frame of them
	//  should the dev incorrectly specify the frame index
	if (firstFrame == "__BASE")
		firstFrame = name_;

	frameTotal++;

	return &frames.find(name_)->second;
}

Frame* Texture::add (int name_, int sourceIndex_, int x_, int y_, int width_, int height_)
{
	return add(std::to_string(name_), sourceIndex_, x_, y_, width_, height_);
}

bool Texture::remove (std::string name_)
{
	if (!has(name_))
		return false;

	frames.erase(frames.find(name_));

	frameTotal--;

	return true;
}

bool Texture::has (std::string name_)
{
	return (frames.find(name_) != frames.end());
}

Frame* Texture::get (std::string name_)
{
	if (name_.empty())
		name_ = firstFrame;

	Frame* frame_ = &frames.find(name_)->second;

	if (!frame_)
	{
		messageWarning("Texture missing: ", name_);

		frame_ = &frames.find(firstFrame)->second;
	}

	return frame_;
}

Frame* Texture::get (int index_)
{
	return get(std::to_string(index_));
	/*
	Frame* frame_ = &frames.at(index_)->second;

	if (!frame_)
	{
		messageWarning("Texture missing at index: ", std::to_string(index_));

		frame_ = &frames.find(firstFrame)->second;
	}

	return frame_;
	*/
}

int Texture::getTextureSourceIndex (TextureSource& source_)
{
	for (int i_ = 0; i_ < source.size(); i_++)
	{
		if (&source[i_] == &source_)
			return i_;
	}

	return -1;
}

std::vector<Frame*> Texture::getFramesFromTextureSource (int sourceIndex_, bool includeBase_)
{
	std::vector<Frame*> out_;

	for (auto it_ = frames.begin(); it_ != frames.end(); it_++)
	{
		if (it_->first == "__BASE" && !includeBase_)
			continue;

		if (it_->second.sourceIndex == sourceIndex_)
			out_.emplace_back(&it_->second);
	}

	return out_;
}

std::vector<std::string> Texture::getFrameNames (bool includeBase_)
{
	std::vector<std::string> out_;

	for (auto it_ = frames.begin(); it_ != frames.end(); it_++)
	{
		if (it_->first == "__BASE" && !includeBase_)
			continue;

		out_.emplace_back(it_->first);
	}

	return out_;
}

}	// namespace Textures
}	// namespace Zen
