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

namespace Zen {

class InputPlugin : public EventEmitter
{
public:
	InputPlugin ();
	~InputPlugin ();

	void onWindowOver ();

	void onWindowOut ();

	void preUpdate ();

	bool isActive ();

	void update ();

	void clear (Entity entity_, bool skipQueue_ = false);

	void disable (Entity entity_);

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

	int processWheelEvents (Pointer *pointer_);

	int processOverEvents (Pointer *pointer_);

	int processOutEvents (Pointer *pointer_);

	int processOverOutEvents (Pointer *pointer_);

	int processUpEvents (Pointer *pointer_);

	int processDownEvents (Pointer *pointer_);

	void queueForInsertion (Entity entity_);

	void queueForRemoval (Entity entity_);

	void setDraggable (Entity entity_, bool value_ = true);

	HitCallback makePixelPerfect (double alphaTolerance_);

	void setHitArea (Entity entity_, Shape hitArea_, HitCallback hitAreaCallback_);

	void setHitAreaCircle (Entity entity_, double x_, double y_, double radius_, HitCallback callback_ = nullptr);

	void setHitAreaEllipse (Entity entity_, double x_, double y_, double width_, double height_, HitCallback callback_ = nullptr);

	void setHitAreaFromTexture (Entity entity_, HitCallback callback_ = nullptr);

	void setHitAreaRectangle (Entity entity_, double x_, double y_, double width_, double height_, HitCallback callback_ = nullptr);

	void setHitAreaTriangle (Entity entity_, double x1_, double y1_, double x2_, double y2_, double x3_, double y3_, HitCallback callback_ = nullptr);

	void enableDebug (Entity entity_, Color color_);

	void removeDebug (Entity entity_);

	void setPollAlways ();

	void setPollOnMove ();

	void setPollRate ();

	void setGlobalTopOnly (bool value_);

	void setTopOnly (bool value_);

	std::vector<Entity> sortEntities (std::vector<Entity> entities_, Pointer *pointer_);

	std::vector<Entity> sortDropZones (std::vector<Entity> entities_);

	int sortDropZoneHandler (Entity childA_, Entity childB_);

	void stopPropagation ();

	std::vector<Pointer> addPointer (int quantity_);

	void setDefaultCursor (std::string textureKey_, std::string frameName_);

	void resetInputEvent (InputEvent* event_);

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

	std::vector<Entity> over;

	std::vector<Entity> tempZones;

	std::vector<Entity> temp;

	std::vector<int> dragState;

	bool enabled = true;

	bool topOnly = true;

	InputEvent tempEvent;

	double dragDistanceThreshold = 0.;

	Uint32 dragTimeThreshold = 0.;

	bool transitionIn ();

	bool transitionComplete ();

	bool transitionOut ();

	void shutdown ();
};

}	// namespace Zen

#endif
