/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_COMPONENTS_INPUT_HPP
#define ZEN_COMPONENTS_INPUT_HPP

#include "../ecs/entity.hpp"
#include "../geom/types/shape.hpp"
#include "../input/types/hit_callback.hpp"
#include <string>

namespace Zen {
namespace Components {

/**
 * @since 0.0.0
 *
 * @property enabled Is this Interactive Object currently enabled for input events?
 * @property alwaysEnabled An Interactive Object that is 'always enabled' will receive input even if the parent object is invisible or won't render.
 * @property draggable Is this Interactive Object draggable? Enable with InputPlugin::setDraggable.
 * @property dropZone Is this Interactive Object a drag-targets drop zone? Set when the object is created.
 * @property cursor Should this Interactive Object change the cursor when over? (desktop only)
 * @property target An optional drop target for a draggable Interactive Object.
 * @property camera The most recent Camera to be tested against this Interactive Object.
 * @property hitArea The hit area for this Interactive Object. Typically a geometry shape, like a Rectangle or Circle.
 * @property hitAreaCallback The 'contains' check callback that the hit area shape will use for all hit tests.
 * @property hitAreaDebug If this Interactive Object has been enabled for debug, via `InputPlugin.enableDebug` then this property holds its debug shape.
 * @property customHitArea Was the hitArea for this Interactive Object created based on texture size (false), or a custom shape? (true)
 * @property localX The x coordinate that the Pointer interacted with this object on, relative to the Game Object's top-left position.
 * @property localY The y coordinate that the Pointer interacted with this object on, relative to the Game Object's top-left position.
 * @property dragState The current drag state of this Interactive Object. 0 = Not being dragged, 1 = being checked for drag, or 2 = being actively dragged.
 * @property dragStartX The x coordinate of the Game Object that owns this Interactive Object when the drag started.
 * @property dragStartY The y coordinate of the Game Object that owns this Interactive Object when the drag started.
 * @property dragStartXGlobal The x coordinate that the Pointer started dragging this Interactive Object from.
 * @property dragStartYGlobal The y coordinate that the Pointer started dragging this Interactive Object from.
 * @property dragX The x coordinate that this Interactive Object is currently being dragged to.
 * @property dragY The y coordinate that this Interactive Object is currently being dragged to.
 */
struct Input
{
	bool enabled = true;
	bool alwaysEnabled = false;
	bool draggable = false;
	bool dropZone = false;
	std::string cursor;
	Entity target = entt::null;
	Entity camera = entt::null;
	Shape hitArea;
	HitCallback hitAreaCallback = nullptr;
	Shape hitAreaDebug;
	bool customHitArea = false;
	double localX = 0.;
	double localY = 0.;
	int dragState = 0;
	double dragStartX = 0.;
	double dragStartY = 0.;
	double dragStartXGlobal = 0.;
	double dragStartYGlobal = 0.;
	double dragX = 0.;
	double dragY = 0.;

	Input ()
	{}

	Input (const Input& other)
		: enabled (other.enabled)
		, alwaysEnabled (other.alwaysEnabled)
		, draggable (other.draggable)
		, dropZone (other.dropZone)
		, cursor (other.cursor)
		, target (other.target)
		, camera (other.camera)
		, hitArea (other.hitArea)
		, hitAreaCallback (other.hitAreaCallback)
		, hitAreaDebug (other.hitAreaDebug)
		, customHitArea (other.customHitArea)
		, localX (other.localX)
		, localY (other.localY)
		, dragState (other.dragState)
		, dragStartX (other.dragStartX)
		, dragStartY (other.dragStartY)
		, dragStartXGlobal (other.dragStartXGlobal)
		, dragStartYGlobal (other.dragStartYGlobal)
		, dragX (other.dragX)
		, dragY (other.dragY)
	{}

	Input (Input&& other)
		: enabled (other.enabled)
		, alwaysEnabled (other.alwaysEnabled)
		, draggable (other.draggable)
		, dropZone (other.dropZone)
		, cursor (other.cursor)
		, target (other.target)
		, camera (other.camera)
		, hitArea (std::move(other.hitArea))
		, hitAreaCallback (std::move(other.hitAreaCallback))
		, hitAreaDebug (std::move(other.hitAreaDebug))
		, customHitArea (other.customHitArea)
		, localX (other.localX)
		, localY (other.localY)
		, dragState (other.dragState)
		, dragStartX (other.dragStartX)
		, dragStartY (other.dragStartY)
		, dragStartXGlobal (other.dragStartXGlobal)
		, dragStartYGlobal (other.dragStartYGlobal)
		, dragX (other.dragX)
		, dragY (other.dragY)
	{}

	void operator = (Input&& other)
	{
		enabled = other.enabled;

		alwaysEnabled = other.alwaysEnabled;

		draggable = other.draggable;

		dropZone = other.dropZone;

		cursor = other.cursor;

		target = other.target;

		camera = other.camera;

		hitArea = std::move(other.hitArea);

		hitAreaCallback = other.hitAreaCallback;

		hitAreaDebug = std::move(other.hitAreaDebug);

		customHitArea = other.customHitArea;

		localX = other.localX;

		localY = other.localY;

		dragState = other.dragState;

		dragStartX = other.dragStartX;

		dragStartY = other.dragStartY;

		dragStartXGlobal = other.dragStartXGlobal;

		dragStartYGlobal = other.dragStartYGlobal;

		dragX = other.dragX;

		dragY = other.dragY;
	}
};

}	// namespace Components
}	// namespace Zen

#endif
