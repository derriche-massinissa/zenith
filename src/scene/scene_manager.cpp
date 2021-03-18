/**
 * @file		scene_manager.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "scene_manager.h"

Zen::SceneManager::SceneManager (
	Zen::Game& g,
	std::queue<std::function<std::unique_ptr<Zen::Scene>(Zen::Game&)>>& sceneFactory)
	: game(g)
{
	isProcessing = false;
	isBooted = false;

	bool as = true;
	while (!sceneFactory.empty()) {
		auto sceneMaker = sceneFactory.front();

		_pending.emplace_back(
			"default",			// key
			sceneMaker(game),	// scene
			as					// autoStart
			);

		sceneFactory.pop();
		as = false;
	}
	// By the end of the loop, the game config's sceneFactory is empty.

	game.events.once("SYS_READY", &SceneManager::bootQueue, this);
}

Zen::SceneManager::~SceneManager ()
{
	scenes.clear();

	keys.clear();

	_start.clear();

	_pending.clear();
}

void Zen::SceneManager::bootQueue (Zen::Data data)
{
	if (isBooted)
		return;

	SceneConfig* entry;
	std::string key;

	for (int i = 0; i < _pending.size(); i++) {
		entry = &_pending.at(i);

		key = entry->key;

		// Check if the scene has a key
		if (entry->scene->sys.settings.key == "")
			entry->scene->sys.settings.key = key;

		entry->scene->sys.init();

		// Replace key in case the scene had one
		key = entry->scene->sys.settings.key;

		// Any data to inject?
		if (_data.find(key) != _data.end()) {
			entry->scene->sys.settings.data = _data.find(key)->second.data;

			if (_data.find(key)->second.autoStart)
				entry->autoStart = true;
		}

		// Check if there are scenes to start up
		if (entry->autoStart || entry->scene->sys.settings.active)
			_start.emplace_back(key);

		// Store a reference to the Scene in `keys`
		keys.emplace(key, *entry->scene);

		// Move the unique pointer to the `scenes` vector
		scenes.emplace_back(std::move(entry->scene));
	}

	// Clear the pending lists
	_pending.clear();

	_data.clear();

	isBooted = true;

	// Start might have been populated by the above
	for (auto s : _start)
		start(s);

	_start.clear();
}

void Zen::SceneManager::processQueue ()
{
	auto pendingLength = _pending.size();
	auto queueLength = _queue.size();

	if (pendingLength == 0 && queueLength == 0)
		return;

	if (pendingLength) {
		SceneConfig* entry;

		for (int i = 0; i < pendingLength; i++) {
			entry = &_pending[i];
			add(entry->key, std::move(entry->scene), entry->autoStart, entry->data);
		}

		// `_start` might have been populated by `add`
		for (auto s : _start)
			start(s);

		// Clear the pending lists
		_start.clear();
		_pending.clear();

		return;
	}

	while (!_queue.empty()) {
		auto entry = _queue.front();
		auto op = entry.operation;
		auto keyA = entry.keyA;
		auto keyB = entry.keyB;
		auto data = entry.data;

		if (op == "start")
			start(keyA, data);
		else if (op == "run")
			run(keyA, data);
		else if (op == "pause")
			pause(keyA, data);
		else if (op == "resume")
			resume(keyA, data);
		else if (op == "sleep")
			sleep(keyA, data);
		else if (op == "wake")
			wake(keyA, data);
		else if (op == "swap")
			swap(keyA, keyB);
		else if (op == "stop")
			stop(keyA, data);
		else if (op == "remove")
			remove(keyA);
		else if (op == "bringToTop")
			bringToTop(keyA);
		else if (op == "sendToBack")
			sendToBack(keyA);
		else if (op == "moveDown")
			moveDown(keyA);
		else if (op == "moveUp")
			moveUp(keyA);
		else if (op == "moveAbove")
			moveAbove(keyA, keyB);
		else if (op == "moveBelow")
			moveBelow(keyA, keyB);
		else if (op == "swapPosition")
			swapPosition(keyA, keyB);
		else
			messageWarning("Operation unknown: ", op);

		_queue.pop();
	}
}

Zen::Scene* Zen::SceneManager::add (
	std::string key,
	std::unique_ptr<Zen::Scene> scene,
	bool autoStart,
	Zen::Data data)
{
	// If processing or not booted then put scene into a holding pattern
	if (isProcessing || !isBooted) {
		_pending.emplace_back(
			key,
			std::move(scene),
			autoStart,
			data
			);

		_data.emplace(key, SceneConfig{"", nullptr, false, data});

		return nullptr;
	}

	key = (scene->sys.settings.key != "") ? scene->sys.settings.key : key;

	// Any data to inject?
	scene->sys.settings.data = data;

	auto scenePtr = scene.get();

	// Store a reference to the Scene in `keys`
	keys.emplace(key, *scene);

	// Move the unique pointer to the `scenes` vector
	scenes.emplace_back(std::move(scene));

	if (autoStart || scenePtr->sys.settings.active) {
		if (_pending.size())
			_start.emplace_back(key);
		else
			start(key);
	}

	return scenePtr;
}

Zen::SceneManager& Zen::SceneManager::remove (std::string key)
{
	if (isProcessing) {
		_queue.emplace("remove", key);
	} else {
		auto sceneToRemove = getScene(key);

		// Stop if the scene doesn't exist or is transitioning.
		if (sceneToRemove == nullptr || sceneToRemove->sys.isTransitioning())
			return *this;

		// Remove from the key map
		auto mapI = keys.find(key);
		if (mapI != keys.end())
			keys.erase(mapI);

		// Remove from the scene vector
		for (int i = 0; i < scenes.size(); i++) {
			if (sceneToRemove == scenes.at(i).get()) {
				scenes.erase(scenes.begin() + i);
				break;
			}
		}

		// Remove from the _start list if there too
		for (int i = 0; i < _start.size(); i++) {
			if (key == _start.at(i)) {
				_start.erase(_start.begin() + i);
				break;
			}
		}
	}

	return *this;
}

void Zen::SceneManager::bootScene (Zen::Scene *scene)
{
	/* TODO INIT AND CO.
	scene->init(scene->sys.settings.data);
	scene->sys.settings.status = SCENE::INIT;

	if (scene->sys.settings.isTransition)
		scene->events.emit(
			"SYS_TRANSITION_INIT",
			{
				{scene->sys.settings.transitionDuration},
				{scene->sys.settings.transitionFrom->sys.settings.key}
			});

	scene->load.reset();

	scene->preload();

	if (scene->load.list.empty())
		create(scene);
	else {
		scene->sys.settings.status = SCENE::LOADING;

		scene->events.once("SYS_LOAD_COMPLETE", &SceneManager::loadComplete, this);

		scene->load.start();
	}
	*/
}

void Zen::SceneManager::loadComplete (Zen::Data data)
{
	Scene* scene = getScene(data.s.at(0));

	if (scene != nullptr)
		create(scene);
}

void Zen::SceneManager::update (Uint32 time, Uint32 delta)
{
	processQueue();

	isProcessing = true;

	// Loop through the active scenes in reverse order
	for (int i = scenes.size() - 1; i >= 0; i--) {
		if (
			scenes.at(i)->sys.settings.status > SCENE::START &&
			scenes.at(i)->sys.settings.status <= SCENE::RUNNING
			)
			scenes.at(i)->sys.step(time, delta);
	}
}

void Zen::SceneManager::render ()
{
	// Loop through the scenes in forward order
	for (int i = 0; i < scenes.size(); i++) {
		if (
			scenes.at(i)->sys.settings.visible &&
			scenes.at(i)->sys.settings.status >= SCENE::LOADING &&
			scenes.at(i)->sys.settings.status < SCENE::SLEEPING
			)
			scenes.at(i)->sys.render();
	}

	isProcessing = false;
}

void Zen::SceneManager::create (Zen::Scene *scene)
{
	auto sys = &scene->sys;
	auto settings = &scene->sys.settings;

	settings->status = SCENE::CREATING;

	scene->create(settings->data);

	if (settings->status == SCENE::DESTROYED)
		return;

	if (settings->isTransition)
		scene->events.emit(
				"SYS_TRANSITION_START",
				{{settings->transitionDuration}, {settings->transitionFrom->sys.settings.key}}
				);

	settings->status = SCENE::RUNNING;

	scene->events.emit("SYS_CREATE");
}

std::vector<Zen::Scene*> Zen::SceneManager::getScenes (
		bool isActive,
		bool inReverse)
{
	std::vector<Scene*> out;

	if (!inReverse) {
		for (int i = 0; i < scenes.size(); i++) {
			if (!isActive || (isActive && scenes.at(i)->sys.isActive()))
				out.emplace_back(scenes.at(i).get());
		}
	} else {
		for (int i = scenes.size() - 1; i >= 0; i++) {
			if (!isActive || (isActive && scenes.at(i)->sys.isActive()))
				out.emplace_back(scenes.at(i).get());
		}
	}

	return out;
}

Zen::Scene*
Zen::SceneManager::getScene (std::string key)
{
	auto scene = keys.find(key);
	if (scene != keys.end())
		return &scene->second;

	return nullptr;
}

bool Zen::SceneManager::isActive (std::string key)
{
	auto scene = getScene(key);

	if (scene != nullptr)
		return scene->sys.isActive();

	return false;
}

bool Zen::SceneManager::isPaused (std::string key)
{
	auto scene = getScene(key);

	if (scene != nullptr)
		return scene->sys.isPaused();

	return false;
}

bool Zen::SceneManager::isVisible (std::string key)
{
	auto scene = getScene(key);

	if (scene != nullptr)
		return scene->sys.isVisible();

	return false;
}

bool Zen::SceneManager::isSleeping (std::string key)
{
	auto scene = getScene(key);

	if (scene != nullptr)
		return scene->sys.isSleeping();

	return false;
}

Zen::SceneManager& Zen::SceneManager::pause (std::string key, Zen::Data data)
{
	auto scene = getScene(key);

	if (scene != nullptr)
		scene->sys.pause(data);

	return *this;
}

Zen::SceneManager& Zen::SceneManager::resume (std::string key, Zen::Data data)
{
	auto scene = getScene(key);

	if (scene != nullptr)
		scene->sys.resume(data);

	return *this;
}

Zen::SceneManager& Zen::SceneManager::sleep (std::string key, Zen::Data data)
{
	auto scene = getScene(key);

	if (scene != nullptr)
		scene->sys.sleep(data);

	return *this;
}

Zen::SceneManager& Zen::SceneManager::wake (std::string key, Zen::Data data)
{
	auto scene = getScene(key);

	if (scene != nullptr)
		scene->sys.wake(data);

	return *this;
}

Zen::SceneManager& Zen::SceneManager::run (std::string key, Zen::Data data)
{
	auto scene = getScene(key);

	if (scene == nullptr) {
		for (int i = 0; i < _pending.size(); i++) {
			if (_pending.at(i).key == key) {
				queueOp("start", key, "", data);
				break;
			}
		}
	} else {
		if (scene->sys.isSleeping())
			// Sleeping?
			scene->sys.wake(data);
		else if (scene->sys.isPaused())
			// Paused?
			scene->sys.resume(data);
		else
			// Not actually running?
			start(key, data);
	}

	return *this;
}

Zen::SceneManager& Zen::SceneManager::start (std::string key, Zen::Data data)
{
	// If the Scene Manager is not running, then put the Scene into a holding pattern
	if (!isBooted) {
		auto d = _data.find(key);
		if (d != _data.end()) {
			d->second.autoStart = true;
			d->second.data = data;
		}

		return *this;
	}

	auto scene = getScene(key);

	if (scene != nullptr) {
		// If the Scene is already running (perhaps they called start from a launched sub-Scene?)
		// then we close it down before starting it again.
		if (scene->sys.isActive() || scene->sys.isPaused()) {
			scene->sys.shutdown();
			scene->sys.start(data);
		} else {
			scene->sys.start(data);
		}

		bootScene(scene);
	}

	return *this;
}

Zen::SceneManager& Zen::SceneManager::stop (std::string key, Zen::Data data)
{
	auto scene = getScene(key);

	if (scene != nullptr && !scene->sys.isTransitioning())
		scene->sys.shutdown(data);

	return *this;
}

Zen::SceneManager& Zen::SceneManager::swap (std::string keyFrom, std::string keyTo)
{
	auto sceneA = getScene(keyFrom);
	auto sceneB = getScene(keyTo);

	if (sceneA != nullptr && sceneB != nullptr && keyFrom != keyTo) {
		sleep(keyFrom);

		if (isSleeping(keyTo))
			wake(keyTo);
		else
			start(keyTo);
	}

	return *this;
}

Zen::Scene* Zen::SceneManager::getAt (int index)
{
	return scenes.at(index).get();
}

int Zen::SceneManager::getIndex (std::string key)
{
	auto scene = getScene(key);

	if (scene == nullptr)
		return -1;

	for (int i = 0; i < scenes.size(); i++) {
		if (scene == scenes.at(i).get())
			return i;
	}

	return -1;
}

Zen::SceneManager& Zen::SceneManager::bringToTop (std::string key)
{
	if (isProcessing) {
		_queue.emplace("bringToTop", key, "");
	} else {
		int index = getIndex(key);

		if (index != -1 && index < scenes.size()) {
			std::unique_ptr<Scene> scene = std::move(scenes.at(index));
			scenes.erase(scenes.begin() + index);
			scenes.emplace_back(std::move(scene));
		}
	}

	return *this;
}

Zen::SceneManager& Zen::SceneManager::sendToBack (std::string key)
{
	if (isProcessing) {
		_queue.emplace("sendToBack", key, "");
	} else {
		int index = getIndex(key);

		if (index != -1 && index < scenes.size()) {
			std::unique_ptr<Scene> scene = std::move(scenes.at(index));
			scenes.erase(scenes.begin() + index);
			scenes.emplace(scenes.begin(), move(scene));
		}
	}

	return *this;
}

Zen::SceneManager& Zen::SceneManager::moveDown (std::string key)
{
	if (isProcessing) {
		_queue.emplace("moveDown", key, "");
	} else {
		int index = getIndex(key);

		if (index > 0) {
			scenes.at(index).swap(scenes.at(index-1));
		}
	}

	return *this;
}

Zen::SceneManager& Zen::SceneManager::moveUp (std::string key)
{
	if (isProcessing) {
		_queue.emplace("moveUp", key, "");
	} else {
		int index = getIndex(key);

		if (index < scenes.size() - 1) {
			scenes.at(index).swap(scenes.at(index+1));
		}
	}

	return *this;
}

Zen::SceneManager& Zen::SceneManager::moveAbove (std::string keyA, std::string keyB)
{
	if (keyA == keyB)
		return *this;

	if (isProcessing) {
		_queue.emplace("moveAbove", keyA, keyB);
	} else {
		int indexA = getIndex(keyA);
		int indexB = getIndex(keyB);

		if (indexA != -1 && indexB != -1) {
			std::unique_ptr<Scene> tempScene = std::move(scenes.at(indexB));
			scenes.erase(scenes.begin() + indexB);

			if (indexA >= scenes.size() - 1) {
				scenes.emplace_back(std::move(tempScene));
			} else {
				scenes.emplace(scenes.begin() + indexA + 1, std::move(tempScene));
			}
		}
	}

	return *this;
}

Zen::SceneManager& Zen::SceneManager::moveBelow (std::string keyA, std::string keyB)
{
	if (keyA == keyB)
		return *this;

	if (isProcessing) {
		_queue.emplace("moveBelow", keyA, keyB);
	} else {
		int indexA = getIndex(keyA);
		int indexB = getIndex(keyB);

		if (indexA != -1 && indexB != -1) {
			std::unique_ptr<Scene> tempScene = std::move(scenes.at(indexB));
			scenes.erase(scenes.begin() + indexB);

			scenes.emplace(scenes.begin() + indexA, std::move(tempScene));
		}
	}

	return *this;
}

Zen::SceneManager& Zen::SceneManager::queueOp (
	std::string operation,
	std::string keyA,
	std::string keyB,
	Zen::Data data)
{
	_queue.emplace(operation, keyA, keyB, data);

	return *this;
}

Zen::SceneManager& Zen::SceneManager::swapPosition (
	std::string keyA,
	std::string keyB)
{
	if (keyA == keyB)
		return *this;

	if (isProcessing) {
		_queue.emplace("swapPosition", keyA, keyB);
	} else {
		int indexA = getIndex(keyA);
		int indexB = getIndex(keyB);

		if (indexA != -1 && indexB != -1) {
			scenes.at(indexA).swap(scenes.at(indexB));
		}
	}

	return *this;
}
