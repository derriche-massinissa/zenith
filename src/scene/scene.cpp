/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "scene.h"

/*
#include "scene_systems.h"
#include "scene_plugin.h"
#include "../core/game.h"
#include "../scale/scale_manager.h"
#include "../cameras/2d/camera_manager.h"
#include "../loader/loader_plugin.h"
#include "../texture/texture_manager.h"
*/

namespace Zen {

extern AudioManager g_audio;

Scene::Scene (Game& game_)
	: sys (this)
	, game (game_)
	, audio (g_audio)
	, textures (game_.textures)
	, cameras (this)
	, add (this)
	, scene (this)
	, updateList (this)
	, input (this)
	, load (this)
	, scale (game_.scale)
	, renderer (game_.renderer)
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
