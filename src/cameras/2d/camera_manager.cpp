/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "camera_manager.h"
#include <memory>

namespace Zen {
namespace Cameras {
namespace Scene2D {

CameraManager::CameraManager (Scene* scene_)
	: systems (scene_->sys)
	, scene (scene_)
{
	scene_->sys.events.once("boot", &CameraManager::boot, this);

	scene_->sys.events.on("start", &CameraManager::start, this);
}

CameraManager::~CameraManager ()
{
	shutdown();

	systems.events.off("start", &CameraManager::start, this);
}

void CameraManager::boot ()
{
	if (systems.settings.cameras.size())
	{
		// We have cameras to create
		fromConfig(systems.settings.cameras);
	}
	else
	{
		// Make one
		add();
	}

	main = cameras[0].get();

	// Configure the default camera (It already exists)
	def.setViewport(0, 0, scene->scale.getWidth(), scene->scale.getHeight())
		.setScene(scene);

	scene->scale.on("resize", &CameraManager::onResize, this);
}

void CameraManager::start ()
{
	if (main == nullptr)
	{
		if (systems.settings.cameras.size())
		{
			// We have cameras to create
			fromConfig(systems.settings.cameras);
		}
		else
		{
			// Make one
			add();
		}

		main = cameras[0].get();
	}

	systems.events.on("update", &CameraManager::update, this);
	systems.events.on("shutdown", &CameraManager::shutdown, this);
}

Camera* CameraManager::add (
		int x_, int y_, int width_, int height_, bool makeMain_, std::string name_)
{
	if (width_ == 0)	width_ = scene->scale.getWidth();
	if (height_ == 0)	height_ = scene->scale.getHeight();

	cameras.emplace_back( std::make_unique<Camera>(x_, y_, width_, height_) );
	Camera* camera_ = cameras.back().get();

	camera_->setName(name_);
	camera_->setScene(scene);

	camera_->id = getNextID();

	if (makeMain_)
		main = camera_;

	return camera_;
}

int CameraManager::getNextID ()
{
	int testID_ = 1;

	// Find the first free camera ID we can use
	for (int t_ = 0; t_ < 32; t_++)
	{
		bool found_ = false;

		for (auto& camera_ : cameras)
		{
			if (camera_->id == testID_)
			{
				found_ = true;
				break;
			}
		}

		if (found_)
			testID_ <<= 1;
		else
			return testID_;
	}

	return 0;
}

int CameraManager::getTotal (bool isVisible_)
{
	int total_ = 0;

	for (auto& camera_ : cameras)
	{
		if (!isVisible_ || (isVisible_ && camera_->getVisible()))
			total_++;
	}

	return total_;
}

CameraManager& CameraManager::fromConfig (
		std::vector<CameraConfig> config_)
{
	int gameWidth_ = scene->scale.getWidth();
	int gameHeight_ = scene->scale.getHeight();

	for (auto& camConfig_ :  config_)
	{
		auto camera_ = add(
				camConfig_.x,
				camConfig_.y,
				camConfig_.width,
				camConfig_.height
				);

		// Direct properties
		camera_->setName(camConfig_.name)
			.setZoom(camConfig_.zoom)
			.setRotation(camConfig_.rotation)
			.setScroll(camConfig_.scrollX, camConfig_.scrollY)
			.setVisible(camConfig_.visible)
			.setBackgroundColor(camConfig_.backgroundColor);

		if (camConfig_.bounds.width)
		{
			camera_->setBounds(
					camConfig_.bounds.x,
					camConfig_.bounds.y,
					camConfig_.bounds.width,
					camConfig_.bounds.height
					);
		}
	}

	return *this;
}

Camera* CameraManager::getCamera (std::string name_)
{
	for (std::unique_ptr<Camera>& camera_ : cameras)
	{
		if (camera_->name == name_)
			return camera_.get();
	}

	return nullptr;
}

std::vector<Camera*> CameraManager::getCamerasBelowPointer (
		Input::Pointer pointer_)
{
	std::vector<Camera*> output_;
	Geom::Rectangle camRect_;

	// Inverted search
	// So the top-most camera is at the top of the search vector
	for (int i = cameras.size() - 1; i >= 0; i++)
	{
		camRect_.setTo(
			cameras[i]->getX(),
			cameras[i]->getY(),
			cameras[i]->getWidth(),
			cameras[i]->getHeight()
			);

		if (cameras[i]->getVisible() &&
			cameras[i]->inputEnabled &&
			camRect_.contains(pointer_.x, pointer_.y)
		   )
		{
			output_.emplace_back(cameras[i].get());
		}
	}

	return output_;
}

int CameraManager::remove (std::vector<Camera*> camerasToRemove_)
{
	int total_ = 0;

	for (auto it_ = cameras.begin(); it_ != cameras.end();)
	{
		for (Camera*& c_ : camerasToRemove_)
		{
			if (c_ == it_->get())
			{
				if (c_ == main) main = nullptr;
				cameras.erase(it_);
				total_++;
			}
			else
			{
				// Only increment if no erasing happened
				it_++;
			}
		}
	}

	if (main == nullptr && cameras.size())
		main = cameras[0].get();

	return total_;
}

int CameraManager::remove (Camera* cameraToRemove_)
{
	std::vector<Camera*> camerasToRemove_ {cameraToRemove_};

	return remove(camerasToRemove_);
}

void CameraManager::render (
		Renderer& renderer_,
		GameObjects::DisplayList& displayList_)
{
	for (auto& camera_ : cameras)
	{
		if (camera_->getVisible() && camera_->getAlpha() > 0)
		{
			camera_->preRender();

			auto visibleChildren_ = getVisibleChildren(displayList_.getChildren(), camera_.get());

			renderer_.render(*scene, visibleChildren_, *camera_.get());
		}
	}
}

std::vector<GameObjects::GameObject*> CameraManager::getVisibleChildren (
		std::vector<GameObjects::GameObject*> children_,
		Camera *camera_)
{
	std::vector<GameObjects::GameObject*> visible_;

	for (auto& child_ : children_)
	{
		if (child_->willRender(*camera_))
			visible_.emplace_back(child_);
	}

	return visible_;
}

Camera* CameraManager::resetAll ()
{
	cameras.clear();

	main = add();

	return main;
}

void CameraManager::update (Uint32 time_, Uint32 delta_)
{
	for (auto& camera_ : cameras)
		camera_->update(time_, delta_);
}

void CameraManager::onResize (
		Structs::Size& gameSize_,
		Structs::Size& displaySize_,
		int previousWidth_,
		int previousHeight_)
{
	for (auto& camera_ : cameras)
	{
		// If camera is at 0x0 and was the size of the previous game size, then
		// we can safely assume it should be updated to match the new game size too

		if (camera_->getX() == 0 && camera_->getY() == 0 && camera_->getWidth() == previousWidth_ && camera_->getHeight() == previousHeight_)
		{
			camera_->setSize(gameSize_.getWidth(), gameSize_.getHeight());
		}
	}
}

void CameraManager::resize (int width_, int height_)
{
	for (auto& camera_ : cameras)
		camera_->setSize(width_, height_);
}

void CameraManager::shutdown ()
{
	main = nullptr;

	cameras.clear();

	systems.events.off("update", &CameraManager::update, this);
	systems.events.off("shutdown", &CameraManager::shutdown, this);
}

}	// namespace Scene2D
}	// namespace Cameras
}	// namespace Zen
