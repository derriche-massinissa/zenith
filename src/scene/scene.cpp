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

Scene::Scene (Game& game_)
	: sys (this)
	, game (game_)
	, textures (game_.textures)
	, cameras (this)
	, add (this)
	, scene (this)
	, updateList (this)
	, load (this)
	, scale (game_.scale)
	, renderer (game_.renderer)
{}

void Scene::init (Data data_)
{}

void Scene::preload ()
{}

void Scene::create (Data data_)
{}

void Scene::update (Uint32 time_, Uint32 delta_)
{}

}	// namespace Zen
