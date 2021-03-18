/**
 * @file		scene_settings.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "scene_settings.h"

Zen::SceneSettings::SceneSettings (std::string k, bool a, bool v)
{
	status = SCENE::PENDING;

	key = k;

	active = a;

	visible = v;

	isBooted = false;

	isTransition = false;
	transitionFrom = nullptr;
	transitionDuration = 0;
	transitionAllowInput = false;

	data = {};
}

Zen::SceneSettings::~SceneSettings ()
{}
