/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "camera_manager.hpp"

#include <memory>
#include "systems/camera.hpp"
#include "../../scene/scene_manager.hpp"
#include "../../scene/scene.hpp"
#include "../../scale/scale_manager.hpp"
#include "../../geom/rectangle.hpp"
#include "../../renderer/renderer.hpp"
#include "../../systems/actor.hpp"
#include "../../systems/name.hpp"
#include "../../systems/id.hpp"
#include "../../systems/visible.hpp"
#include "../../systems/zoom.hpp"
#include "../../systems/rotation.hpp"
#include "../../systems/scroll.hpp"
#include "../../systems/background_color.hpp"
#include "../../systems/bounds.hpp"
#include "../../systems/position.hpp"
#include "../../systems/size.hpp"
#include "../../systems/input.hpp"
#include "../../systems/alpha.hpp"
#include "../../systems/renderable.hpp"

namespace Zen {

extern ScaleManager g_scale;
extern entt::registry g_registry;

CameraManager::CameraManager (Scene* scene_)
	: scene (scene_)
	, systems (scene_->sys)
{
	scene_->sys.events.once("boot", &CameraManager::boot, this);

	lStart = scene_->sys.events.on("start", &CameraManager::start, this);
}

CameraManager::~CameraManager ()
{
	for (Entity camera : cameras)
		g_registry.destroy(camera);

	main = entt::null;

	systems.events.off(lUpdate);
	systems.events.off(lStart);

	g_scale.off(lResize);
}

void CameraManager::boot ()
{
	// Make a main camera
	add();

	main = cameras[0];

	// Create a default camera
	def = CreateCamera(0, 0, g_scale.getWidth(), g_scale.getHeight());
	//SetScene(def, scene);
	//TODO SetDefaultCamera();

	lResize = g_scale.on("resize", &CameraManager::onResize, this);
}

void CameraManager::start ()
{
	if (main == entt::null)
	{
		// Make a main camera
		add();

		main = cameras[0];
	}

	lUpdate = systems.events.on("update", &CameraManager::update, this);
}

Entity CameraManager::add (
		int x_, int y_, int width_, int height_, bool makeMain_, std::string name_)
{
	if (width_ == 0)
		width_ = g_scale.getWidth();

	if (height_ == 0)
		height_ = g_scale.getHeight();

	auto camera_ = CreateCamera(x_, y_, width_, height_);
	cameras.emplace_back(camera_);

	SetName(camera_, name_.c_str());
	SetScene(camera_, scene);

	SetId(camera_, getNextID());

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
			if (GetId(camera_) == testID_)
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
		if (!isVisible_ || (isVisible_ && GetVisible(camera_)))
			total_++;
	}

	return total_;
}

CameraManager& CameraManager::fromConfig (
		std::vector<CameraConfig> config_)
{
	for (auto& camConfig_ :  config_)
	{
		auto camera_ = add(
				camConfig_.x,
				camConfig_.y,
				camConfig_.width,
				camConfig_.height
				);

		// Direct properties
		SetName(camera_, camConfig_.name.c_str());
		SetZoom(camera_, camConfig_.zoom);
		SetRotation(camera_, camConfig_.rotation);
		SetScroll(camera_, camConfig_.scrollX, camConfig_.scrollY);
		SetVisible(camera_, camConfig_.visible);
		SetBackgroundColor(camera_, camConfig_.backgroundColor);;

		if (camConfig_.bounds.width)
		{
			SetBounds(
					camera_,
					camConfig_.bounds.x,
					camConfig_.bounds.y,
					camConfig_.bounds.width,
					camConfig_.bounds.height
					);
		}
	}

	return *this;
}

Entity CameraManager::getCamera (std::string name_)
{
	for (auto camera_ : cameras)
	{
		if (GetName(camera_) == name_)
			return camera_;
	}

	return entt::null;
}

std::vector<Entity> CameraManager::getCamerasBelowPointer (Pointer *pointer_)
{
	std::vector<Entity> output_;
	Rectangle camRect_;

	// Inverted search
	// So the top-most camera is at the top of the search vector
	for (auto c_ = cameras.rbegin(); c_ != cameras.rend(); c_++)
	{
		SetTo(
			&camRect_,
			GetX(*c_),
			GetY(*c_),
			GetWidth(*c_),
			GetHeight(*c_)
			);

		if (GetVisible(*c_) &&
			IsInputEnabled(*c_) &&
			Contains(camRect_, pointer_->position.x, pointer_->position.y)
		   )
		{
			output_.emplace_back(*c_);
		}
	}

	return output_;
}

int CameraManager::remove (std::vector<Entity> camerasToRemove_)
{
	int total_ = 0;

	for (auto it_ = cameras.begin(); it_ != cameras.end();)
	{
		for (Entity c_ : camerasToRemove_)
		{
			if (c_ == *it_)
			{
				if (c_ == main) main = entt::null;
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

	if (main == entt::null && cameras.size())
		main = cameras[0];

	return total_;
}

int CameraManager::remove (Entity cameraToRemove_)
{
	std::vector<Entity> camerasToRemove_ {cameraToRemove_};

	return remove(camerasToRemove_);
}

void CameraManager::render (
		Renderer& renderer_,
		DisplayList& displayList_)
{
	for (auto camera_ : cameras)
	{
		if (GetVisible(camera_) && GetAlpha(camera_) > 0)
		{
			PreRender(camera_);

			auto visibleChildren_ = getVisibleChildren(displayList_.getChildren(), camera_);

			renderer_.render(visibleChildren_, camera_);
		}
	}
}

std::vector<Entity> CameraManager::getVisibleChildren (
		std::vector<Entity> children_,
		Entity camera_)
{
	std::vector<Entity> visible_;

	for (auto& child_ : children_)
	{
		if (WillRender(child_, camera_))
			visible_.emplace_back(child_);
	}

	return visible_;
}

Entity CameraManager::resetAll ()
{
	cameras.clear();

	main = add();

	return main;
}

void CameraManager::update (Uint32 time_, Uint32 delta_)
{
	for (auto& camera_ : cameras)
		UpdateCamera(camera_, time_, delta_);
}

void CameraManager::onResize (
		Size gameSize_,
		[[maybe_unused]] Size displaySize_,
		int previousWidth_,
		int previousHeight_)
{
	for (auto& camera_ : cameras)
	{
		// If camera is at 0x0 and was the size of the previous game size, then
		// we can safely assume it should be updated to match the new game size too

		if (GetX(camera_) == 0 && GetY(camera_) == 0 && GetWidth(camera_) == previousWidth_ && GetHeight(camera_) == previousHeight_)
		{
			SetSize(camera_, gameSize_.width, gameSize_.height);
		}
	}
}

void CameraManager::resize (int width_, int height_)
{
	for (auto& camera_ : cameras)
		SetSize(camera_, width_, height_);
}

}	// namespace Zen
