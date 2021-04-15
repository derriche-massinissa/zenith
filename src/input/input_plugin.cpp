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
#include "../utils/vector/contains.hpp"
#include "../utils/vector/index_of.hpp"
#include "../utils/vector/remove.hpp"
#include "events/events.hpp"
#include "../scene/scene.h"
#include "../components/input.hpp"
#include "../components/position.hpp"
#include "../components/container_item.hpp"
#include "../components/rotation.hpp"
#include "../components/scale.hpp"
#include "../components/origin.hpp"
#include "../components/container.hpp"
#include "../components/size.hpp"
#include "../systems/textured.hpp"
#include "../systems/input.hpp"
#include "../texture/components/frame.hpp"
#include "../texture/texture_manager.hpp"
#include "input_manager.hpp"
#include "pointer.hpp"
#include "types/input_configuration.hpp"
#include "../geom/rectangle.hpp"
#include "../geom/circle.hpp"
#include "../geom/ellipse.hpp"
#include "../geom/triangle.hpp"
#include "../geom/types/circle.hpp"
#include "../geom/types/ellipse.hpp"
#include "../geom/types/triangle.hpp"
#include "../geom/types/rectangle.hpp"
#include "../geom/types/circle.hpp"
#include "../geom/types/triangle.hpp"
#include "../cameras/2d/systems/camera.hpp"
#include "../math/distance/distance_between.hpp"

namespace Zen {

class InputManager;

extern entt::registry g_registry;
extern InputManager g_input;
extern TextureManager g_texture;

InputPlugin::InputPlugin (Scene* scene_)
	: keyboard (scene_)
	, scene (scene_)
	, sys (&scene_->sys)
	, settings (&scene_->sys.settings)
	, displayList (&scene_->children)
	, cameras (&scene_->cameras)
{
	sys->events.on("start", &InputPlugin::start, this);

	for (int i = 0; i < 10; i++)
	{
		over.emplace_back();
		drag.emplace_back();
	}
}

void InputPlugin::start ()
{
	sys->events.on("transition-start", &InputPlugin::transitionIn, this);
	sys->events.on("transition-out", &InputPlugin::transitionOut, this);
	sys->events.on("transition-complete", &InputPlugin::transitionComplete, this);

	sys->events.on("pre-update", &InputPlugin::preUpdate, this);
	sys->events.on("shutdown", &InputPlugin::shutdown, this);

	enabled = true;

	for (int i = 0; i < 10; i++)
		dragState.push_back(0);

	pluginEvents.emit("start");
}

void InputPlugin::onWindowOver (InputEvent event_)
{
	if (isActive())
		emit("windowover", event_.timestamp);
}

void InputPlugin::onWindowOut (InputEvent event_)
{
	if (isActive())
		emit("windowout",event_.timestamp);
}

void InputPlugin::preUpdate ([[maybe_unused]] Uint32 time_, [[maybe_unused]] Uint32 delta_)
{
	pluginEvents.emit("pre-update");

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

bool InputPlugin::updatePoll (Uint32 time_, Uint32 delta_)
{
	if (!isActive())
		return false;

	//  The plugins should update every frame, regardless if there has been
	//  any DOM input events or not (such as the Gamepad and Keyboard)
	pluginEvents.emit("update", time_, delta_);

	//  We can leave now if we've already updated once this frame via the immediate DOM event handlers
	if (updatedThisFrame)
	{
		updatedThisFrame = false;

		return false;
	}

	for (auto& pointer_ : g_input.pointers)
	{
		pointer_.updateMotion();
	}

	//  No point going any further if there aren't any interactive objects
	if (list.empty())
	{
		return false;
	}

	if (pollRate == -1)
	{
		return false;
	}
	else if (pollRate > 0)
	{
		pollTimer -= delta_;

		if (pollTimer < 0)
		{
			//  Discard timer diff, we're ready to poll again
			pollTimer = pollRate;
		}
		else
		{
			//  Not enough time has elapsed since the last poll, so abort now
			return false;
		}
	}

	//  We got this far? Then we should poll for movement
	bool captured_ = false;

	for (auto& pointer_ : g_input.pointers)
	{
		int total_ = 0;

		// Always reset this array
		tempZones.clear();

		// _temp contains a hit test and camera culled list of IO objects
		temp = hitTestPointer(&pointer_);

		sortGameObjects(&temp, &pointer_);
		sortDropZones(&tempZones);

		if (topOnly)
		{
			// Only the top-most one counts now, so safely ignore the rest
			if (!temp.empty())
				temp.erase(temp.begin() + 1, temp.end());

			if (!tempZones.empty())
				tempZones.erase(tempZones.begin() + 1, tempZones.end());
		}

		total_ += processOverOutEvents(&pointer_);

		if (getDragState(&pointer_) == 2)
			processDragThresholdEvent(&pointer_, time_);

		if (total_ > 0)
		{
		//  We interacted with an event in this Scene, so block any Scenes below us from doing the same this frame
			captured_ = true;
		}
	}

	return captured_;
}

bool InputPlugin::update (INPUT type_, std::vector<Pointer*> pointers_)
{
	if (!isActive())
		return false;

	bool captured_ = false;

	for (auto pointer_ : pointers_)
	{
		int total_ = 0;

		// Always reset this array
		tempZones.clear();

		// _temp contains a hit test and camera culled list of IO objects
		temp = hitTestPointer(pointer_);

		sortGameObjects(&temp, pointer_);
		sortDropZones(&tempZones);

		if (topOnly)
		{
			// Only the top-most one counts now, so safely ignore the rest
			if (!temp.empty())
				temp.erase(temp.begin() + 1, temp.end());

			if (!tempZones.empty())
				tempZones.erase(tempZones.begin() + 1, tempZones.end());
		}

		switch (type_)
		{
			case INPUT::MOUSE_DOWN:
				total_ += processDragDownEvent(pointer_);
				total_ += processDownEvents(pointer_);
				total_ += processOverOutEvents(pointer_);
				break;

			case INPUT::MOUSE_UP:
				total_ += processDragUpEvent(pointer_);
				total_ += processUpEvents(pointer_);
				total_ += processOverOutEvents(pointer_);
				break;

			case INPUT::TOUCH_START:
				total_ += processDragDownEvent(pointer_);
				total_ += processDownEvents(pointer_);
				total_ += processOverEvents(pointer_);
				break;

			case INPUT::TOUCH_END:
			case INPUT::TOUCH_CANCEL:
				total_ += processDragUpEvent(pointer_);
				total_ += processUpEvents(pointer_);
				total_ += processOutEvents(pointer_);
				break;

			case INPUT::MOUSE_MOVE:
			case INPUT::TOUCH_MOVE:
				total_ += processDragMoveEvent(pointer_);
				total_ += processMoveEvents(pointer_);
				total_ += processOverOutEvents(pointer_);
				break;

			case INPUT::MOUSE_WHEEL:
				total_ += processWheelEvent(pointer_);
				break;
		}

		if (total_ > 0)
		{
		//  We interacted with an event in this Scene, so block any Scenes below us from doing the same this frame
			captured_ = true;
		}
	}

	updatedThisFrame = true;

	return captured_;
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
	Remove(draggable, entity_);

	Remove(drag.at(0), entity_);

	Remove(over.at(0), entity_);

	g_input.resetCursor();
}

void InputPlugin::disable (Entity entity_)
{
	auto input_ = g_registry.try_get<Components::Input>(entity_);
	ZEN_ASSERT(input_, "The entity has no 'Input' component.");

	input_->enabled = false;
}

void InputPlugin::enable (Entity entity_)
{
	auto input_ = g_registry.try_get<Components::Input>(entity_);

	if (input_)
	{
		input_->enabled = true;
	}
	else
	{
		setHitArea(entity_);
	}
}

void InputPlugin::enable (Entity entity_, InputConfiguration config_)
{
	auto input_ = g_registry.try_get<Components::Input>(entity_);

	if (input_)
	{
		input_->enabled = true;
	}
	else
	{
		setHitArea(entity_, config_);
	}
}

void InputPlugin::enable (Entity entity_, Shape hitArea_, HitCallback hitAreaCallback_, bool dropZone_)
{
	auto input_ = g_registry.try_get<Components::Input>(entity_);

	if (input_)
	{
		input_->enabled = true;
	}
	else
	{
		setHitArea(entity_, hitArea_, hitAreaCallback_);
	}

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

	if (!cameras_.empty())
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

	if (distanceTh_ > 0 && Math::DistanceBetween(pointer_->position.x, pointer_->position.y, pointer_->downX, pointer_->downY) >= distanceTh_)
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

	std::vector<Entity>& list_ = drag[pointer_->id];

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

		emit(ZEN_INPUT_DRAG_START, pointer_, ent_);
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

		if (input_ && input_->draggable && input_->dragState == 0)
			draglist_.push_back(ent_);
	}

	if (draglist_.empty())
	{
		setDragState(pointer_, 0);
		return 0;
	}
	else if (draglist_.size() > 1)
	{
		sortGameObjects(&draglist_, pointer_);

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

		// If this GO has a target then let's check it
		if (target_ != entt::null)
		{
			int index_ = IndexOf(dropZones_, target_);

			// Got a target, are we still over it?
			if (index_ == 0)
			{
				// We're still over it, and it's still the top of the display list
				emit(ZEN_INPUT_DRAG_OVER, pointer_, obj_, target_);
			}
			else if (index_ > 0)
			{
				// Still over it but it's no longer top of the display list
				// (targets must always be at the top)
				emit(ZEN_INPUT_DRAG_LEAVE, pointer_, obj_, target_);

				input_.target = dropZones_.at(0);

				target_ = input_.target;

				emit(ZEN_INPUT_DRAG_ENTER, pointer_, obj_, target_);
			}
			else
			{
				//  Nope, we've moved on (or the target has!), leave the old target
				emit(ZEN_INPUT_DRAG_LEAVE, pointer_, obj_, target_);

				//  Anything new to replace it?
				if (dropZones_.size())
				{
					//  Yup!
					input_.target = dropZones_.at(0);

					target_ = input_.target;

					emit(ZEN_INPUT_DRAG_ENTER, pointer_, obj_, target_);
				}
				else
				{
					// Nope
					input_.target = entt::null;
				}
			}
		}
		else if (dropZones_.size())
		{
			input_.target = dropZones_.at(0);

			target_ = input_.target;

			emit(ZEN_INPUT_DRAG_ENTER, pointer_, obj_, target_);
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

		emit(ZEN_INPUT_DRAG, pointer_, obj_, dragX_, dragY_);
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
				emit(ZEN_INPUT_DROP, pointer_, obj_, target_);

				input_->target = entt::null;

				dropped_ = true;
			}

			//  And finally the dragend event
			emit(ZEN_INPUT_DRAG_END, pointer_, obj_, dropped_);
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

	tempEvent.reset();

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
		emit(ZEN_INPUT_POINTER_MOVE, &tempEvent);

	return total_;
}

int InputPlugin::processWheelEvent (Pointer *pointer_)
{
	int total_ = 0;
	auto& currentlyOver_ = temp;

	tempEvent.reset();

	bool aborted_ = false;

	double dx_ = pointer_->deltaX;
	double dy_ = pointer_->deltaY;
	double dz_ = pointer_->deltaZ;

	//  Go through all objects the pointer was over and fire their events / callbacks
	for (auto obj_ : currentlyOver_)
	{
		auto input_ = g_registry.try_get<Components::Input>(obj_);
		if (!input_)
			continue;

		total_++;

		tempEvent.pointer = pointer_;
		tempEvent.gameObject = obj_;
		tempEvent.x = dx_;
		tempEvent.y = dy_;
		tempEvent.z = dz_;

		emit(ZEN_INPUT_GAMEOBJECT_WHEEL, &tempEvent);

		if (tempEvent.stopFlag)
		{
			aborted_ = true;
			break;
		}
	}

	//tempEvent.gameObjectList = currentlyOver_;

	if (!aborted_)
		emit(ZEN_INPUT_POINTER_WHEEL, &tempEvent);

	return total_;
}

int InputPlugin::processOverEvents (Pointer *pointer_)
{
	auto& currentlyOver_ = temp;

	int totalInteracted_ = 0;

	int total_ = currentlyOver_.size();

	std::vector<Entity> justOver_;

	if (total_ > 0)
	{
		tempEvent.reset();

		bool aborted_ = false;

		//  Go through all objects the pointer was over and fire their events / callbacks
		for (auto obj_ : currentlyOver_)
		{
			auto input_ = g_registry.try_get<Components::Input>(obj_);

			if (!input_)
				continue;

			justOver_.push_back(obj_);

			// TODO check this when done with the input manager
			//g_input.setCursor(input_);

			totalInteracted_++;

			tempEvent.pointer = pointer_;
			tempEvent.gameObject = obj_;
			tempEvent.x = input_->localX;
			tempEvent.y = input_->localY;

			emit(ZEN_INPUT_GAMEOBJECT_OVER, &tempEvent);

			if (tempEvent.stopFlag)
			{
				aborted_ = true;
				break;
			}
		}

		//tempEvent.gameObjectList = justOver_;

		if (!aborted_)
			emit(ZEN_INPUT_POINTER_WHEEL, &tempEvent);
	}

	// Then sort it into display list order
	over[pointer_->id] = justOver_;

	return totalInteracted_;
}

int InputPlugin::processOutEvents (Pointer *pointer_)
{
	auto& previouslyOver_ = over[pointer_->id];

	int totalInteracted_ = 0;

	int total_ = previouslyOver_.size();

	if (total_ > 0)
	{
		tempEvent.reset();

		bool aborted_ = false;

		sortGameObjects(&previouslyOver_, pointer_);

		//  Go through all objects the pointer was over and fire their events / callbacks
		for (auto obj_ : previouslyOver_)
		{
			// Call onOut for everything in the previouslyOver_ vector
			auto input_ = g_registry.try_get<Components::Input>(obj_);

			if (!input_)
				continue;

			// TODO check this when done with the input manager
			//g_input.resetCursor(input_);

			totalInteracted_++;

			tempEvent.pointer = pointer_;
			tempEvent.gameObject = obj_;

			emit(ZEN_INPUT_GAMEOBJECT_OUT, &tempEvent);

			if (tempEvent.stopFlag)
			{
				aborted_ = true;
				break;
			}
		}

		//tempEvent.gameObjectList = previouslyOver_;

		if (!aborted_)
			emit(ZEN_INPUT_POINTER_OUT, &tempEvent);
	}

	// Then sort it into display list order
	over[pointer_->id].clear();

	return totalInteracted_;
}

int InputPlugin::processOverOutEvents (Pointer *pointer_)
{
	auto& currentlyOver_ = temp;

	std::vector<Entity> justOut_;
	std::vector<Entity> justOver_;
	std::vector<Entity> stillOver_;
	auto& previouslyOver_ = over[pointer_->id];
	auto& currentlyDragging_ = drag[pointer_->id];

	// Go through all objects the pointer was previously over, and see if it still is.
	// Splits the previouslyOver array into two parts: justOut and stillOver
	for (auto obj_ : previouslyOver_)
	{
		if (!Contains(currentlyOver_, obj_) && !Contains(currentlyDragging_, obj_))
		{
			// Not in the currentlyOver array, so must be outside of this object now
			justOut_.push_back(obj_);
		}
		else
		{
			// In the currentlyOver_ vector
			stillOver_.push_back(obj_);
		}
	}

	// Go through all objects the pointer is currently over (the hit test results)
	// and if not in the previouslyOver array we know it's a new entry, so add to justOver
	for (auto obj_ : currentlyOver_)
	{
		// Is this newly over?
		if (!Contains(previouslyOver_, obj_))
		{
			justOver_.push_back(obj_);
		}
	}

	// By this point the vectors are filled, so now we can process what happened...

	// Process the Just Out objects
	int total_ = justOut_.size();

	int totalInteracted_ = 0;

	tempEvent.reset();

	bool aborted_ = false;

	if (total_ > 0)
	{
		sortGameObjects(&justOut_, pointer_);

		//  Call onOut for everything in the justOut_ vector
		for (auto obj_ : justOut_)
		{
			auto input_ = g_registry.try_get<Components::Input>(obj_);

			if (!input_)
				continue;

			// Reset cursor before we emit the event, in case they want to change it during the event
			// TODO custom cursor
			//g_input.resetCursor(input_);

			tempEvent.pointer = pointer_;
			tempEvent.gameObject = obj_;

			totalInteracted_++;

			emit(ZEN_INPUT_GAMEOBJECT_OUT, &tempEvent);

			if (tempEvent.stopFlag)
			{
				aborted_ = true;
				break;
			}
		}

		//tempEvent.gameObjectList = justOut_;

		if (!aborted_)
			emit(ZEN_INPUT_POINTER_OUT, &tempEvent);
	}

	// Process the Just Over objects
	total_ = justOver_.size();

	tempEvent.reset();

	aborted_ = false;

	if (total_ > 0)
	{
		sortGameObjects(&justOver_, pointer_);

		//  Call onOver for everything in the justOver_ vector
		for (auto obj_ : justOver_)
		{
			auto input_ = g_registry.try_get<Components::Input>(obj_);

			if (!input_)
				continue;

			// Set cursor before we emit the event, in case they want to change it during the event
			// TODO check this when done with the input manager
			//g_input.setCursor(input_);

			totalInteracted_++;

			tempEvent.pointer = pointer_;
			tempEvent.gameObject = obj_;
			tempEvent.x = input_->localX;
			tempEvent.x = input_->localY;

			emit(ZEN_INPUT_GAMEOBJECT_OVER, &tempEvent);

			if (tempEvent.stopFlag)
			{
				aborted_ = true;
				break;
			}
		}

		//tempEvent.gameObjectList = justOver_;
		tempEvent.pointer = pointer_;

		if (!aborted_)
			emit(ZEN_INPUT_POINTER_OVER, &tempEvent);
	}

	// Add the contents of justOver_ to the previously over array
	stillOver_.insert(stillOver_.end(), justOver_.begin(), justOver_.end());

	// Then sort it into display list order
	sortGameObjects(&stillOver_, pointer_);
	over[pointer_->id] = stillOver_;

	return totalInteracted_;
}

int InputPlugin::processUpEvents (Pointer *pointer_)
{
	auto& currentlyOver_ = temp;

	tempEvent.reset();

	bool aborted_ = false;

	//  Go through all objects the pointer was over and fire their events / callbacks
	for (auto obj_ : currentlyOver_)
	{
		auto input_ = g_registry.try_get<Components::Input>(obj_);
		if (!input_)
			continue;

		tempEvent.pointer = pointer_;
		tempEvent.x = input_->localX;
		tempEvent.y = input_->localY;
		tempEvent.gameObject = obj_;

		emit(ZEN_INPUT_GAMEOBJECT_UP, &tempEvent);

		if (tempEvent.stopFlag)
		{
			aborted_ = true;
			break;
		}
	}

	//tempEvent.gameObjectList = currentlyOver_;
	tempEvent.pointer = pointer_;

	if (!aborted_)
		emit(ZEN_INPUT_POINTER_UP, &tempEvent);

	return currentlyOver_.size();
}

int InputPlugin::processDownEvents (Pointer *pointer_)
{
	int total_ = 0;
	auto& currentlyOver_ = temp;

	tempEvent.reset();

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

		emit(ZEN_INPUT_GAMEOBJECT_DOWN, &tempEvent);

		if (tempEvent.stopFlag)
		{
			aborted_ = true;
			break;
		}
	}

	//tempEvent.gameObjectList = currentlyOver_;
	tempEvent.pointer = pointer_;

	if (!aborted_)
		emit(ZEN_INPUT_POINTER_DOWN, &tempEvent);

	return total_;
}

void InputPlugin::queueForInsertion (Entity entity_)
{
	if (!Contains(pendingInsertion, entity_) && !Contains(list, entity_))
	{
		pendingInsertion.push_back(entity_);
	}
}

void InputPlugin::queueForRemoval (Entity entity_)
{
	pendingRemoval.push_back(entity_);
}

void InputPlugin::setDraggable (Entity entity_, bool value_)
{
	auto input_ = g_registry.try_get<Components::Input>(entity_);
	ZEN_ASSERT(input_, "The entity has no 'Input' component.");

	input_->draggable = value_;

	bool contained_ = Contains(draggable, entity_);

	if (value_ && !contained_)
	{
		draggable.push_back(entity_);
	}
	else if (!value_ && contained_)
	{
		Remove(draggable, entity_);
	}
}

void InputPlugin::setDraggable (std::vector<Entity> entities_, bool value_)
{
	for (auto entity_ : entities_)
	{
		setDraggable(entity_, value_);
	}
}

HitCallback InputPlugin::makePixelPerfect (int alphaTolerance_)
{
	return std::function<bool(Shape, double, double, Entity)>
	(
		[alphaTolerance_] (Shape shape, double x, double y, Entity entity)
		{
			int alpha = g_texture.getPixelAlpha(x, y, GetFrame(entity));

			return (alpha && alpha >= alphaTolerance_);
		}
	);
}

void InputPlugin::setHitArea (std::vector<Entity> entities_, InputConfiguration config_)
{
	bool customHitArea_ = true;

	if (config_.pixelPerfect)
	{
		config_.hitArea.shape.type = SHAPE::NONE;
		config_.hitAreaCallback = makePixelPerfect(config_.alphaTolerance);
	}

	// Still no callback?
	if (config_.hitAreaCallback == nullptr)
	{
		setHitAreaFromTexture(entities_);
		customHitArea_ = false;
	}

	for (auto ent_ : entities_)
	{
		auto input_ = g_registry.try_get<Components::Input>(ent_);

		auto container_ = g_registry.has<Components::Container>(ent_);

		if (config_.pixelPerfect && container_)
		{
			MessageWarning("Cannot pixelPerfect test a Container. Use a custom callback.");
			continue;
		}

		if (!input_)
		{
			input_ = &g_registry.emplace<Components::Input>(ent_);
			input_->hitArea = config_.hitArea;
			input_->hitAreaCallback = config_.hitAreaCallback;
		}

		input_->customHitArea = customHitArea_;
		input_->dropZone = config_.dropZone;
		// TODO input_->cursor = (config_.useHandCursor) ? "pointer" : config_.cursor;

		if (config_.draggable)
			setDraggable(ent_);

		queueForInsertion(ent_);
	}
}

void InputPlugin::setHitArea (Entity entity_, InputConfiguration config_)
{
	setHitArea(std::vector<Entity> {entity_}, config_);
}

void InputPlugin::setHitArea (Entity entity_)
{
	setHitAreaFromTexture(entity_);
}

void InputPlugin::setHitArea (std::vector<Entity> entities_)
{
	for (auto ent_ : entities_)
		setHitAreaFromTexture(ent_);
}

void InputPlugin::setHitArea (std::vector<Entity> entities_, Shape hitArea_, HitCallback hitAreaCallback_)
{
	for (auto ent_ : entities_)
	{
		auto input_ = g_registry.try_get<Components::Input>(ent_);

		if (!input_)
			input_ = &g_registry.emplace<Components::Input>(ent_);

		input_->hitArea = hitArea_;
		input_->hitAreaCallback = hitAreaCallback_;

		queueForInsertion(ent_);
	}
}

void InputPlugin::setHitArea (Entity entity_, Shape hitArea_, HitCallback hitAreaCallback_)
{
	setHitArea(std::vector<Entity> {entity_}, hitArea_, hitAreaCallback_);
}

void InputPlugin::setHitAreaCircle (Entity entity_, double x_, double y_, double radius_, HitCallback callback_)
{
	Shape shape_( Circle ( x_, y_, radius_ ) );

	if (!callback_)
	{
		callback_ = std::function<bool(Shape, double, double, Entity)> (
			[] (Shape shape, double x, double y, Entity entity)
			{
				return CircleContains(shape, x, y);
			}
		);
	}

	setHitArea(entity_, shape_, callback_);
}

void InputPlugin::setHitAreaEllipse (Entity entity_, double x_, double y_, double width_, double height_, HitCallback callback_)
{
	Shape shape_( Ellipse ( x_, y_, width_, height_ ) );

	if (!callback_)
	{
		callback_ = std::function<bool(Shape, double, double, Entity)> (
			[] (Shape shape, double x, double y, Entity entity)
			{
				return EllipseContains(shape, x, y);
			}
		);
	}

	setHitArea(entity_, shape_, callback_);
}

void InputPlugin::setHitAreaFromTexture (Entity entity_, HitCallback callback_)
{
	if (!callback_)
	{
		callback_ = std::function<bool(Shape, double, double, Entity)> (
			[] (Shape shape, double x, double y, Entity entity)
			{
				return RectangleContains(shape, x, y);
			}
		);
	}

	auto [frame_, size_] = g_registry.try_get<Components::Frame, Components::Size>(entity_);

	double width_ = 0.;
	double height_ = 0.;

	if (size_)
	{
		width_ = size_->width;
		height_ = size_->height;
	}
	else if (frame_)
	{
		width_ = frame_->data.sourceSize.width;
		height_ = frame_->data.sourceSize.height;
	}

	bool container_ = g_registry.has<Components::Container>(entity_);

	if (container_ && (width_ == 0. || height_ == 0.))
	{
		MessageWarning("Making a container interactive requires a shape or setting its size first.");
		return;
	}

	if (width_ != 0. && height_ != 0.)
	{
		auto& input_ = g_registry.emplace<Components::Input>(entity_);

		input_.hitArea = Shape ( Rectangle ( 0., 0., width_, height_ ) );
		input_.hitAreaCallback = callback_;

		queueForInsertion(entity_);
	}
}

void InputPlugin::setHitAreaFromTexture (std::vector<Entity> entities_, HitCallback callback_)
{
	for (auto entity_ : entities_)
	{
		setHitAreaFromTexture(entity_, callback_);
	}
}

void InputPlugin::setHitAreaRectangle (Entity entity_, double x_, double y_, double width_, double height_, HitCallback callback_)
{
	Shape shape_ ( Rectangle ( x_, y_, width_, height_ ) );

	if (!callback_)
	{
		callback_ = std::function<bool(Shape, double, double, Entity)> (
			[] (Shape shape, double x, double y, Entity entity)
			{
				return RectangleContains(shape, x, y);
			}
		);
	}

	setHitArea(entity_, shape_, callback_);
}

void InputPlugin::setHitAreaTriangle (Entity entity_, double x1_, double y1_, double x2_, double y2_, double x3_, double y3_, HitCallback callback_)
{
	Shape shape_ ( Triangle ( x1_, y1_, x2_, y2_, x3_, y3_ ) );

	if (!callback_)
	{
		callback_ = std::function<bool(Shape, double, double, Entity)> (
			[] (Shape shape, double x, double y, Entity entity)
			{
				return TriangleContains(shape, x, y);
			}
		);
	}

	setHitArea(entity_, shape_, callback_);
}

void InputPlugin::enableDebug (Entity entity_, Color color_)
{
	// TODO debug
}

void InputPlugin::removeDebug (Entity entity_)
{
	// TODO debug
}

void InputPlugin::setPollAlways ()
{
	setPollRate(0);
}

void InputPlugin::setPollOnMove ()
{
	setPollRate(-1);
}

void InputPlugin::setPollRate (int value_)
{
	pollRate = value_;
	pollTimer = 0;
}

void InputPlugin::setGlobalTopOnly (bool value_)
{
	g_input.globalTopOnly = value_;
}

void InputPlugin::setTopOnly (bool value_)
{
	topOnly = value_;
}

void InputPlugin::sortGameObjects (std::vector<Entity> *entities_, Pointer *pointer_)
{
	if (entities_->size() < 2)
		return;

	auto list_ = GetRenderList(pointer_->camera);

	std::stable_sort(
		entities_->begin(),
		entities_->end(),
		[&] (Entity childA_, Entity childB_) -> bool {
			return IndexOf(*list_, childA_) > IndexOf(*list_, childB_);
		}
	);
}

void InputPlugin::sortDropZones (std::vector<Entity> *entities_)
{
	if (entities_->size() < 2)
		return;

	scene->sys.depthSort();

	std::stable_sort(
		entities_->begin(),
		entities_->end(),
		[&] (Entity childA_, Entity childB_) -> bool {
			return sortDropZoneHandler(childA_, childB_);
		}
	);
}

bool InputPlugin::sortDropZoneHandler (Entity childA_, Entity childB_)
{
	auto itemA_ = g_registry.try_get<Components::ContainerItem>(childA_);
	auto itemB_ = g_registry.try_get<Components::ContainerItem>(childB_);

	if (!itemA_ && !itemB_)
	{
		// Quick bail when neither child has a container
		return scene->children.getIndex(childA_) < scene->children.getIndex(childA_);
	}
	/*
	 * TODO container
	else if (itemA_->parent == itemB_->parent)
	{
		// Quick bail out when both children have the same container
		return
			itemB_->
	}
	*/
	else if (itemA_->parent == childB_)
	{
		return false;
	}
	else if (itemB_->parent == childA_)
	{
		return true;
	}
	else
	{
		// Container index check
		// TODO containers
		return true;
	}
}

void InputPlugin::stopPropagation ()
{
	g_input.tempSkip = true;
}

std::vector<Pointer*> InputPlugin::addPointer (int quantity_)
{
	return g_input.addPointer(quantity_);
}

void InputPlugin::setDefaultCursor (std::string textureKey_, std::string frameName_)
{
	// TODO cursor
	//g_input.setDefaultCursor(textureKey_, frameName_);
}

void InputPlugin::transitionIn ()
{
	enabled = scene->sys.settings.transitionAllowInput;
}

void InputPlugin::transitionComplete ()
{
	if (scene->sys.settings.transitionAllowInput)
		enabled = true;
}

void InputPlugin::transitionOut ()
{
	enabled = scene->sys.settings.transitionAllowInput;
}

void InputPlugin::shutdown ()
{
}

}	// namespace Zen
