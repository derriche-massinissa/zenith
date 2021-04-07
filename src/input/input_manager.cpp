/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "input_manager.hpp"

#include "events/events.hpp"
#include "../utils/assert.hpp"
#include "../scene/scene_manager.h"
#include "../scene/scene.h"
#include "../scale/scale_manager.hpp"
#include "../window/window.hpp"
#include <SDL2/SDL_timer.h>
#include "../core/config.hpp"
#include "../components/container_item.hpp"
#include "../components/scroll.hpp"
#include "../components/scroll_factor.hpp"
#include "../components/position.hpp"
#include "../components/rotation.hpp"
#include "../components/scale.hpp"
#include "../components/input.hpp"
#include "../systems/renderable.hpp"
#include "../cameras/2d/systems/camera.hpp"
#include "../systems/transform.hpp"
#include "../systems/transform_matrix.hpp"
#include "../systems/origin.hpp"
#include "../math/transform_xy.hpp"

namespace Zen {

extern entt::registry g_registry;
extern Scenes::SceneManager g_scene;
extern ScaleManager g_scale;
extern Window g_window;

InputManager::InputManager ()
{}

InputManager::~InputManager ()
{
	g_window.off("pointerin", &InputManager::setWindowOver, this);
	g_window.off("pointerout", &InputManager::setWindowOut, this);
}

void InputManager::boot (GameConfig* cfg_)
{
	config = cfg_;

	mousePointer = addPointer(1)[0];

	g_window.on("pointerin", &InputManager::setWindowOver, this);
	g_window.on("pointerout", &InputManager::setWindowOut, this);

	booted = true;
}

void InputManager::setWindowOver ()
{
	if (!booted) return;

	isOver = true;

	events.emit(ZEN_INPUT_WINDOW_OVER);
}

void InputManager::setWindowOut ()
{
	if (!booted) return;

	isOver = false;

	events.emit(ZEN_INPUT_WINDOW_OUT);
}

void InputManager::preRender (Uint32 time_, Uint32 delta_)
{
	if (!booted) return;

	auto scenes_ = g_scene.getScenes(true, true);

	events.emit("SYS_UPDATE");

	for (auto& scene_ : scenes_)
	{
		if (scene_->input.updatePoll(time_, delta_) && globalTopOnly)
		{
			// If the Scene returns true, it means it captured some input that no other Scene should get, so we bail out
			return;
		}
	}
}

void InputManager::setDefaultCursor ()
{
	if (!booted) return;

	// TODO custom cursor
}

void InputManager::setCursor ()
{
	if (!booted) return;

	// TODO custom cursor
}

void InputManager::resetCursor ()
{
	if (!booted) return;

	// TODO custom cursor
}

std::vector<Pointer*> InputManager::addPointer (int quantity_)
{
	std::vector<Pointer*> output_;

	if (pointers.size() + quantity_ > 10)
		quantity_ = 10 - pointers.size();

	for (int i_ = 0; i_ < quantity_; i_++)
	{
		int id_ = pointers.size();

		pointers.emplace_back();

		auto& pointer_ = pointers.back();

		pointer_.smoothFactor = config->inputSmoothFactor;
		pointer_.id = id_;

		output_.push_back(&pointer_);
	}

	return output_;
}

void InputManager::updateInputPlugins (INPUT type_, std::vector<Pointer*> pointers_)
{
	if (!booted) return;

	auto scenes_ = g_scene.getScenes(true, true);

	tempSkip = false;

	for (auto& scene_ : scenes_)
	{
		bool capture_ = scene_->input.update(type_, pointers_);

		if ((capture_ && globalTopOnly) || tempSkip)
		{
			// If the Scene returns true, or called stopPropagation, it means it captured some input that no other Scene should get, so we bail out
			return;
		}
	}
}
void InputManager::onTouchStart (InputEvent event_)
{}

void InputManager::onTouchMove (InputEvent event_)
{}

void InputManager::onTouchEnd (InputEvent event_)
{}

void InputManager::onTouchCancel (InputEvent event_)
{}

void InputManager::onMouseDown (InputEvent event_)
{
	if (!booted) return;

	mousePointer->down(event_);

	mousePointer->updateMotion();

	activePointer = mousePointer;

	updateInputPlugins(INPUT::MOUSE_DOWN, {mousePointer});
}

void InputManager::onMouseMove (InputEvent event_)
{
	if (!booted) return;

	mousePointer->move(event_);

	mousePointer->updateMotion();

	activePointer = mousePointer;

	updateInputPlugins(INPUT::MOUSE_MOVE, {mousePointer});
}

void InputManager::onMouseUp (InputEvent event_)
{
	if (!booted) return;

	mousePointer->up(event_);

	mousePointer->updateMotion();

	activePointer = mousePointer;

	updateInputPlugins(INPUT::MOUSE_UP, {mousePointer});
}

void InputManager::onMouseWheel (InputEvent event_)
{
	if (!booted) return;

	mousePointer->wheel(event_);

	mousePointer->updateMotion();

	activePointer = mousePointer;

	updateInputPlugins(INPUT::MOUSE_WHEEL, {mousePointer});
}

void InputManager::onPointerLockChange (bool locked_)
{
	mousePointer->locked = locked_;
}

bool InputManager::inputCandidate (Entity entity_, Entity camera_)
{
	auto [input_, item_] = g_registry.try_get<Components::Input, Components::ContainerItem>(entity_);

	if (!input_ || !input_->enabled || (!input_->alwaysEnabled && !WillRender(entity_, camera_)))
	{
		return false;
	}

	bool visible_ = true;

	if (item_)
	{
		do
		{
			if (!WillRender(item_->parent, camera_))
			{
				visible_ = false;
				break;
			}

			item_ = g_registry.try_get<Components::ContainerItem>(item_->parent);

		} while (item_);
	}

	return visible_;
}

std::vector<Entity> InputManager::hitTest (Pointer* pointer_, std::vector<Entity> gameObjects_, Entity camera_)
{
	tempHitTest.clear();
	auto& output_ = tempHitTest;

	auto scroll_ = g_registry.try_get<Components::Scroll>(camera_);
	ZEN_ASSERT(scroll_, "The entity has no 'Scroll' component.");

	double csx_ = scroll_->x;
	double csy_ = scroll_->y;

	double x_ = pointer_->position.x;
	double y_ = pointer_->position.y;

	// Stores the world point inside of tempPoint
	tempPoint = GetWorldPoint(camera_, x_, y_);

	pointer_->worldX = tempPoint.x;
	pointer_->worldY = tempPoint.y;

	Math::Vector2 point_;

	auto& matrix_ = tempMatrix;
	auto& parentMatrix_ = tempMatrix2;

	for (auto obj_ : gameObjects_)
	{
		if (!inputCandidate(obj_, camera_))
			continue;

		auto [scrollFactor_, item_, position_, rotation_, scale_] = g_registry.try_get<Components::ScrollFactor,
		Components::ContainerItem,
		Components::Position,
		Components::Rotation,
		Components::Scale
		>(obj_);

		double px_ = tempPoint.x + (csx_ * scrollFactor_->x) - csx_;
		double py_ = tempPoint.y + (csy_ * scrollFactor_->y) - csy_;

		if (item_ != nullptr)
		{
			matrix_ = GetWorldTransformMatrix(obj_);
			point_ = ApplyInverse(matrix_, px_, py_);
		}
		else
		{
			point_ = Math::TransformXY(px_, py_, position_->x, position_->y, rotation_->value, scale_->x, scale_->y);
		}

		if (pointWithinHitArea(obj_, point_.x, point_.y))
		{
			output_.push_back(obj_);
		}
	}

	return output_;
}

bool InputManager::pointWithinHitArea (Entity gameObject_, double x_, double y_)
{
	// Normalize the origin
	x_ += GetDisplayOriginX(gameObject_);
	y_ += GetDisplayOriginY(gameObject_);

	auto input_ = g_registry.try_get<Components::Input>(gameObject_);

	if (input_ && input_->hitAreaCallback(input_->hitArea, x_, y_))
	{
		input_->localX = x_;
		input_->localY = y_;

		return true;
	}
	else
	{
		return false;
	}
}

void InputManager::transformPointer (Pointer* pointer_, double windowX_, double windowY_, bool wasMove_)
{
	auto& p0_ = pointer_->position;
	auto& p1_ = pointer_->prevPosition;

	// Store previous position
	p1_.x = p0_.x;
	p1_.y = p0_.y;

	// Translate coordinates
	double x_ = g_scale.transformX(windowX_);
	double y_ = g_scale.transformY(windowY_);

	double a_ = pointer_->smoothFactor;

	if (!wasMove_ || a_ == 0.)
	{
		// Set immediately
		p0_.x = x_;
		p0_.y = y_;
	}
	else
	{
		// Apply smoothing
		p0_.x = x_ * a_ + p1_.x * (1. - a_);
		p0_.y = y_ * a_ + p1_.x * (1. - a_);
	}
}

}	// namespace Zen
