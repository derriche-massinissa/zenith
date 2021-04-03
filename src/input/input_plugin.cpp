/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "input_plugin.hpp"

#include <algorithm>
#include <SDL2/SDL_timer.h>
#include <cmath>
#include "../utils/assert.hpp"
#include "events/events.hpp"
#include "../scene/scene.h"
#include "../components/input.hpp"
#include "../components/position.hpp"
#include "../components/container_item.hpp"
#include "../components/rotation.hpp"
#include "../components/scale.hpp"
#include "../components/origin.hpp"
#include "input_manager.hpp";
#include "pointer.hpp";

namespace Zen {

class InputManager;

extern entt::registry g_registry;
extern EventEmitter g_event;
extern InputManager g_input;

InputPlugin::InputPlugin ()
{
}

InputPlugin::~InputPlugin ()
{
}

void InputPlugin::onWindowOver ()
{
	if (isActive())
		emit("windowover",);
}

void InputPlugin::onWindowOut ()
{
	if (isActive())
		emit("windowout",);
}

void InputPlugin::preUpdate ()
{
	emit("SYS_PRE_UPDATE");

	size_t toRemove_ = pendingRemoval.size();
	size_t toInsert_ = pendingInsertion.size();

	if (toRemove_ == 0 && toInsert_ == 0)
		return;

	// Delete old entities
	for (size_t i = 0; i < toRemove_; i++)
	{
		Entity entity = pendingRemoval[i];

		for (auto it = list.begin(); it != list.end();)
		{
			if (entity == *it)
			{
				list.erase(it);
				clear(entity, true);
			}
			else
			{
				it++;
			}
		}
	}

	// Clear the removal list
	pendingRemoval.clear();

	// Move pendingInsertion to list
	list.insert(list.end(), pendingInsertion.begin(), pendingInsertion.end());
	pendingInsertion.clear();
}

bool InputPlugin::isActive ()
{
	return (enabled && scene->sys.isActive());
}

void InputPlugin::update ()
{
}

void InputPlugin::clear (Entity entity_, bool skipQueue_)
{
	auto input = g_registry.try_get<Components::Input>(entity_);

	if (!input)
		return;

	if (!skipQueue_)
		queueForRemoval(entity_);

	g_registry.remove<Components::Input>(entity_);

	// Clear from draggable, drag and over
	draggable.erase(std::remove(draggable.begin(), draggable.end(), entity_), draggable.end());

	drag.erase(std::remove(drag.begin(), drag.end(), entity_), drag.end());

	over.erase(std::remove(over.begin(), over.end(), entity_), over.end());

	manager->resetCursor(entity_);
}

void InputPlugin::disable (Entity entity_)
{
	auto input = g_registry.try_get<Components::Input>(entity_);
	ZEN_ASSERT(input, "The entity has no 'Input' component.");

	input->enabled = false;
}

void InputPlugin::enable (Entity entity_, Shape hitArea_, HitCallback hitAreaCallback_, bool dropZone_)
{
	auto input_ = g_registry.try_get<Components::Input>(entity_);
	ZEN_ASSERT(input_, "The entity has no 'Input' component.");

	setHitArea(entity_, hitArea_, hitAreaCallback_);

	input_->dropZone = dropZone_;
}

std::vector<Entity> InputPlugin::hitTestPointer (Pointer *pointer_)
{
	auto cameras_ = cameras->getCamerasBelowPointer(pointer_);

	for (auto c_ : cameras_)
	{
		// Get a list of all objects that can be seen by the camera below the
		// pointer in the scene and store in 'over' array. All objects in this
		// array are input enabled, as checked by the hitTest method, so we
		// don't need to check later on as well.
		auto over_ = g_input.hitTest(pointer_, list, c_);

		// Filter out the drop zones
		for (auto ent_ : over_)
		{
			auto input_ = g_registry.try_get<Components::Input>(ent_);
			if (input_ && input_->dropZone)
				tempZones.push_back(ent_);
		}

		if (over_.size() > 0)
		{
			pointer_->camera = c_;

			return over_;
		}
	}

	pointer_->camera = cameras_.at(0);

	return {};
}

/**
 * Returns the drag state of the given Pointer for this InputPlugin.
 *
 * The state will be one of the following:
 *
 * 0 = Not dragging anything
 * 1 = Primary button down and objects below, so collect a draglist
 * 2 = Pointer being checked if meets drag criteria
 * 3 = Pointer meets criteria, notify the draglist
 * 4 = Pointer actively dragging the draglist and has moved
 * 5 = Pointer actively dragging but has been released, notify draglist
 *
 * @since 0.0.0
 *
 * @param pointer The Pointer to get the drag state for.
 *
 * @return The drag state of the given Pointer.
 */
int InputPlugin::getDragState (Pointer *pointer_)
{
	return dragState[pointer_->id];
}


/**
 * Sets the drag state of the given Pointer for this Input Plugin.
 *
 * The state must be one of the following values:
 *
 * 0 = Not dragging anything
 * 1 = Primary button down and objects below, so collect a draglist
 * 2 = Pointer being checked if meets drag criteria
 * 3 = Pointer meets criteria, notify the draglist
 * 4 = Pointer actively dragging the draglist and has moved
 * 5 = Pointer actively dragging but has been released, notify draglist
 *
 * @since 0.0.0
 *
 * @param pointer The Pointer to set the drag state for.
 * @param state The drag state value. An integer between 0 and 5.
 */
void InputPlugin::setDragState (Pointer *pointer_, int state_)
{
	dragState[pointer_->id] = state_;
}

int InputPlugin::processDragThresholdEvent (Pointer *pointer_, Uint32 time_)
{
	bool passed_ = false;
	Uint32 timeTh_ = dragTimeThreshold;
	double distanceTh_ = dragDistanceThreshold;

	if (distanceTh_ > 0 && DistanceBetween(pointer_->position.x, pointer_->position.y, pointer_->downX, pointer_->downY) >= distanceTh_)
	{
		//  It has moved far enough to be considered a drag
		passed_ = true;
	}
	else if (timeTh_ > 0 && (time_ >= pointer_->downTime + timeTh_))
	{
		//  It has been held down long enough to be considered a drag
		passed_ = true;
	}

	if (passed_)
	{
		setDragState(pointer_, 3);

		return processDragStartList(pointer_);
	}

	return 0;
}

int InputPlugin::processDragStartList (Pointer *pointer_)
{
	//  3 = Pointer meets criteria and is freshly down, notify the draglist
	if (getDragState(pointer_) != 3)
		return 0;

	auto list_ = drag[pointer_->id];

	for (auto ent_ : list_)
	{
		auto [input_, position_] = g_registry.try_get<Components::Input, Components::Position>(ent_);
		if (!input_) continue;

		input_->dragState = 2;

		input_->dragStartX = position_->x;
		input_->dragStartY = position_->y;

		input_->dragStartXGlobal = pointer_->worldX;
		input_->dragStartYGlobal = pointer_->worldY;

		input_->dragX = input_->dragStartXGlobal - input_->dragStartX;
		input_->dragY = input_->dragStartYGlobal - input_->dragStartY;

		g_event.emit(ent_, "dragstart", pointer_, input_->dragX, input_->dragY);

		emit("dragstart", pointer_, ent_);
	}

	setDragState(pointer_, 4);

	return list_.size();
}

int InputPlugin::processDragDownEvent (Pointer *pointer_)
{
	auto& currentlyOver_ = temp;

	if (draggable.empty() || currentlyOver_.empty() || !pointer_->primaryDown || getDragState(pointer_))
		//  There are no draggable items, no over items or the pointer isn't down
		return 0;

	//  1 = Primary button down and objects below, so collect a draglist
	setDragState(pointer_, 1);

	std::vector<Entity> draglist_;

	for (auto ent_ : currentlyOver_)
	{
		auto input_ = g_registry.try_get<Components::Input>(ent_);

		if (input_ && input_->draggable && !input_->dragState)
			draglist_.push_back(ent_);
	}

	if (draglist_.empty())
	{
		setDragState(pointer_, 0);
		return 0;
	}
	else if (draglist_.size() > 1)
	{
		sortEntities(draglist_, pointer_);

		if (topOnly)
		{
			draglist_.resize(1);
		}
	}

	//  draglist now contains all potential candidates for dragging
	drag[pointer_->id] = draglist_;

	if (dragDistanceThreshold == 0 && dragTimeThreshold == 0)
	{
		//  No drag criteria, so snap immediately to mode 3
		setDragState(pointer_, 3);

		return processDragStartList(pointer_);
	}
	else
	{
		//  Check the distance / time on the next event
		setDragState(pointer_, 2);

		return 0;
	}
}

int InputPlugin::processDragMoveEvent (Pointer *pointer_)
{
	//  2 = Pointer being checked if meets drag criteria
	if (getDragState(pointer_) == 2)
	{
		processDragThresholdEvent(pointer_, SDL_GetTicks());
	}

	if (getDragState(pointer_) != 4)
		return 0;

	//  4 = Pointer actively dragging the draglist and has moved
	auto& dropZones_ = tempZones;
	auto& list_ = drag[pointer_->id];

	for (auto& obj_ : list_)
	{
		auto& input_ = g_registry.get<Components::Input>(obj_);
		Entity target_ = input_.target;

		//  If this GO has a target then let's check it
		if (target_ != entt::null)
		{
			auto it_ = std::find(dropZones_.begin(), dropZones_.end(), target_);

			//  Got a target, are we still over it?
			if (it_ == dropZones_.begin())
			{
				//  We're still over it, and it's still the top of the display list, phew ...
				g_event.emit(obj_, "dragover", pointer_, target_);
				emit("dragover", pointer_, obj_, target_);
			}
			else if (it_ != dropZones_.end())
			{
				//  Still over it but it's no longer top of the display list (targets must always be at the top)
				g_event.emit(obj_, "dragleave", pointer_, target_);

				emit("dragleave", pointer_, obj_, target_);

				input_.target = dropZones_.at(0);

				target_ = input_.target;

				g_event.emit(obj_, "dragenter", pointer_, target_);

				emit("dragenter", pointer_, obj_, target_);
			}
			else {
				//  Nope, we've moved on (or the target has!), leave the old target
				g_event.emit(obj_, "dragleave", pointer_, target_);

				emit("dragleave", pointer_, obj_, target_);

				//  Anything new to replace it?
				if (dropZones_.size())
				{
					//  Yup!
					input_.target = dropZones_.at(0);

					target_ = input_.target;

					g_event.emit(obj_, "dragenter", pointer_, target_);

					emit("dragenter", pointer_, obj_, target_);
				}
				else {
					// Nope
					input_.target = entt::null;
				}
			}
		}
		else if (dropZones_.size())
		{
			input_.target = dropZones_.at(0);

			target_ = input_.target;

			g_event.emit(obj_, "dragenter", pointer_, target_);

			emit("dragenter", pointer_, obj_, target_);
		}

		double dragX_, dragY_;

		auto item_ = g_registry.try_get<Components::ContainerItem>(obj_);
		if (!item_)
		{
			dragX_ = pointer_->worldX - input_.dragX;
			dragY_ = pointer_->worldY - input_.dragY;
		}
		else
		{
			double dx_ = pointer_->worldX - input_.dragStartXGlobal;
			double dy_ = pointer_->worldY - input_.dragStartYGlobal;

			auto [rot_, sca_] = g_registry.try_get<Components::Rotation, Components::Scale>(item_->parent);

			if (rot_ && sca_)
			{
				double dxRotated_ = dx_ * std::cos(rot_->value) + dy_ * std::sin(rot_->value);
				double dyRotated_ = dy_ * std::cos(rot_->value) - dx_ * std::sin(rot_->value);

				dxRotated_ *= (1. / sca_->x);
				dyRotated_ *= (1. / sca_->y);

				dragX_ = dxRotated_ + input_.dragStartX;
				dragY_ = dyRotated_ + input_.dragStartY;
			}
		}

		g_event.emit(obj_, "drag", pointer_, dragX_, dragY_);

		emit("drag", pointer_, obj_, dragX_, dragY_);
	}

	return list_.size();
}

int InputPlugin::processDragUpEvent (Pointer *pointer_)
{
	//  5 = Pointer was actively dragging but has been released, notify draglist
	auto& list_ = drag[pointer_->id];

	for (auto& obj_ : list_)
	{
		auto [input_, origin_] = g_registry.try_get<Components::Input, Components::Origin>(obj_);

		if (input_ && origin_ && input_->dragState == 2)
		{
			input_->dragState = 0;

			input_->dragX = input_->localX - origin_->displayX;
			input_->dragY = input_->localY - origin_->displayY;

			bool dropped_ = false;
			Entity target_ = input_->target;

			if (target_ != entt::null)
			{
				g_event.emit(obj_, "drop", pointer_, target_);

				emit("drop", pointer_, obj_, target_);

				input_->target = entt::null;

				dropped_ = true;
			}

			//  And finally the dragend event
			g_event.emit(obj_, "dragend", pointer_, input_->dragX, input_->dragY, dropped_);

			emit("dragend", pointer_, obj_, target_);
		}
	}

	setDragState(pointer_, 0);

	list_.clear();

	return 0;
}

int InputPlugin::processMoveEvents (Pointer *pointer_)
{
	int total_ = 0;
	auto& currentlyOver_ = temp;

	resetInputEvent(&tempEvent);

	bool aborted_ = false;

	//  Go through all objects the pointer was over and fire their events / callbacks
	for (auto obj_ : currentlyOver_)
	{
		auto input_ = g_registry.try_get<Components::Input>(obj_);
		if (!input_)
			continue;

		total_++;

		tempEvent.pointer = pointer_;
		tempEvent.gameObject = obj_;
		tempEvent.x = input_->localX;
		tempEvent.y = input_->localY;

		g_event.emit(obj_, ZEN_INPUT_POINTER_MOVE, &tempEvent);

		if (tempEvent.stopFlag)
		{
			aborted_ = true;
			break;
		}

		emit(ZEN_INPUT_GAMEOBJECT_MOVE, &tempEvent);

		if (tempEvent.stopFlag)
		{
			aborted_ = true;
			break;
		}

		if (topOnly)
			break;
	}

	if (!aborted_)
		emit("pointermove", &tempEvent);

	return total_;
}

int InputPlugin::processWheelEvents (Pointer *pointer_)
{
}

int InputPlugin::processOverEvents (Pointer *pointer_)
{
}

int InputPlugin::processOutEvents (Pointer *pointer_)
{
}

int InputPlugin::processOverOutEvents (Pointer *pointer_)
{
}

int InputPlugin::processUpEvents (Pointer *pointer_)
{
}

int InputPlugin::processDownEvents (Pointer *pointer_)
{
	int total = 0;
	auto& currentlyOver_ = temp;

	for (auto ent_ : currentlyOver_)
	{
		auto input_ = g_registry.try_get<Components::Input>(ent_);

		if (!input_)
			continue;

		total++;

		g_event.emit(ent_, "pointerdown", pointer_, input_.localX, input_.localY, );

		if ()
	}
}

void InputPlugin::queueForInsertion (Entity entity_)
{
}

void InputPlugin::queueForRemoval (Entity entity_)
{
}

void InputPlugin::setDraggable (Entity entity_, bool value_ = true)
{
}

HitCallback InputPlugin::makePixelPerfect (double alphaTolerance_)
{
}

void InputPlugin::setHitArea (Entity entity_, Shape hitArea_, HitCallback hitAreaCallback_)
{
}

void InputPlugin::setHitAreaCircle (Entity entity_, double x_, double y_, double radius_, HitCallback callback_ = nullptr)
{
}

void InputPlugin::setHitAreaEllipse (Entity entity_, double x_, double y_, double width_, double height_, HitCallback callback_ = nullptr)
{
}

void InputPlugin::setHitAreaFromTexture (Entity entity_, HitCallback callback_ = nullptr)
{
}

void InputPlugin::setHitAreaRectangle (Entity entity_, double x_, double y_, double width_, double height_, HitCallback callback_ = nullptr)
{
}

void InputPlugin::setHitAreaTriangle (Entity entity_, double x1_, double y1_, double x2_, double y2_, double x3_, double y3_, HitCallback callback_ = nullptr)
{
}

void InputPlugin::enableDebug (Entity entity_, Color color_)
{
}

void InputPlugin::removeDebug (Entity entity_)
{
}

void InputPlugin::setPollAlways ()
{
}

void InputPlugin::setPollOnMove ()
{
}

void InputPlugin::setPollRate ()
{
}

void InputPlugin::setGlobalTopOnly (bool value_)
{
}

void InputPlugin::setTopOnly (bool value_)
{
}

std::vector<Entity> InputPlugin::sortEntities (std::vector<Entity> entities_, Pointer pointer)
{
}

std::vector<Entity> InputPlugin::sortDropZones (std::vector<Entity> entities_)
{
}

int InputPlugin::sortDropZoneHandler (Entity childA, Entity childB)
{
}

void InputPlugin::stopPropagation ()
{
}

std::vector<Pointer> InputPlugin::addPointer (int quantity_)
{
}

void InputPlugin::setDefaultCursor (std::string textureKey_, std::string frameName_)
{
}

// TODO Move private members here pls
bool InputPlugin::transitionIn ()
{
}

bool InputPlugin::transitionComplete ()
{
}

bool InputPlugin::transitionOut ()
{
}

void InputPlugin::shutdown ()
{
}

void resetInputEvent (InputEvent* event_)
{
	event_->pointer = nullptr;

	event_->gameObject = entt::null;

	event_->gameObjectList.clear();

	event_->target = entt::null;

	event_->stopPropagation = false;

	event_->timestamp = 0;

	event_->x = 0.;
	event_->y = 0.;
	event_->z = 0.;
}

}	// namespace Zen
