/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "scene_manager.hpp"

#include "../utils/messages.hpp"
#include "config.hpp"

namespace Zen {

extern EventEmitter g_event;

void SceneManager::boot (
		std::queue<std::function<std::unique_ptr<Scene>()>>* sceneFactory_)
{
	bool as_ = true;
	while (!sceneFactory_->empty())
	{
		auto sceneMaker_ = sceneFactory_->front();

		pending.emplace_back(
				"default",				// key
				sceneMaker_(),			// scene
				as_						// autoStart
				);

		sceneFactory_->pop();
		// Only set the first scene to autoStart, this can change later
		as_ = false;
	}
	// By the end of the loop, the game config's sceneFactory is empty.

	g_event.on("post-render", [this] (Uint32, Uint32) {
			isProcessing = false;
	});

	g_event.once("ready", &SceneManager::bootQueue, this);
}

void SceneManager::bootQueue ()
{
	if (isBooted)
		return;

	std::string key_;

	for (SceneConfig& entry_ : pending)
	{
		key_ = entry_.key;

		// Check if the scene has a key
		if (entry_.scene->sys.settings.key == "")
			entry_.scene->sys.settings.key = key_;

		// Boot the scene
		entry_.scene->sys.init();

		// Replace key in case the scene had one
		key_ = entry_.scene->sys.settings.key;

		// Any data to inject?
		if (bootData.find(key_) != bootData.end())
		{
			entry_.scene->sys.settings.data = bootData[key_].data;

			if (bootData[key_].autoStart)
				entry_.autoStart = true;
		}

		// Check if there are scenes to start up
		if (entry_.autoStart || entry_.scene->sys.settings.active)
			toStart.emplace_back(key_);

		// Store a reference to the Scene in `keys`
		keys.emplace(key_, *entry_.scene);

		// Move the unique pointer to the `scenes` vector
		scenes.emplace_back(std::move(entry_.scene));
	}

	// Clear the pending lists
	pending.clear();

	bootData.clear();

	isBooted = true;

	// Start might have been populated by the above
	for (const std::string& s_ : toStart)
		start(s_);

	toStart.clear();
}

void SceneManager::processQueue ()
{
	auto pendingLength_ = pending.size();
	auto queueLength_ = operationsQueue.size();

	if (pendingLength_ == 0 && queueLength_ == 0)
		return;

	if (pendingLength_)
	{
		for (auto entry_ = pending.begin(); entry_ != pending.end(); entry_++)
		{
			add(entry_->key, std::move(entry_->scene), entry_->autoStart,
					entry_->data);
		}

		// `toStart` might have been populated by `add`
		for (auto s_ : toStart)
			start(s_);

		// Clear the pending lists
		toStart.clear();
		pending.clear();

		return;
	}

	while (!operationsQueue.empty()) {
		auto entry_ = operationsQueue.front();
		auto op_ = entry_.operation;
		auto keyA_ = entry_.keyA;
		auto keyB_ = entry_.keyB;
		auto data_ = entry_.data;

		switch (op_)
		{
			case SCENE_OP::NONE:											break;
			case SCENE_OP::START:			start(keyA_, data_);			break;
			case SCENE_OP::RUN:				run(keyA_, data_);				break;
			case SCENE_OP::PAUSE:			pause(keyA_, data_);			break;
			case SCENE_OP::RESUME:			resume(keyA_, data_);			break;
			case SCENE_OP::SLEEP:			sleep(keyA_, data_);			break;
			case SCENE_OP::WAKE:			wake(keyA_, data_);				break;
			case SCENE_OP::SWAP:			swap(keyA_, keyB_);				break;
			case SCENE_OP::REMOVE:			remove(keyA_);					break;
			case SCENE_OP::BRING_TO_TOP:	bringToTop(keyA_);				break;
			case SCENE_OP::SEND_TO_BACK:	sendToBack(keyA_);				break;
			case SCENE_OP::MOVE_DOWN:		moveDown(keyA_);				break;
			case SCENE_OP::MOVE_UP:			moveUp(keyA_);					break;
			case SCENE_OP::MOVE_ABOVE:		moveAbove(keyA_, keyB_);		break;
			case SCENE_OP::MOVE_BELOW:		moveBelow(keyA_, keyB_);		break;
			case SCENE_OP::SWAP_POSITIONS:	swapPosition(keyA_, keyB_);		break;
		}

		operationsQueue.pop();
	}
}

Scene* SceneManager::add (
		std::string key_,
		std::unique_ptr<Scene> scene_,
		bool autoStart_,
		Data data_)
{
	// If processing or not booted then put scene into a holding pattern
	if (isProcessing || !isBooted)
	{
		pending.emplace_back(key_, std::move(scene_), autoStart_, data_);

		bootData.emplace(key_, SceneConfig{"", nullptr, false, data_});

		return nullptr;
	}

	if (scene_->sys.settings.key == "")
		scene_->sys.settings.key = key_;

	// Boot the scene
	scene_->sys.init();

	// Replace key in case the scene had one
	key_ = scene_->sys.settings.key;

	// Any data to inject?
	scene_->sys.settings.data = data_;

	auto scenePtr_ = scene_.get();

	// Store a reference to the Scene in `keys`
	keys.emplace(key_, *scene_);

	// Move the unique pointer to the `scenes` vector
	scenes.emplace_back(std::move(scene_));

	if (autoStart_ || scenePtr_->sys.settings.active)
	{
		if (pending.size() > 0)
			toStart.emplace_back(key_);
		else
			start(key_);
	}

	return scenePtr_;
}

SceneManager& SceneManager::remove (std::string key_)
{
	if (isProcessing)
	{
		operationsQueue.emplace(SCENE_OP::REMOVE, key_);
	}
	else
	{
		auto sceneToRemove_ = getScene(key_);

		// Stop if the scene doesn't exist or is transitioning.
		if (sceneToRemove_ == nullptr || sceneToRemove_->sys.isTransitioning())
			return *this;

		// Remove from the key map
		auto mapI_ = keys.find(key_);
		if (mapI_ != keys.end())
		{
			keys.erase(mapI_);

			// Remove from the scene vector
			for (auto s_ = scenes.begin(); s_ != scenes.end(); s_++)
			{
				if (s_->get() == sceneToRemove_)
				{
					scenes.erase(s_);
					break;
				}
			}

			// Remove from the toStart list if there too
			for (auto s_ = toStart.begin(); s_ != toStart.end(); s_++)
			{
				if (*s_ == key_)
				{
					toStart.erase(s_);
					break;
				}
			}
		}
	}

	return *this;
}

void SceneManager::bootScene (Scene *scene_)
{
	auto& sys_ = scene_->sys;
	auto& settings_ = sys_.settings;

	scene_->init(settings_.data);
	settings_.status = SCENE::INIT;

	if (settings_.isTransition)
		sys_.events.emit(
				"transition_init",
				settings_.transitionFrom,
				settings_.transitionDuration
				);

	scene_->load.reset();

	scene_->preload();

	/*
	 * TODO loader progress, maybe if I make something BIG later...
	if (scene->load.list.empty())
	{
		create(scene);
	}
	else
	{
		scene->sys.settings.status = SCENE::LOADING;

		scene->load.once("load_complete", &SceneManager::loadComplete, this);

		scene->load.start();
	}
	*/

	create(scene_);
}

void SceneManager::loadComplete (Scene* scene_)
{
	if (scene_ != nullptr)
		create(scene_);
}

void SceneManager::update (Uint32 time_, Uint32 delta_)
{
	processQueue();

	isProcessing = true;

	// Loop through the active scenes in reverse order
	for (int i_ = scenes.size() - 1; i_ >= 0; i_--)
	{
		auto& sys_ = scenes[i_]->sys;

		if (sys_.settings.status > SCENE::START &&
			sys_.settings.status <= SCENE::RUNNING
			)
			sys_.step(time_, delta_);
	}
}

void SceneManager::render ()
{
	// Loop through the scenes in forward order
	for (size_t i_ = 0; i_ < scenes.size(); i_++)
	{
		auto& sys_ = scenes[i_]->sys;

		if (sys_.settings.visible &&
			sys_.settings.status >= SCENE::LOADING &&
			sys_.settings.status < SCENE::SLEEPING
		   )
			sys_.render();
	}

	isProcessing = false;
}

void SceneManager::create (Scene *scene_)
{
	auto& sys_ = scene_->sys;
	auto& settings_ = sys_.settings;

	settings_.status = SCENE::CREATING;

	scene_->create(settings_.data);

	if (settings_.isTransition)
		sys_.events.emit(
				"transition-start",
				settings_.transitionFrom,
				settings_.transitionDuration
				);

	settings_.status = SCENE::RUNNING;

	sys_.events.emit("create");
}

std::vector<Scene*> SceneManager::getScenes (bool isActive_, bool inReverse_)
{
	std::vector<Scene*> out_;

	if (!inReverse_)
	{
		for (size_t i_ = 0; i_ < scenes.size(); i_++)
		{
			if (!isActive_ || (isActive_ && scenes.at(i_)->sys.isActive()))
				out_.emplace_back(scenes.at(i_).get());
		}
	}
	else
	{
		for (int i_ = scenes.size() - 1; i_ >= 0; i_--)
		{
			if (!isActive_ || (isActive_ && scenes.at(i_)->sys.isActive()))
				out_.emplace_back(scenes.at(i_).get());
		}
	}

	return out_;
}

Scene* SceneManager::getScene (std::string key_)
{
	auto scene_ = keys.find(key_);
	if (scene_ != keys.end())
		return &scene_->second;

	MessageWarning("The requested scene '", key_, "' does not exist");

	return nullptr;
}

bool SceneManager::isActive (std::string key_)
{
	auto scene_ = getScene(key_);

	if (scene_ != nullptr)
		return scene_->sys.isActive();

	return false;
}

bool SceneManager::isPaused (std::string key_)
{
	auto scene_ = getScene(key_);

	if (scene_ != nullptr)
		return scene_->sys.isPaused();

	return false;
}

bool SceneManager::isVisible (std::string key_)
{
	auto scene_ = getScene(key_);

	if (scene_ != nullptr)
		return scene_->sys.isVisible();

	return false;
}

bool SceneManager::isSleeping (std::string key_)
{
	auto scene_ = getScene(key_);

	if (scene_ != nullptr)
		return scene_->sys.isSleeping();

	return false;
}

SceneManager& SceneManager::pause (std::string key_, Data data_)
{
	auto scene_ = getScene(key_);

	if (scene_ != nullptr)
		scene_->sys.pause(data_);

	return *this;
}

SceneManager& SceneManager::resume (std::string key_, Data data_)
{
	auto scene_ = getScene(key_);

	if (scene_ != nullptr)
		scene_->sys.resume(data_);

	return *this;
}

SceneManager& SceneManager::sleep (std::string key_, Data data_)
{
	auto scene_ = getScene(key_);

	if (scene_ != nullptr)
		scene_->sys.sleep(data_);

	return *this;
}

SceneManager& SceneManager::wake (std::string key_, Data data_)
{
	auto scene_ = getScene(key_);

	if (scene_ != nullptr)
		scene_->sys.wake(data_);

	return *this;
}

SceneManager& SceneManager::run (std::string key_, Data data_)
{
	auto scene_ = getScene(key_);

	if (scene_ == nullptr)
	{
		for (size_t i_ = 0; i_ < pending.size(); i_++)
		{
			if (pending.at(i_).key == key_)
			{
				queueOp(SCENE_OP::START, key_, "", data_);
				break;
			}
		}
	}
	else
	{
		if (scene_->sys.isSleeping())
			// Sleeping?
			scene_->sys.wake(data_);
		else if (scene_->sys.isPaused())
			// Paused?
			scene_->sys.resume(data_);
		else
			// Not actually running?
			start(key_, data_);
	}

	return *this;
}

SceneManager& SceneManager::start (std::string key_, Data data_)
{
	// If the Scene Manager is not running, then put the Scene into a holding
	// pattern
	if (!isBooted)
	{
		auto d_ = bootData.find(key_);
		if (d_ != bootData.end())
		{
			d_->second.autoStart = true;
			d_->second.data = data_;
		}

		return *this;
	}

	auto scene_ = getScene(key_);

	if (scene_ != nullptr)
	{
		// If the Scene is already running (perhaps they called start from a
		// launched sub-Scene?) then we close it down before starting it again.
		if (!scene_->sys.isActive() && !scene_->sys.isPaused())
			scene_->sys.start(data_);

		bootScene(scene_);
	}

	return *this;
}

SceneManager& SceneManager::swap (std::string keyFrom_, std::string keyTo_)
{
	auto sceneA_ = getScene(keyFrom_);
	auto sceneB_ = getScene(keyTo_);

	if (sceneA_ != nullptr && sceneB_ != nullptr && keyFrom_ != keyTo_)
	{
		sleep(keyFrom_);

		if (isSleeping(keyTo_))
			wake(keyTo_);
		else
			start(keyTo_);
	}

	return *this;
}

Scene* SceneManager::getAt (int index_)
{
	return scenes.at(index_).get();
}

int SceneManager::getIndex (std::string key_)
{
	for (size_t i_ = 0; i_ < scenes.size(); i_++)
	{
		if (key_ == scenes[i_]->sys.settings.key)
			return i_;
	}

	return -1;
}

SceneManager& SceneManager::bringToTop (std::string key_)
{
	if (isProcessing)
	{
		operationsQueue.emplace(SCENE_OP::BRING_TO_TOP, key_, "");
	}
	else
	{
		int index_ = getIndex(key_);

		if (index_ != -1 && static_cast<unsigned int>(index_) < scenes.size())
		{
			std::unique_ptr<Scene> scene_ = std::move(scenes.at(index_));
			scenes.erase(scenes.begin() + index_);
			scenes.emplace_back(std::move(scene_));
		}
	}

	return *this;
}

SceneManager& SceneManager::sendToBack (std::string key_)
{
	if (isProcessing)
	{
		operationsQueue.emplace(SCENE_OP::SEND_TO_BACK, key_, "");
	}
	else
	{
		int index_ = getIndex(key_);

		if (index_ != -1 && static_cast<unsigned int>(index_) < scenes.size())
		{
			std::unique_ptr<Scene> scene_ = std::move(scenes.at(index_));
			scenes.erase(scenes.begin() + index_);
			scenes.emplace(scenes.begin(), move(scene_));
		}
	}

	return *this;
}

SceneManager& SceneManager::moveDown (std::string key_)
{
	if (isProcessing)
	{
		operationsQueue.emplace(SCENE_OP::MOVE_DOWN, key_, "");
	}
	else
	{
		int index_ = getIndex(key_);

		if (index_ > 0)
		{
			scenes.at(index_).swap(scenes.at(index_-1));
		}
	}

	return *this;
}

SceneManager& SceneManager::moveUp (std::string key_)
{
	if (isProcessing)
	{
		operationsQueue.emplace(SCENE_OP::MOVE_UP, key_, "");
	}
	else
	{
		int index_ = getIndex(key_);

		if (static_cast<unsigned int>(index_) < scenes.size() - 1)
		{
			scenes.at(index_).swap(scenes.at(index_+1));
		}
	}

	return *this;
}

SceneManager& SceneManager::moveAbove (std::string keyA_, std::string keyB_)
{
	if (keyA_ == keyB_)
		return *this;

	if (isProcessing)
	{
		operationsQueue.emplace(SCENE_OP::MOVE_ABOVE, keyA_, keyB_);
	}
	else
	{
		int indexA_ = getIndex(keyA_);
		int indexB_ = getIndex(keyB_);

		if (indexA_ != -1 && indexB_ != -1)
		{
			std::unique_ptr<Scene> tempScene_ = std::move(scenes.at(indexB_));
			scenes.erase(scenes.begin() + indexB_);

			if (static_cast<unsigned int>(indexA_) >= scenes.size() - 1)
			{
				scenes.emplace_back(std::move(tempScene_));
			}
			else
			{
				scenes.emplace(scenes.begin() + indexA_ + 1, std::move(tempScene_));
			}
		}
	}

	return *this;
}

SceneManager& SceneManager::moveBelow (std::string keyA_, std::string keyB_)
{
	if (keyA_ == keyB_)
		return *this;

	if (isProcessing)
	{
		operationsQueue.emplace(SCENE_OP::MOVE_BELOW, keyA_, keyB_);
	}
	else
	{
		int indexA_ = getIndex(keyA_);
		int indexB_ = getIndex(keyB_);

		if (indexA_ != -1 && indexB_ != -1)
		{
			std::unique_ptr<Scene> tempScene_ = std::move(scenes.at(indexB_));
			scenes.erase(scenes.begin() + indexB_);

			scenes.emplace(scenes.begin() + indexA_, std::move(tempScene_));
		}
	}

	return *this;
}

SceneManager& SceneManager::queueOp (
		SCENE_OP operation_,
		std::string keyA_,
		std::string keyB_,
		Data data_)
{
	operationsQueue.emplace(operation_, keyA_, keyB_, data_);

	return *this;
}

SceneManager& SceneManager::swapPosition (
		std::string keyA_,
		std::string keyB_)
{
	if (keyA_ == keyB_)
		return *this;

	if (isProcessing)
	{
		operationsQueue.emplace(SCENE_OP::SWAP_POSITIONS, keyA_, keyB_);
	}
	else
	{
		int indexA_ = getIndex(keyA_);
		int indexB_ = getIndex(keyB_);

		if (indexA_ != -1 && indexB_ != -1)
		{
			scenes.at(indexA_).swap(scenes.at(indexB_));
		}
	}

	return *this;
}

}	// namespace Zen
