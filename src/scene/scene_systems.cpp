/**
 * @file		scene_systems.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "scene_systems.h"

Zen::SceneSystems::SceneSystems (Zen::Scene& s)
	: scene(s)
	, settings(s.key)
{}

Zen::SceneSystems::~SceneSystems ()
{
	settings.status = SCENE::DESTROYED;

	settings.active = false;

	settings.visible = false;

	scene.events.emit("SYS_DESTROY");

	scene.events.removeAllListeners();
}

void Zen::SceneSystems::init ()
{
	settings.status = SCENE::INIT;

	scene.events.emit("SYS_BOOT");

	settings.isBooted = true;
}

void Zen::SceneSystems::step (Uint32 time, Uint32 delta)
{
	scene.events.emit("SYS_PRE_UPDATE", {{time, delta}});

	scene.events.emit("SYS_UPDATE", {{time, delta}});

	scene.update(time, delta);

	scene.events.emit("SYS_POST_UPDATE", {{time, delta}});
}

void Zen::SceneSystems::render ()
{
	//scene.children.depthSort();

	scene.events.emit("SYS_PRE_RENDER");

	//scene.cameras.render(scene.children);

	scene.events.emit("SYS_RENDER");
}

void Zen::SceneSystems::queueDepthSort ()
{
	//scene.children.queueDepthSort();
}

void Zen::SceneSystems::depthSort ()
{
	//scene.children.depthSort();
}

Zen::SceneSystems& Zen::SceneSystems::pause (Zen::Data e)
{
	if (settings.active) {
		settings.status = SCENE::PAUSED;

		settings.active = false;

		scene.events.emit("SYS_PAUSE", e);
	}

	return *this;
}

Zen::SceneSystems& Zen::SceneSystems::resume (Zen::Data e)
{
	if (settings.active) {
		settings.status = SCENE::RUNNING;

		settings.active = true;

		scene.events.emit("SYS_RESUME", e);
	}

	return *this;
}

Zen::SceneSystems& Zen::SceneSystems::sleep (Zen::Data e)
{
	settings.status = SCENE::SLEEPING;

	settings.active = false;

	settings.visible = false;

	scene.events.emit("SYS_SLEEP", e);

	return *this;
}

Zen::SceneSystems& Zen::SceneSystems::wake (Zen::Data e)
{
	settings.status = SCENE::RUNNING;

	settings.active = true;

	settings.visible = true;

	scene.events.emit("SYS_WAKE", e);

	if (settings.isTransition)
		scene.events.emit("SYS_TRANSITION_WAKE", {{settings.transitionDuration}, {settings.transitionFrom->sys.settings.key}});

	return *this;
}

bool Zen::SceneSystems::isSleeping ()
{
	return settings.status == SCENE::SLEEPING;
}

bool Zen::SceneSystems::isActive ()
{
	return settings.status == SCENE::RUNNING;
}

bool Zen::SceneSystems::isPaused ()
{
	return settings.status == SCENE::PAUSED;
}

bool Zen::SceneSystems::isTransitioning ()
{
	return (settings.isTransition || scene.scene._target != nullptr);
}

bool Zen::SceneSystems::isTransitionOut ()
{
	return (scene.scene._target != nullptr && scene.scene._duration > 0);
}

bool Zen::SceneSystems::isTransitionIn ()
{
	return settings.isTransition;
}

bool Zen::SceneSystems::isVisible ()
{
	return settings.visible;
}

Zen::SceneSystems& Zen::SceneSystems::setVisible (bool value)
{
	settings.visible = value;

	return *this;
}

Zen::SceneSystems& Zen::SceneSystems::setActive (bool value, Data data)
{
	if (value)
		return resume (data);
	else
		return pause (data);
}

void Zen::SceneSystems::start (Zen::Data data)
{
	settings.data = data;

	settings.status = SCENE::START;

	settings.active = true;

	settings.visible = true;

	scene.events.emit("SYS_START");

	scene.events.emit("SYS_READY", data);
}

void Zen::SceneSystems::shutdown (Zen::Data data)
{
	scene.events.removeAllListeners({"SYS_TRANSITION_INIT"});
	scene.events.removeAllListeners({"SYS_TRANSITION_START"});
	scene.events.removeAllListeners({"SYS_TRANSITION_COMPLETE"});
	scene.events.removeAllListeners({"SYS_TRANSITION_OUT"});

	settings.status = SCENE::SHUTDOWN;

	settings.active = false;
	settings.visible = false;

	scene.events.emit("SYS_SHUTDOWN", data);
}
