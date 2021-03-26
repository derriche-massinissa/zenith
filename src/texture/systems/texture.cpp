/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "texture.hpp"

#include <vector>
#include <string>
#include "../../ecs/entity.hpp"
#include "../../utils/assert.hpp"
#include "frame.hpp"
#include "source.hpp"
#include "../components/texture.hpp"
#include "../components/source.hpp"
#include "../components/frame.hpp"

namespace Zen {

extern entt::registry g_registry;

Entity CreateTexture (const char* key, std::vector<const char*> sources)
{
	std::vector<Entity> sourceEntities;

	// Create the texture entity
	auto texture = g_registry.create();
	g_registry.emplace<TextureComponent>(texture, key, 0, entt::null);
	
	// Load the Sources
	for (int i = 0; i < sources.size(); i++)
	{
		sourceEntities[i] = CreateTextureSource(texture, sources[i], i);

		// Check if the source is valid and loaded
		// Otherwise this texture is invalid
		auto& src = g_registry.get<TextureSourceComponent>(sourceEntities[i]);
		if (!src.sdlTexture)
		{
			// Destroy all created sources and return null
			for (int j = i; j >= 0; j--)
				DestroyTextureSource(sourceEntities[i]);

			return entt::null;
		}
	}
	
	return texture;
}

void DestroyTexture (Entity texture)
{
	g_registry.destroy(texture);
}

Entity AddFrame (Entity texture, const char* name, int sourceIndex, int x, int y, int width, int height)
{
	if (HasFrame(texture, name))
		return entt::null;

	Entity frame;

	// Get the source in the texture with the given index
	for (auto entity : g_registry.view<TextureSourceComponent>())
	{
		auto& source = g_registry.get<TextureSourceComponent>(entity);

		if (source.texture == texture && source.index == sourceIndex)
		{
			frame = CreateFrame(entity, name, x, y, width, height);

			auto& tx = g_registry.get<TextureComponent>(texture);

			tx.firstFrame = frame;

			tx.frameTotal++;

			return frame;
		}
	}

	return entt::null;
}

Entity AddFrame (Entity texture, int index, int sourceIndex, int x, int y, int width, int height)
{
	return AddFrame(texture, std::to_string(index).c_str(), sourceIndex, x, y, width, height);
}

bool RemoveFrame (Entity texture, std::string name)
{
	auto frame = GetFrame(texture, name);

	if (frame != entt::null)
	{
		g_registry.destroy(frame);

		auto& tx = g_registry.get<TextureComponent>(texture);
		tx.frameTotal--;

		return true;
	}
	else
	{
		return false;
	}

	/*
	if (!HasFrame(texture, name))
		return false;

	auto& tx = g_registry.get<TextureComponent>(texture);

	for (auto entity : g_registry.view<FrameComponent>())
	{
		auto& frame = g_registry.get<FrameComponent>(entity);

		if (frame.texture == texture && name == frame.name)
		{
			g_registry.destroy(entity);
			break;
		}
	}

	tx.frameTotal--;

	return true;
	*/
}

bool RemoveFrame (Entity texture, int index)
{
	return RemoveFrame(texture, std::to_string(index).c_str());
}

bool HasFrame (Entity texture, std::string name)
{
	return GetFrame(texture, name) != entt::null;

	/*
	auto tx = g_registry.try_get<TextureComponent>(texture);
	ZEN_ASSERT(tx, "The entity has no 'Texture' component.");

	for (auto entity : g_registry.view<FrameComponent>())
	{
		auto& frame = g_registry.get<FrameComponent>(entity);

		if (frame.texture == texture && name == frame.name)
		{
			return true;
		}
	}

	return false;
	*/
}

bool HasFrame (Entity texture, int index)
{
	return HasFrame(texture, std::to_string(index));
}

Entity GetFrame (Entity texture, std::string name)
{
	auto tx = g_registry.try_get<TextureComponent>(texture);
	ZEN_ASSERT(tx, "The entity has no 'Texture' component.");

	Entity frame = entt::null;

	if (name.empty())
	{
		frame = tx->firstFrame;
	}
	else
	{
		for (auto entity : g_registry.view<FrameComponent>())
		{
			auto& fr = g_registry.get<FrameComponent>(entity);
			auto& src = g_registry.get<TextureSourceComponent>(fr.source);
			if (src.texture == texture && name == fr.name)
			{
				frame = entity;
				break;
			}
		}
	}

	return frame;
}

Entity GetFrame (Entity texture, int index)
{
	return GetFrame(texture, std::to_string(index));
}

int GetTextureSourceIndex (Entity source)
{
	auto src = g_registry.try_get<TextureSourceComponent>(source);
	ZEN_ASSERT(src, "The entity has no 'Source' component.");

	return src->index;
}

std::vector<Entity> GetFramesFromSource (Entity source, bool includeBase)
{
	std::vector<Entity> output;

	for (auto entity : g_registry.view<FrameComponent>())
	{
		auto& frame = g_registry.get<FrameComponent>(entity);

		if (std::string("__BASE") == frame.name && !includeBase)
			continue;

		if (frame.source == source)
			output.emplace_back(frame);
	}

	return output;
}

std::vector<std::string> GetFrameNames (Entity texture, bool includeBase)
{
	auto tx = g_registry.try_get<TextureSourceComponent>(texture);
	ZEN_ASSERT(tx, "The entity has no 'Texture' component.");

	std::vector<std::string> output;

	for (auto frame : g_registry.view<FrameComponent>())
	{
		auto& fr = g_registry.get<FrameComponent>(frame);
		auto& src = g_registry.get<TextureSourceComponent>(fr.source);

		if (std::string("__BASE") == fr.name && !includeBase)
			continue;

		if (src.texture == texture)
			output.emplace_back(fr.name);
	}

	return output;
}

}	// namespace Zen
