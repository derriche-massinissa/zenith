/**
 * @file		scene_plugin.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "scene_plugin.h"
#include <memory>

Zen::ScenePlugin::ScenePlugin (Zen::Scene& s)
	: scene(s)
	, manager(s.game.scene)
{
	/*
	scene = s;
	manager = &scene.game->scene;
	*/

	transitionProgress = 0.0;

	_elapsed = 0;
	_target = nullptr;
	_duration = 0;
	_onUpdate = nullptr;
	_updateListener = nullptr;

	_willSleep = false;
	_willRemove = false;
}

Zen::ScenePlugin::~ScenePlugin ()
{}

Zen::ScenePlugin& Zen::ScenePlugin::start (
		std::string key,
		Zen::Data data
		)
{
	if (key == "")
		key = scene.sys.settings.key;

	manager.queueOp("stop", scene.sys.settings.key);
	manager.queueOp("stop", key, "", data);

	return *this;
}

Zen::ScenePlugin& Zen::ScenePlugin::restart (
		Zen::Data data
		)
{
	manager.queueOp("stop", scene.sys.settings.key);
	manager.queueOp("start", scene.sys.settings.key, "", data);

	return *this;
}

bool Zen::ScenePlugin::transition (
		Zen::SceneTransitionConfig config
		)
{
	std::string key = config.target;
	Scene* target = manager.getScene(key);

	if (!checkValidTransition(target))
		return false;

	Uint32 duration = config.duration;

	_elapsed = 0;
	_target = target;
	_duration = duration;
	_willSleep = config.sleep;
	_willRemove = config.remove;

	_onUpdate = (config.onUpdate != nullptr) ? config.onUpdate : nullptr;

	scene.sys.settings.transitionAllowInput = config.allowInput;

	// Configure the target
	target->sys.settings.isTransition = true;
	target->sys.settings.transitionFrom = &scene;
	target->sys.settings.transitionDuration = duration;
	target->sys.settings.transitionAllowInput = config.allowInput;

	if (config.moveAbove)
		manager.moveAbove(scene.sys.settings.key, target->sys.settings.key);
	else if (config.moveBelow)
		manager.moveBelow(scene.sys.settings.key, target->sys.settings.key);

	if (target->sys.isSleeping())
		target->sys.wake(config.data);
	else
		manager.start(target->sys.settings.key, config.data);

	scene.events.emit("SYS_TRANSITION_OUT", {{duration}, {target->sys.settings.key}});

	_updateListener = scene.events.on("SYS_UPDATE", std::bind(
				&ScenePlugin::step,
				this,
				std::placeholders::_1
				));

	return true;
}

bool Zen::ScenePlugin::checkValidTransition (
		Zen::Scene* target
		)
{
	// Not a valid target if it doesn't exist, isn't active or is already
	// transitioning in or out
	if (target == nullptr) return false;

	if (target->sys.isActive() ||
		target->sys.isTransitioning() ||
		target == &scene ||
		scene.sys.isTransitioning()
		)
		return false;

	return true;
}

void Zen::ScenePlugin::step (
		Data data
		)
{
	int time = data.i.at(0);
	int delta = data.i.at(1);

	_elapsed += delta;

	transitionProgress = Math::clamp(_elapsed / _duration, 0, 1);

	if (_onUpdate != nullptr)
		_onUpdate({{}, {}, {transitionProgress}});

	if (_elapsed >= _duration)
		transitionComplete();
}

void Zen::ScenePlugin::transitionComplete ()
{
	// Stop the step
	scene.events.off("SYS_UPDATE", _updateListener);
	_updateListener = nullptr;

	// Notify the target scene
	_target->events.emit("SYS_TRANSITION_COMPLETE", {{}, {scene.sys.settings.key}});

	// Clear the target scene settings
	_target->sys.settings.isTransition = false;
	_target->sys.settings.transitionFrom = nullptr;

	// Clear local settings
	_duration = 0;
	_target = nullptr;
	_onUpdate = nullptr;

	// Now everything is clear we can handle what happens to this Scene
	if (_willRemove)
		manager.remove(scene.sys.settings.key);
	else if (_willSleep)
		scene.sys.sleep();
	else
		manager.stop(scene.sys.settings.key);
}

Zen::Scene& Zen::ScenePlugin::add (
		std::string key,
		std::unique_ptr<Zen::Scene> sceneToAdd,
		bool autoStart,
		Zen::Data data
		)
{
	manager.add(key, std::move(sceneToAdd), autoStart, data);
}

Zen::ScenePlugin& Zen::ScenePlugin::launch (
		std::string key,
		Zen::Data data
		)
{
	if (key != scene.sys.settings.key)
		manager.queueOp("start", key, "", data);

	return *this;
}

Zen::ScenePlugin& Zen::ScenePlugin::run (
		std::string key,
		Zen::Data data
		)
{
	if (key != scene.sys.settings.key)
		manager.queueOp("run", key, "", data);

	return *this;
}

Zen::ScenePlugin& Zen::ScenePlugin::pause (
		std::string key,
		Zen::Data data
		)
{
	if (key == "")
		key = scene.sys.settings.key;

	manager.queueOp("pause", key, "", data);

	return *this;
}

Zen::ScenePlugin& Zen::ScenePlugin::resume (
		std::string key,
		Zen::Data data
		)
{
	if (key == "")
		key = scene.sys.settings.key;

	manager.queueOp("resume", key, "", data);

	return *this;
}

Zen::ScenePlugin& Zen::ScenePlugin::sleep (
		std::string key,
		Zen::Data data
		)
{
	if (key == "")
		key = scene.sys.settings.key;

	manager.queueOp("sleep", key, "", data);

	return *this;
}

Zen::ScenePlugin& Zen::ScenePlugin::wake (
		std::string key,
		Zen::Data data
		)
{
	if (key == "")
		key = scene.sys.settings.key;

	manager.queueOp("wake", key, "", data);

	return *this;
}

Zen::ScenePlugin& Zen::ScenePlugin::swap (
		std::string key
		)
{
	if (key != scene.sys.settings.key)
		manager.queueOp("swap", scene.sys.settings.key, key);

	return *this;
}

Zen::ScenePlugin& Zen::ScenePlugin::stop (
		std::string key,
		Zen::Data data)
{
	if (key == "")
		key = scene.sys.settings.key;

	manager.queueOp("stop", key, "", data);

	return *this;
}

Zen::ScenePlugin& Zen::ScenePlugin::setVisible (
		bool value,
		std::string key
		)
{
	if (key == "")
		key = scene.sys.settings.key;

	manager.getScene(key)->sys.setVisible(value);

	return *this;
}

Zen::ScenePlugin& Zen::ScenePlugin::setActive (
		bool value,
		std::string key,
		Zen::Data data
		)
{
	if (key == "")
		key = scene.sys.settings.key;

	manager.getScene(key)->sys.setActive(value, data);

	return *this;
}

bool Zen::ScenePlugin::isSleeping (
		std::string key
		)
{
	if (key == "")
		key = scene.sys.settings.key;

	return manager.isSleeping(key);
}

bool Zen::ScenePlugin::isActive (
		std::string key
		)
{
	if (key == "")
		key = scene.sys.settings.key;

	return manager.isActive(key);
}

bool Zen::ScenePlugin::isPaused (
		std::string key
		)
{
	if (key == "")
		key = scene.sys.settings.key;

	return manager.isActive(key);
}

bool Zen::ScenePlugin::isVisible (
		std::string key
		)
{
	if (key == "")
		key = scene.sys.settings.key;

	return manager.isVisible(key);
}

Zen::ScenePlugin& Zen::ScenePlugin::swapPosition (
		std::string keyA,
		std::string keyB
		)
{
	if (keyB == "")
		keyB = scene.sys.settings.key;

	if (keyA != keyB)
		manager.swapPosition(keyA, keyB);

	return *this;
}

Zen::ScenePlugin& Zen::ScenePlugin::moveAbove (
		std::string keyA,
		std::string keyB
		)
{
	if (keyB == "")
		keyB = scene.sys.settings.key;

	if (keyA != keyB)
		manager.moveAbove(keyA, keyB);

	return *this;
}

Zen::ScenePlugin& Zen::ScenePlugin::moveBelow (
		std::string keyA,
		std::string keyB
		)
{
	if (keyB == "")
		keyB = scene.sys.settings.key;

	if (keyA != keyB)
		manager.moveBelow(keyA, keyB);

	return *this;
}

Zen::ScenePlugin& Zen::ScenePlugin::remove (
		std::string key
		)
{
	if (key == "")
		key = scene.sys.settings.key;

	manager.remove(key);

	return *this;
}

Zen::ScenePlugin& Zen::ScenePlugin::moveUp (
		std::string key
		)
{
	if (key == "")
		key = scene.sys.settings.key;

	manager.moveUp(key);

	return *this;
}

Zen::ScenePlugin& Zen::ScenePlugin::moveDown (
		std::string key
		)
{
	if (key == "")
		key = scene.sys.settings.key;

	manager.moveDown(key);

	return *this;
}

Zen::ScenePlugin& Zen::ScenePlugin::bringToTop (
		std::string key
		)
{
	if (key == "")
		key = scene.sys.settings.key;

	manager.bringToTop(key);

	return *this;
}

Zen::ScenePlugin& Zen::ScenePlugin::sendToBack (
		std::string key
		)
{
	if (key == "")
		key = scene.sys.settings.key;

	manager.sendToBack(key);

	return *this;
}

Zen::Scene& Zen::ScenePlugin::get (std::string key)
{
	return *manager.getScene(key);
}

int Zen::ScenePlugin::getIndex (std::string key)
{
	if (key == "")
		key = scene.sys.settings.key;

	return manager.getIndex(key);
}

void Zen::ScenePlugin::shutdown ()
{

}
