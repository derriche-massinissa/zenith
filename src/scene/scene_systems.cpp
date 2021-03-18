/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "scene_systems.h"

#include "scene_config.h"
#include "scene.h"
#include "../core/game.h"

namespace Zen {
namespace Scenes {

SceneSystems::SceneSystems (Scene* scene_)
	: settings(scene_->key)
	, scene(scene_)
{}

SceneSystems::~SceneSystems ()
{
	settings.status = SCENE::DESTROYED;

	events.emit("destroy");

	events.removeAllListeners();
	scene->events.removeAllListeners();
}

void SceneSystems::init ()
{
	settings.status = SCENE::INIT;

	events.emit("boot");

	settings.isBooted = true;
}

void SceneSystems::step (Uint32 time_, Uint32 delta_)
{
	events.emit("pre-update", time_, delta_);

	events.emit("update", time_, delta_);

	scene->update(time_, delta_);

	events.emit("post-update", time_, delta_);
}

void SceneSystems::render ()
{
	scene->children.depthSort();

	events.emit("pre-render");

	scene->cameras.render(scene->renderer, scene->children);

	events.emit("render");
}

void SceneSystems::queueDepthSort ()
{
	scene->children.queueDepthSort();
}

void SceneSystems::depthSort ()
{
	scene->children.depthSort();
}

SceneSystems& SceneSystems::pause (Data data_)
{
	if (settings.active) {
		settings.status = SCENE::PAUSED;

		settings.active = false;

		events.emit("pause", data_);
	}

	return *this;
}

SceneSystems& SceneSystems::resume (Data data_)
{
	if (!settings.active) {
		settings.status = SCENE::RUNNING;

		settings.active = true;

		events.emit("resume", data_);
	}

	return *this;
}

SceneSystems& SceneSystems::sleep (Data data_)
{
	settings.status = SCENE::SLEEPING;

	settings.active = false;
	settings.visible = false;

	events.emit("sleep", data_);

	return *this;
}

SceneSystems& SceneSystems::wake (Data data_)
{
	settings.status = SCENE::RUNNING;

	settings.active = true;
	settings.visible = true;

	events.emit("wake", data_);

	if (settings.isTransition)
	{
		events.emit(
				"transition-wake",
				settings.transitionFrom->sys.settings.key,
				settings.transitionDuration
				);
	}

	return *this;
}

Data SceneSystems::getData ()
{
	return settings.data;
}

bool SceneSystems::isSleeping ()
{
	return settings.status == SCENE::SLEEPING;
}

bool SceneSystems::isActive ()
{
	return settings.status == SCENE::RUNNING;
}

bool SceneSystems::isPaused ()
{
	return settings.status == SCENE::PAUSED;
}

bool SceneSystems::isTransitioning ()
{
	return (settings.isTransition || scene->scene.transitionTarget != nullptr);
}

bool SceneSystems::isTransitionOut ()
{
	return (scene->scene.transitionTarget != nullptr &&
			scene->scene.transitionDuration > 0);
}

bool SceneSystems::isTransitionIn ()
{
	return settings.isTransition;
}

bool SceneSystems::isVisible ()
{
	return settings.visible;
}

SceneSystems& SceneSystems::setVisible (bool value_)
{
	settings.visible = value_;

	return *this;
}

SceneSystems& SceneSystems::setActive (bool value_, Data data_)
{
	if (value_)
		return resume (data_);
	else
		return pause (data_);
}

void SceneSystems::start (Data data_)
{
	settings.data = data_;

	settings.status = SCENE::START;

	settings.active = true;
	settings.visible = true;

	events.emit("start");

	events.emit("ready", data_);
}

void SceneSystems::shutdown (Data data_)
{
	events.removeAllListeners("transition-init");
	events.removeAllListeners("transition-start");
	events.removeAllListeners("transition-complete");
	events.removeAllListeners("transition-out");

	settings.status = SCENE::SHUTDOWN;

	settings.active = false;
	settings.visible = false;

	events.emit("shutdown", data_);
}

}	// namespace Scenes
}	// namespace Zen
