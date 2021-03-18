/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "json_array.h"

#include "../../messages.h"
#include "../texture.h"
#include "../texture_source.h"
#include "../frame.h"

namespace Zen {
namespace Textures {

void parseJsonArray (Texture *texture, int sourceIndex, nlohmann::json json)
{
	// Malformed?
	if (json.find("frames") == json.end() ||
		json.find("textures") == json.end())
	{
		messageError("Invalid Texture Atlas JSON Array");
		return;
	}

	// Add in a __BASE entry (for the entire atlas)
	TextureSource *source = &texture->source[sourceIndex];

	texture->add("__BASE", sourceIndex, 0, 0, source->width, source->height);

	// By this stage frames is a fully parsed array
	auto& frames = 
		(json["textures"] != nullptr && json["textures"].is_array())
		? json["textures"][sourceIndex]["frames"]
		: json["frames"];

	Frame *newFrame = nullptr;

	for (int i = 0; i < frames.size(); i++)
	{
		auto& src = frames[i];

        //  The frame values are the exact coordinates to cut the frame out of the 
		//  atlas from
		newFrame = texture->add(
				src["filename"].get<std::string>(),
				sourceIndex,
				src["frame"]["x"],
				src["frame"]["y"],
				src["frame"]["w"],
				src["frame"]["h"]
				);

		// These are the original (non-trimmed) sprite values
		if (src["trimmed"] != nullptr && src["trimmed"])
		{
			newFrame->setTrim(
					src["sourceSize"]["w"],
					src["sourceSize"]["h"],
					src["spriteSourceSize"]["x"],
					src["spriteSourceSize"]["y"],
					src["spriteSourceSize"]["w"],
					src["spriteSourceSize"]["h"]
					);
		}

		if (src["rotated"] != nullptr && src["rotated"])
		{
			newFrame->rotated = true;
			newFrame->updateUVsInverted();
		}

		auto& pivot = (src["anchor"] != nullptr) ? src["anchor"] : src["pivot"];

		if (pivot != nullptr)
		{
			newFrame->customPivot = true;
			newFrame->pivotX = pivot["x"];
			newFrame->pivotY = pivot["y"];
		}
	}
}

}	// namespace Textures
}	// namespace Zen
