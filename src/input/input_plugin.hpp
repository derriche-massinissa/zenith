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
#include "../ecs/entity.hpp"
#include "../display/types/color.hpp"

namespace Zen {

struct Shape;
struct Pointer;

class InputPlugin{
	using HitCallback = std::function<bool(Shape, double, double, Entity)>;

public:
	InputPlugin ();
	~InputPlugin ();

	void onWindowOver ();

	void onWindowOut ();

	void preUpdate ();

	bool isActive ();

	void update ();

	void clear (Entity entity_, bool skipQueue_);

	void disable (Entity entity_);

	void enable (Entity entity_, Shape hitArea_, HitCallback hitAreaCallback_, bool dropZone_);

	std::vector<Entity> hitTestPointer (Pointer pointer_);

	int getDragState (Pointer pointer_);

	void setDragState (Pointer pointer_, int state_);

	void processDragThresholdEvent (Pointer pointer_, Uint32 time_);

	int processDragStartList (Pointer pointer_);

	int processDragDownEvent (Pointer pointer_);

	int processDragMoveEvent (Pointer pointer_);

	int processDragUpEvent (Pointer pointer_);

	int processMoveEvents (Pointer pointer_);

	int processWheelEvents (Pointer pointer_);

	int processOverEvents (Pointer pointer_);

	int processOutEvents (Pointer pointer_);

	int processOverOutEvents (Pointer pointer_);

	int processUpEvents (Pointer pointer_);

	int processDownEvents (Pointer pointer_);

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

	std::vector<Entity> sortEntities (std::vector<Entity> entities_, Pointer pointer);

	std::vector<Entity> sortDropZones (std::vector<Entity> entities_);

	int sortDropZoneHandler (Entity childA, Entity childB);

	void stopPropagation ();

	std::vector<Pointer> addPointer (int quantity_);

	void setDefaultCursor (std::string textureKey_, std::string frameName_);

private:
	// TODO Move private members here pls
	bool transitionIn ();

	bool transitionComplete ();

	bool transitionOut ();

	void shutdown ();
};

}	// namespace Zen

#endif
