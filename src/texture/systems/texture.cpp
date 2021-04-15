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
	g_registry.emplace<Components::Texture>(texture, key, 0, entt::null);
	
	// Load the Sources
	for (size_t i = 0; i < sources.size(); i++)
	{
		sourceEntities.emplace_back(CreateTextureSource(texture, sources[i], i));

		// Check if the source is valid and loaded
		// Otherwise this texture is invalid
		auto& src = g_registry.get<Components::TextureSource>(sourceEntities[i]);
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

Entity CreateTexture (std::string key, std::vector<std::string> sources)
{
	std::vector<const char*> cSources;
	cSources.reserve(sources.size());

	for (auto& source : sources)
	{
		cSources.emplace_back(source.c_str());
	}

	return CreateTexture(key.c_str(), cSources);
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
	for (auto entity : g_registry.view<Components::TextureSource>())
	{
		auto& source = g_registry.get<Components::TextureSource>(entity);

		if (source.texture == texture && source.index == sourceIndex)
		{
			frame = CreateFrame(entity, name, x, y, width, height);

			auto& tx = g_registry.get<Components::Texture>(texture);

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

		auto& tx = g_registry.get<Components::Texture>(texture);
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

	auto& tx = g_registry.get<Components::Texture>(texture);

	for (auto entity : g_registry.view<Components::Frame>())
	{
		auto& frame = g_registry.get<Components::Frame>(entity);

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
	auto tx = g_registry.try_get<Components::Texture>(texture);
	ZEN_ASSERT(tx, "The entity has no 'Texture' component.");

	for (auto entity : g_registry.view<Components::Frame>())
	{
		auto& frame = g_registry.get<Components::Frame>(entity);

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
	auto tx = g_registry.try_get<Components::Texture>(texture);
	ZEN_ASSERT(tx, "The entity has no 'Texture' component.");

	Entity frame = entt::null;

	if (name.empty())
	{
		frame = tx->firstFrame;
	}
	else
	{
		for (auto entity : g_registry.view<Components::Frame>())
		{
			auto& fr = g_registry.get<Components::Frame>(entity);
			auto& src = g_registry.get<Components::TextureSource>(fr.source);
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
	auto src = g_registry.try_get<Components::TextureSource>(source);
	ZEN_ASSERT(src, "The entity has no 'Source' component.");

	return src->index;
}

std::vector<Entity> GetFramesFromSource (Entity source, bool includeBase)
{
	std::vector<Entity> output;

	for (auto entity : g_registry.view<Components::Frame>())
	{
		auto& frame = g_registry.get<Components::Frame>(entity);

		if (std::string("__BASE") == frame.name && !includeBase)
			continue;

		if (frame.source == source)
			output.emplace_back(entity);
	}

	return output;
}

std::vector<std::string> GetFrameNames (Entity texture, bool includeBase)
{
	auto tx = g_registry.try_get<Components::TextureSource>(texture);
	ZEN_ASSERT(tx, "The entity has no 'TextureSource' component.");

	std::vector<std::string> output;

	for (auto frame : g_registry.view<Components::Frame>())
	{
		auto& fr = g_registry.get<Components::Frame>(frame);
		auto& src = g_registry.get<Components::TextureSource>(fr.source);

		if (std::string("__BASE") == fr.name && !includeBase)
			continue;

		if (src.texture == texture)
			output.emplace_back(fr.name);
	}

	return output;
}

std::vector<Entity> GetTextureSources (Entity texture)
{
	std::vector<Entity> out_;

	auto tx = g_registry.try_get<Components::Texture>(texture);
	ZEN_ASSERT(tx, "The entity has no 'Texture' component.");

	auto sources_ = g_registry.view<Components::TextureSource>();

	for (auto source_ : sources_)
	{
		auto& src_ = g_registry.get<Components::TextureSource>(source_);

		if (src_.sdlTexture)
		{
			out_.push_back(source_);
		}
	}

	return out_;
}

std::vector<std::string> GetTextureSourceFiles (Entity texture)
{
	std::vector<std::string> out_;

	auto tx = g_registry.try_get<Components::Texture>(texture);
	ZEN_ASSERT(tx, "The entity has no 'Texture' component.");

	auto sources_ = g_registry.view<Components::TextureSource>();

	for (auto source_ : sources_)
	{
		auto& src_ = g_registry.get<Components::TextureSource>(source_);

		if (src_.sdlTexture)
		{
			out_.push_back(src_.source);
		}
	}

	return out_;
}

}	// namespace Zen
