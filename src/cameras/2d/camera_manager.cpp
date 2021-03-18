/**
 * @file		camera_manager.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "camera_manager.h"

Zen::CameraManager::CameraManager (Zen::Scene& s)
	: scene(s)
{
	scene.events.once("SYS_BOOT", &CameraManager::boot, this);

	scene.events.on("SYS_START", &CameraManager::start, this);
}

Zen::CameraManager::~CameraManager ()
{
	shutdown();

	scene.events.off("SYS_START", &CameraManager::start, this);
}

void Zen::CameraManager::boot (Zen::Data data)
{
	if (scene.sys.settings.cameras.size())
		// We have cameras to create
		fromConfig(scene.sys.settings.cameras);
	else
		// Make one
		add();

	main = &cameras.at(0);

	// Configure the default camera (It already exists)
	def.setViewpointX(0)
		.setViewpointY(0)
		.setViewpointWidth(scene.sys.scale.width)
		.setViewpointHeight(scene.sys.scale.height)
		.setScene(&scene);

	scene.events.on("SYS_RESIZE", &CameraManager::onResize, this);
}

void Zen::CameraManager::start (Zen::Data data)
{
	if (main == nullptr) {
		if (scene.sys.settings.cameras.size())
			// We have cameras to create
			fromConfig(scene.sys.settings.cameras);
		else
			// Make one
			add();

		main = &cameras.at(0);
	}

	scene.events.on("SYS_UPDATE", &CameraManager::update, this);
	scene.events.on("SYS_SHUTDOWN", &CameraManager::shutdown, this);
}

Zen::Camera* Zen::CameraManager::add (
	int x, int y, int width, int height, bool makeMain, std::string name)
{
	cameras.emplace_back(x, y, width, height);
	Camera& camera = cameras.back();

	camera.setName(name)
		.setScene(&scene);

	camera.id = getNextID();

	if (makeMain)
		main = &camera;

	return &camera;
}

int Zen::CameraManager::getNextID ()
{
	int testID = 1;

	// Find the first free camera ID we can use
	for (int t = 0; t < 32; t++) {
		bool found = false;

		for (int i = 0; i < cameras.size(); i++) {
			if (cameras.at(i).id == testID) {
				found = true;
				break;
			}
		}

		if (found)
			testID = testID << 1;
		else
			return testID;
	}

	return 0;
}

int Zen::CameraManager::getTotal (bool isVisible)
{
	int total = 0;

	for (int i = 0; i < cameras.size(); i++) {
		if (!isVisible || (isVisible && cameras.get(i).visible))
			total++;
	}

	return total;
}

Zen::CameraManager& Zen::CameraManager::fromConfig (
	std::vector<Zen::CameraConfig> config)
{
	int gameWidth = scene.sys.scale.width;
	int gameHeight = scene.sys.scale.height;

	for (int i = 0; i < config.size(); i++) {
		auto camera = add(
				config[i].x,
				config[i].y,
				config[i].width,
				config[i].height
			);

		// Direct properties
		camera->name = config[i].name;
		camera->zoom = config[i].zoom;
		camera->rotation = config[i].rotation;
		camera->scrollX = config[i].scrollX;
		camera->scrollY = config[i].scrollY;
		camera->visible = config[i].visible;
		camera->backgroundColor = config[i].backgroundColor;

		camera->setBounds(
				config[i].bounds.x,
				config[i].bounds.y,
				config[i].bounds.width,
				config[i].bounds.height
			);
	}

	return *this;
}

Zen::Camera* Zen::CameraManager::getCamera (std::string name)
{
	for (int i = 0; i < cameras.size(); i++) {
		if (cameras[i].name == name)
			return &cameras[i];
	}

	return nullptr;
}

std::vector<Zen::Camera*> Zen::CameraManager::getCamerasBelowPointer (
	Zen::Pointer pointer)
{
	std::vector<Camera*> output;

	// So the top-most camera is at the top of the search vector
	for (int i = cameras.size() - 1; i >= 0; i++) {
		if (cameras[i].visible &&
			cameras[i].inputEnabled &&
			Geom::Rectangle::contains(
				cameras[i].x,
				cameras[i].y,
				cameras[i].width,
				cameras[i].height,
				pointer.x,
				pointer.y)
			)
		{
			output.emplace_back(&cameras[i]);
		}
	}

	return output;
}

int Zen::CameraManager::remove (
	std::vector<Zen::Camera*> camerasToRemove)
{
	int total = 0;

	for (auto it = cameras.begin(); it != cameras.end(); it++) {
		for (auto c : camerasToRemove) {
			if (*c == &*it) {
				if (*c == main) main = nullptr;
				cameras.erase(it);
				total++;
			}
		}
	}

	if (main == nullptr && cameras.size())
		main = &cameras[0];

	return total;
}

int Zen::CameraManager::remove (Zen::Camera* cameraToRemove)
{
	std::vector<Camera*> camerasToRemove {cameraToRemove};

	return remove(camerasToRemove);
}

void Zen::CameraManager::render (Zen::Renderer& renderer, Zen::DisplayList& displayList)
{
	for (int i = 0; i < cameras.size(); i++) {
		if (cameras[i].visible && cameras[i].alpha > 0) {
			cameras[i].preRender();

			auto visibleChildren = getVisibleChildren(displayList.getChildren(), cameras[i]);

			renderer.render(scene, visibleChildren, cameras[i]);
		}
	}
}

std::vector<Zen::GameObject*> Zen::CameraManager::getVisibleChildren (
	std::vector<Zen::GameObject*>& children,
	Zen::Camera& camera)
{
	std::vector<GameObject*> visible;

	for (int i = 0; i < children.size(); i ++) {
		if (children[i]->willRender(camera))
			visible.emplace_back(children[i]);
	}

	return visible;
}

void Zen::Camera* resetAll ()
{
	cameras.clear();

	main = add();

	return main;
}

void Zen::update (Uint32 time, Uint32 delta)
{
	for (int i = 0; i < cameras.size(); i++)
		cameras[i].update(time, delta);
}

void Zen::CameraManager::onResize (Zen::Data data)
{
	int gameWidth = data.i[0];
	int gameHeight = data.i[1];
	int baseWidth = data.i[2];
	int baseHeight = data.i[3];
	int previousWidth = data.i[4];
	int previousHeight = data.i[5];

	for (int i = 0; i < cameras.size(); i++) {
		// If camera is at 0x0 and was the size of the previous game size, then
		// we can safely assume it should be updated to match the new game size too

		if (cameras[i].x == 0 && cameras[i].y == 0 && cameras[i].width == previousWidth && cameras[i].height == previousHeight)
			cameras[i].setSize(baseWidth, baseHeight);
	}
}

void Zen::CameraManager::resize (int width, int height)
{
	for (int i = 0; i < cameras.size(); i++)
		cameras[i].setSize(width, height);
}

void Zen::CameraManager::shutdown ()
{
	main = nullptr;

	cameras.clear();

	scene.events.off("SYS_UPDATE", &CameraManager::update, this);
	scene.events.off("SYS_SHUTDOWN", &CameraManager::shutdown, this);
}
