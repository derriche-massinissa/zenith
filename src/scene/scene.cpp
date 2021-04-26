/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "scene.hpp"

namespace Zen {

extern TextureManager g_texture;
extern ScaleManager g_scale;
extern Renderer g_renderer;
extern AudioManager g_audio;

Scene::Scene (Game& game_)
	: sys (this)
	, game (game_)
	, audio (g_audio)
	, textures (g_texture)
	, cameras (this)
	, add (this)
	, scene (this)
	, updateList (this)
	, input (this)
	, load (this)
	, scale (g_scale)
	, renderer (g_renderer)
{}

void Scene::init ([[maybe_unused]] Data data_)
{}

void Scene::preload ()
{}

void Scene::create ([[maybe_unused]] Data data_)
{}

void Scene::update ([[maybe_unused]] Uint32 time_, [[maybe_unused]] Uint32 delta_)
{}

}	// namespace Zen
