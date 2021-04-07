/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_INPUT_INPUTPLUGIN_HPP
#define ZEN_INPUT_INPUTPLUGIN_HPP

#include <vector>
#include <string>
#include <SDL2/SDL_types.h>
#include "../event/event_emitter.hpp"
#include "../ecs/entity.hpp"
#include "../display/types/color.hpp"
#include "../scene/scene.fwd.hpp"
#include "../scene/scene_systems.fwd.h"
#include "../scene/scene_settings.fwd.h"
#include "../cameras/2d/camera_manager.fwd.hpp"
#include "../gameobjects/display_list.fwd.h"
#include "types/hit_callback.hpp"
#include "types/event.hpp"
#include "pointer.fwd.hpp"
#include "types/input_configuration.hpp"
#include "const.hpp"

namespace Zen {

class InputPlugin : public EventEmitter
{
public:
	InputPlugin (Scene* scene_);

	void start ();

	void onWindowOver (InputEvent event_);

	void onWindowOut (InputEvent input_);

	void preUpdate (Uint32 time_, Uint32 delta_);

	bool isActive ();

	bool updatePoll (Uint32 time_, Uint32 delta_);

	bool update (INPUT type_, std::vector<Pointer*> pointers_);

	void clear (Entity entity_, bool skipQueue_ = false);

	void disable (Entity entity_);

	void enable (Entity entity_);

	void enable (Entity entity_, InputConfiguration config_);

	void enable (Entity entity_, Shape hitArea_, HitCallback hitAreaCallback_, bool dropZone_ = false);

	std::vector<Entity> hitTestPointer (Pointer *pointer_);

	int getDragState (Pointer *pointer_);

	void setDragState (Pointer *pointer_, int state_);

	int processDragThresholdEvent (Pointer *pointer_, Uint32 time_);

	int processDragStartList (Pointer *pointer_);

	int processDragDownEvent (Pointer *pointer_);

	int processDragMoveEvent (Pointer *pointer_);

	int processDragUpEvent (Pointer *pointer_);

	int processMoveEvents (Pointer *pointer_);

	int processWheelEvent (Pointer *pointer_);

	int processOverEvents (Pointer *pointer_);

	int processOutEvents (Pointer *pointer_);

	int processOverOutEvents (Pointer *pointer_);

	int processUpEvents (Pointer *pointer_);

	int processDownEvents (Pointer *pointer_);

	void queueForInsertion (Entity entity_);

	void queueForRemoval (Entity entity_);

	void setDraggable (Entity entity_, bool value_ = true);

	void setDraggable (std::vector<Entity> entity_, bool value_ = true);

	HitCallback makePixelPerfect (double alphaTolerance_);

	// Simple +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	void setHitArea (Entity entity_);

	void setHitArea (std::vector<Entity> entities_);

	// Config +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	void setHitArea (Entity entity_, InputConfiguration config_);

	void setHitArea (std::vector<Entity> entity_, InputConfiguration config_);

	// Shape + callback +++++++++++++++++++++++++++++++++++++++++++++++
	void setHitArea (Entity entity_, Shape hitArea_, HitCallback hitAreaCallback_);

	void setHitArea (std::vector<Entity> entities_, Shape hitArea_, HitCallback hitAreaCallback_);

	void setHitAreaCircle (Entity entity_, double x_, double y_, double radius_, HitCallback callback_ = nullptr);

	void setHitAreaEllipse (Entity entity_, double x_, double y_, double width_, double height_, HitCallback callback_ = nullptr);

	void setHitAreaFromTexture (Entity entity_, HitCallback callback_ = nullptr);

	void setHitAreaFromTexture (std::vector<Entity> entity_, HitCallback callback_ = nullptr);

	void setHitAreaRectangle (Entity entity_, double x_, double y_, double width_, double height_, HitCallback callback_ = nullptr);

	void setHitAreaTriangle (Entity entity_, double x1_, double y1_, double x2_, double y2_, double x3_, double y3_, HitCallback callback_ = nullptr);

	void enableDebug (Entity entity_, Color color_);

	void removeDebug (Entity entity_);

	void setPollAlways ();

	void setPollOnMove ();

	void setPollRate (int value_);

	void setGlobalTopOnly (bool value_);

	void setTopOnly (bool value_);

	void sortGameObjects (std::vector<Entity> *entities_, Pointer *pointer_);

	void sortDropZones (std::vector<Entity> *entities_);

	bool sortDropZoneHandler (Entity childA_, Entity childB_);

	void stopPropagation ();

	std::vector<Pointer*> addPointer (int quantity_);

	void setDefaultCursor (std::string textureKey_, std::string frameName_);

	bool topOnly = true;

private:
	Scene* scene;

	Scenes::SceneSystems *sys;

	Scenes::SceneSettings *settings;

	GameObjects::DisplayList *displayList;

	CameraManager *cameras;

	std::vector<Entity> list;

	std::vector<Entity> pendingRemoval;

	std::vector<Entity> pendingInsertion;

	std::vector<Entity> draggable;

	std::vector<std::vector<Entity>> drag;

	std::vector<std::vector<Entity>> over;

	std::vector<Entity> tempZones;

	std::vector<Entity> temp;

	std::vector<int> dragState;

	bool enabled = true;

	InputEvent tempEvent;

	double dragDistanceThreshold = 0.;

	Uint32 dragTimeThreshold = 0.;

	Uint32 pollTimer = 0;

	int pollRate = -1;

	bool updatedThisFrame = false;

	void transitionIn ();

	void transitionComplete ();

	void transitionOut ();

	void shutdown ();
};

}	// namespace Zen

#endif
