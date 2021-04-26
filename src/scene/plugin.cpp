/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "plugin.hpp"

#include "scene.hpp"
#include "systems.fwd.hpp"
#include "scene_manager.hpp"
#include "const.hpp"
#include <algorithm>

namespace Zen {

extern SceneManager g_scene;

ScenePlugin::ScenePlugin (Scene* scene_)
	: scene(scene_)
	, systems (&scene_->sys)
	, key (scene_->sys.settings.key)
{
	scene_->sys.events.on("start", &ScenePlugin::pluginStart, this);
}

ScenePlugin& ScenePlugin::start (std::string key_, Data data_)
{
	if (key_ == "")
		key_ = key;

	g_scene.queueOp(SCENE_OP::STOP, key);
	g_scene.queueOp(SCENE_OP::START, key_, "", data_);

	return *this;
}

ScenePlugin& ScenePlugin::restart (Data data_)
{
	g_scene.queueOp(SCENE_OP::STOP, key);
	g_scene.queueOp(SCENE_OP::START, key, "", data_);

	return *this;
}

bool ScenePlugin::transition (SceneTransitionConfig config_)
{
	std::string key_ = config_.target;
	Scene* target_ = g_scene.getScene(key_);

	if (!checkValidTransition(target_))
		return false;

	Uint32 duration_ = config_.duration;

	transitionElapsed = 0;
	transitionTarget = target_;
	transitionDuration = duration_;
	transitionWillSleep = config_.sleep;
	transitionWillRemove = config_.remove;

	if (config_.onUpdate)
	{
		transitionCallback = std::bind(
				config_.onUpdate,
				config_.onUpdateScope,
				std::placeholders::_1
				);
	}

	systems->settings.transitionAllowInput = config_.allowInput;

	// Configure the target
	auto& targetSettings_ = target_->sys.settings;

	targetSettings_.isTransition = true;
	targetSettings_.transitionFrom = scene;
	targetSettings_.transitionDuration = duration_;
	targetSettings_.transitionAllowInput = config_.allowInput;

	if (config_.moveAbove)
		g_scene.moveAbove(key, key_);
	else if (config_.moveBelow)
		g_scene.moveBelow(key, key_);

	if (target_->sys.isSleeping())
		target_->sys.wake(config_.data);
	else
		g_scene.start(key_, config_.data);

	systems->events.emit("transition-out", key_, duration_);

	lUpdate = systems->events.on("update", &ScenePlugin::step, this);

	return true;
}

bool ScenePlugin::checkValidTransition (Scene* target_)
{
	// Not a valid target if it doesn't exist, isn't active or is already
	// transitioning in or out
	if (target_ == nullptr) return false;

	if (target_->sys.isActive() || target_->sys.isTransitioning() || target_ == scene || systems->isTransitioning())
	{
		return false;
	}

	return true;
}

void ScenePlugin::step ([[maybe_unused]] Uint32 time_, Uint32 delta_)
{
	transitionElapsed += delta_;

	transitionProgress = std::clamp(static_cast<double>(transitionElapsed) / transitionDuration, 0., 1.);

	if (transitionCallback)
		transitionCallback(transitionProgress);

	if (transitionElapsed >= transitionDuration)
		transitionComplete();
}

void ScenePlugin::transitionComplete ()
{
	// Stop the step
	systems->events.off(lUpdate);

	// Notify the target scene
	transitionTarget->sys.events.emit("transition-complete", key);

	// Clear the target scene settings
	transitionTarget->sys.settings.isTransition = false;
	transitionTarget->sys.settings.transitionFrom = nullptr;

	// Clear local settings
	transitionDuration = 0;
	transitionTarget = nullptr;
	transitionCallback = nullptr;

	// Now everything is clear we can handle what happens to this Scene
	if (transitionWillRemove)
		g_scene.remove(key);
	else if (transitionWillSleep)
		systems->sleep();
	else
		g_scene.stop(key);
}

Scene& ScenePlugin::add (
		std::string key_,
		std::unique_ptr<Scene> sceneToAdd_,
		bool autoStart_,
		Data data_
		)
{
	return *g_scene.add(key_, std::move(sceneToAdd_), autoStart_, data_);
}

ScenePlugin& ScenePlugin::launch (std::string key_, Data data_)
{
	if (key_ != key)
		g_scene.queueOp(SCENE_OP::START, key_, "", data_);

	return *this;
}

ScenePlugin& ScenePlugin::run (std::string key_, Data data_)
{
	if (key_ != key)
		g_scene.queueOp(SCENE_OP::RUN, key_, "", data_);

	return *this;
}

ScenePlugin& ScenePlugin::pause (std::string key_, Data data_)
{
	if (key_ == "")
		key_ = key;

	g_scene.queueOp(SCENE_OP::PAUSE, key_, "", data_);

	return *this;
}

ScenePlugin& ScenePlugin::resume (std::string key_, Data data_)
{
	if (key_ == "")
		key_ = key;

	g_scene.queueOp(SCENE_OP::RESUME, key_, "", data_);

	return *this;
}

ScenePlugin& ScenePlugin::sleep (std::string key_, Data data_)
{
	if (key_ == "")
		key_ = key;

	g_scene.queueOp(SCENE_OP::SLEEP, key_, "", data_);

	return *this;
}

ScenePlugin& ScenePlugin::wake (std::string key_, Data data_)
{
	if (key_ == "")
		key_ = key;

	g_scene.queueOp(SCENE_OP::WAKE, key_, "", data_);

	return *this;
}

ScenePlugin& ScenePlugin::swap (std::string key_)
{
	if (key_ != key)
		g_scene.queueOp(SCENE_OP::SWAP, key, key_);

	return *this;
}

ScenePlugin& ScenePlugin::stop (std::string key_, Data data_)
{
	if (key_ == "")
		key_ = key;

	g_scene.queueOp(SCENE_OP::STOP, key_, "", data_);

	return *this;
}

ScenePlugin& ScenePlugin::setVisible (bool value_, std::string key_)
{
	if (key_ == "")
		key_ = key;

	g_scene.getScene(key_)->sys.setVisible(value_);

	return *this;
}

ScenePlugin& ScenePlugin::setActive (bool value_, std::string key_, Data data_)
{
	if (key_ == "")
		key_ = key;

	g_scene.getScene(key_)->sys.setActive(value_, data_);

	return *this;
}

bool ScenePlugin::isSleeping (std::string key_)
{
	if (key_ == "")
		key_ = key;

	return g_scene.isSleeping(key_);
}

bool ScenePlugin::isActive (std::string key_)
{
	if (key_ == "")
		key_ = key;

	return g_scene.isActive(key_);
}

bool ScenePlugin::isPaused (std::string key_)
{
	if (key_ == "")
		key_ = key;

	return g_scene.isActive(key_);
}

bool ScenePlugin::isVisible (std::string key_)
{
	if (key_ == "")
		key_ = key;

	return g_scene.isVisible(key_);
}

ScenePlugin& ScenePlugin::swapPosition (std::string keyA_, std::string keyB_)
{
	if (keyB_ == "")
		keyB_ = key;

	if (keyA_ != keyB_)
		g_scene.swapPosition(keyA_, keyB_);

	return *this;
}

ScenePlugin& ScenePlugin::moveAbove (std::string keyA_, std::string keyB_)
{
	if (keyB_ == "")
		keyB_ = key;

	if (keyA_ != keyB_)
		g_scene.moveAbove(keyA_, keyB_);

	return *this;
}

ScenePlugin& ScenePlugin::moveBelow (std::string keyA_, std::string keyB_)
{
	if (keyB_ == "")
		keyB_ = key;

	if (keyA_ != keyB_)
		g_scene.moveBelow(keyA_, keyB_);

	return *this;
}

ScenePlugin& ScenePlugin::remove (std::string key_)
{
	if (key_ == "")
		key_ = key;

	g_scene.remove(key_);

	return *this;
}

ScenePlugin& ScenePlugin::moveUp (std::string key_)
{
	if (key_ == "")
		key_ = key;

	g_scene.moveUp(key_);

	return *this;
}

ScenePlugin& ScenePlugin::moveDown (std::string key_)
{
	if (key_ == "")
		key_ = key;

	g_scene.moveDown(key_);

	return *this;
}

ScenePlugin& ScenePlugin::bringToTop (std::string key_)
{
	if (key_ == "")
		key_ = key;

	g_scene.bringToTop(key_);

	return *this;
}

ScenePlugin& ScenePlugin::sendToBack (std::string key_)
{
	if (key_ == "")
		key_ = key;

	g_scene.sendToBack(key_);

	return *this;
}

Scene& ScenePlugin::get (std::string key_)
{
	return *g_scene.getScene(key_);
}

int ScenePlugin::getIndex (std::string key_)
{
	if (key_ == "")
		key_ = key;

	return g_scene.getIndex(key_);
}

}	// namespace Zen
