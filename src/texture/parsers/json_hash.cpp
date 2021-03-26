/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "json_hash.hpp"

#include "../../utils/messages.hpp"
#include "../../utils/assert.hpp"
#include "../systems/texture.hpp"
#include "../systems/frame.hpp"
#include "../components/source.hpp"
#include "../components/frame.hpp"

namespace Zen {

extern entt::registry g_registry;

void ParseJsonHash (Entity texture, int sourceIndex, nlohmann::json json)
{
	// Malformed?
	if (json.find("frames") == json.end())
	{
		MessageError("Invalid Texture Atlas JSON Hash. Missing 'frames' object");
		return;
	}

	// Get the source component
	TextureSourceComponent *source = nullptr;
	for (auto& entity : g_registry.view<TextureSourceComponent>())
	{
		auto& src = g_registry.get<TextureSourceComponent>(entity);
		if (src.texture == texture && src.index == sourceIndex)
		{
			source = &src;
			break;
		}
	}
	ZEN_ASSERT(source, "The requested texture source does not exist.");

	// Add in a __BASE entry (for the entire atlas)
	AddFrame(texture, "__BASE", sourceIndex, 0, 0, source->width, source->height);

	// By this stage frames is a fully parsed object
	auto frames = json["frames"].items();
	Entity newFrame = entt::null;

	for (auto& [key, src] : frames)
	{
        //  The frame values are the exact coordinates to cut the frame out of the
		//  atlas from
		newFrame = AddFrame(
				texture,
				key.c_str(),
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

		auto& frame = g_registry.get<FrameComponent>(newFrame);

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
