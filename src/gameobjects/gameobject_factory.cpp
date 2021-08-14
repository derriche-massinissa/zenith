/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "gameobject_factory.h"
#include "../scene/scene.hpp"

#include <memory>
#include "../components/alpha.hpp"
#include "../components/blend_mode.hpp"
#include "../components/depth.hpp"
#include "../components/flip.hpp"
#include "../components/bounds.hpp"
#include "../components/mask.hpp"
#include "../components/origin.hpp"
#include "../components/scroll_factor.hpp"
#include "../components/size.hpp"
#include "../components/textured.hpp"
#include "../components/tint.hpp"
#include "../components/position.hpp"
#include "../components/rotation.hpp"
#include "../components/scale.hpp"
#include "../components/visible.hpp"
#include "../components/renderable.hpp"
#include "../components/actor.hpp"
#include "../components/crop.hpp"
#include "../components/text.hpp"
#include "../systems/size.hpp"
#include "../systems/origin.hpp"
#include "../systems/textured.hpp"
#include "../systems/text.hpp"

namespace Zen {

extern entt::registry g_registry;


GameObjectFactory::GameObjectFactory (Scene* scene_)
	: scene (scene_)
{}

void GameObjectFactory::boot ()
{}

/*
Image& GameObjectFactory::image (int x, int y, std::string key, std::string frame)
{
	return *static_cast<Image*> (
		scene->children.add(std::move( std::make_unique<Image>(scene, x, y, key, frame) ))
	);
}
*/

Entity GameObjectFactory::image (double x, double y, std::string key, std::string frame)
{
	auto img = g_registry.create();

	g_registry.emplace<Components::Alpha>(img);
	g_registry.emplace<Components::BlendMode>(img);
	g_registry.emplace<Components::Depth>(img);
	g_registry.emplace<Components::Flip>(img);
	g_registry.emplace<Components::Bounds>(img);
	g_registry.emplace<Components::Mask>(img);
	g_registry.emplace<Components::Origin>(img);
	g_registry.emplace<Components::ScrollFactor>(img);
	g_registry.emplace<Components::Size>(img);
	g_registry.emplace<Components::Textured>(img);
	g_registry.emplace<Components::Tint>(img);
	g_registry.emplace<Components::Position>(img, x, y);
	g_registry.emplace<Components::Rotation>(img);
	g_registry.emplace<Components::Scale>(img);
	g_registry.emplace<Components::Visible>(img);
	g_registry.emplace<Components::Renderable>(img);
	g_registry.emplace<Components::Crop>(img);
	g_registry.emplace<Components::Actor>(img);

	SetTexture(img, key, frame);
	SetSizeToFrame(img);
	SetOriginFromFrame(img);

	scene->children.add(img);

	return img;
}

Entity GameObjectFactory::text (double x, double y, std::string text, TextStyle style)
{
	auto txt = g_registry.create();

	g_registry.emplace<Components::Text>(txt);

	// Not a textured object. Each character is directly rendered from an atlas
	g_registry.emplace<Components::Alpha>(txt);
	g_registry.emplace<Components::BlendMode>(txt);
	g_registry.emplace<Components::Depth>(txt);
	g_registry.emplace<Components::Bounds>(txt);
	g_registry.emplace<Components::Origin>(txt, 0.5, 0.5);
	g_registry.emplace<Components::ScrollFactor>(txt);
	g_registry.emplace<Components::Size>(txt);
	g_registry.emplace<Components::Position>(txt, x, y);
	g_registry.emplace<Components::Rotation>(txt);
	g_registry.emplace<Components::Scale>(txt);
	g_registry.emplace<Components::Visible>(txt);
	g_registry.emplace<Components::Renderable>(txt);
	g_registry.emplace<Components::Actor>(txt);

	// Generate texture
	SetTextStyle(txt, style);
	SetText(txt, text);

	scene->children.add(txt);

	return txt;
}

Entity GameObjectFactory::text (double x, double y, std::vector<std::string> lines,
		TextStyle style)
{
	std::string txt = lines[0];

	// Join all the lines, separating them with a new line
	for (size_t i = 1; i < lines.size(); i++) {
		txt += '\n';
		txt += lines[i];
	}

	return text(x, y, txt, style);
}


}	//namespace Zen
