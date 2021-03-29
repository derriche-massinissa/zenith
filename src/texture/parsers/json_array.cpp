/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "json_array.hpp"

#include "../../utils/messages.hpp"
#include "../../utils/assert.hpp"
#include "../systems/texture.hpp"
#include "../systems/frame.hpp"
#include "../components/source.hpp"
#include "../components/frame.hpp"

namespace Zen {

extern entt::registry g_registry;

void ParseJsonArray (Entity texture, int sourceIndex, nlohmann::json json)
{
	// Malformed?
	if (json.find("frames") == json.end() ||
		json.find("textures") == json.end())
	{
		MessageError("Invalid Texture Atlas JSON Array");
		return;
	}

	// Get the source component
	Components::TextureSource *source = nullptr;
	for (auto& entity : g_registry.view<Components::TextureSource>())
	{
		auto& src = g_registry.get<Components::TextureSource>(entity);
		if (src.texture == texture && src.index == sourceIndex)
		{
			source = &src;
			break;
		}
	}
	ZEN_ASSERT(source, "The requested texture source does not exist.");

	// Add in a __BASE entry (for the entire atlas)
	AddFrame(texture, "__BASE", sourceIndex, 0, 0, source->width, source->height);

	// By this stage frames is a fully parsed array
	auto& frames =
		(json["textures"] != nullptr && json["textures"].is_array())
		? json["textures"][sourceIndex]["frames"]
		: json["frames"];

	Entity newFrame = entt::null;

	for (int i = 0; i < frames.size(); i++)
	{
		auto& src = frames[i];

        //  The frame values are the exact coordinates to cut the frame out of the 
		//  atlas from
		newFrame = AddFrame(
				texture,
				src["filename"].get<std::string>().c_str(),
				sourceIndex,
				src["frame"]["x"],
				src["frame"]["y"],
				src["frame"]["w"],
				src["frame"]["h"]
				);

		// These are the original (non-trimmed) sprite values
		if (src["trimmed"] != nullptr && src["trimmed"])
		{
			SetFrameTrim(
					newFrame,
					src["sourceSize"]["w"],
					src["sourceSize"]["h"],
					src["spriteSourceSize"]["x"],
					src["spriteSourceSize"]["y"],
					src["spriteSourceSize"]["w"],
					src["spriteSourceSize"]["h"]
					);
		}

		auto& frame = g_registry.get<Components::Frame>(newFrame);

		if (src["rotated"] != nullptr && src["rotated"])
		{
			frame.rotated = true;
			UpdateFrameUVsInverted(newFrame);
		}

		auto& pivot = (src["anchor"] != nullptr) ? src["anchor"] : src["pivot"];

		if (pivot != nullptr)
		{
			frame.customPivot = true;
			frame.pivotX = pivot["x"];
			frame.pivotY = pivot["y"];
		}
	}
}

}	// namespace Zen
