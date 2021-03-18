/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "scene.h"

namespace Zen {

Scene::Scene (Game& game_)
	: load (*this)
	, sys (*this)
	, scene (*this)
	, cameras (this)
	, renderer (*this)
	, textures (game_.textures)
	, scale (game_.scale)
	, game (game_)
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
