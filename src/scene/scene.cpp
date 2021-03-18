/**
 * @file		scene.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "scene.h"

Zen::Scene::Scene (Game& g)
	: game(g)
	, sys(*this)
	, scene(*this)
{}

Zen::Scene::~Scene ()
{}

void Zen::Scene::init (Zen::Data data)
{}

void Zen::Scene::preload ()
{}

void Zen::Scene::create (Zen::Data data)
{}

void Zen::Scene::update (Uint32 time, Uint32 delta)
{}
