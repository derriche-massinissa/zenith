/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_GAMEOBJECT_H
#define ZEN_GAMEOBJECTS_GAMEOBJECT_H

#include <string>
#include <vector>
#include <SDL2/SDL_types.h>

#include "../event/event_emitter.h"

#include "../cameras/2d/camera.fwd.h"
#include "../texture/frame.h"
#include "../texture/crop_data.h"
#include "components/transform_matrix.h"
#include "display_list.h"

#include "components/alpha.h"
#include "components/blend_mode.h"
#include "components/depth.h"
#include "components/get_bounds.h"
#include "components/mask.h"
#include "components/origin.h"
#include "components/scroll_factor.h"
#include "components/size.h"
#include "components/texture_crop.h"
#include "components/tint.h"
#include "components/transform.h"
#include "components/visible.h"

namespace Zen {
namespace GameObjects {

/**
 * The base class that all Game Objects inherit from.
 * You don't create GameObjects directly and they cannot be added to the display 
 * list.
 * Instead, use them as the base for your own custom classes.
 *
 * @class GameObject
 * @since 0.0.0
 */
class GameObject
	: public Events::EventEmitter
	, public Components::Alpha<GameObject>
	, public Components::BlendMode<GameObject>
	, public Components::Depth<GameObject>
	, public Components::Flip<GameObject>
	, public Components::GetBounds<GameObject>
	, public Components::Mask<GameObject>
	, public Components::Origin<GameObject>
	, public Components::ScrollFactor<GameObject>
	, public Components::Size<GameObject>
	, public Components::TextureCrop<GameObject>
	, public Components::Tint<GameObject>
	, public Components::Transform<GameObject>
	, public Components::Visible<GameObject>
{
public:
	/**
	 * @since 0.0.0
	 *
	 * @param scene - The Scene to which this Game Object belongs.
	 */
	GameObject (Scene* scene_);

	/**
	 * @since 0.0.0
	 */
	~GameObject ();

	static const int COMPONENT_MASK =
		COMPONENT_MASK_ALPHA			|
		COMPONENT_MASK_BLENDMODE		|
		COMPONENT_MASK_DEPTH			|
		COMPONENT_MASK_FLIP				|
		COMPONENT_MASK_BOUNDS			|
		COMPONENT_MASK_MASK				|
		COMPONENT_MASK_ORIGIN			|
		COMPONENT_MASK_SCROLLFACTOR		|
		COMPONENT_MASK_SIZE				|
		COMPONENT_MASK_TEXTURECROP		|
		COMPONENT_MASK_TINT				|
		COMPONENT_MASK_TRANSFORM		|
		COMPONENT_MASK_VISIBLE			;

	Scene* scene = nullptr;

	DisplayList* displayList = nullptr;

	std::string state = "";

	GameObject* parentContainer = nullptr;
	Components::TransformMatrix* parentMatrix = nullptr;

	std::string name = "";

	bool active = true;

	int tabIndex = -1;

	/**
	 * @todo TODO DataManager for GameObject
	 */
	// Data::DataManager data;

	int renderFlags = 15;

	int cameraFilter = 0;

	Textures::CropData crop;

	/**
	 * @todo TODO interactivity for GameObject
	 */
	//std::unique_ptr<Input::InteractiveObject> input = nullptr;

	/**
	 * @todo TODO physics body for GameObject
	 */
	//std::unique_ptr<Physics::Box2D::StaticBody> body = nullptr;

	/**
	 * A flag that indicates if this GameObject is a parent, meaning that it can
	 * hold other GameObject instances.
	 *
	 * @since 0.0.0
	 */
	static const bool isParent = false;

	/**
	 * Given a component mask, it returns `true` if the GameObject has inherited
	 * from that component, or `false` otherwise.
	 *
	 * @since 0.0.0
	 */
	static bool hasComponent (int compMask_);

	void setActive (bool value_);

	void setName (std::string name_);

	void setState (std::string state_);

	/**
	 * @todo TODO DataManager for GameObject
	 */
	/*
	GameObject& setDataEnabled ()
	{
		if (!data)
			data = std::make_unique<Data::DataManager>();

		return *This();
	}
	template <typename T>
	GameObject& setData(std::string key_, T data_);
	GameObject& incData(std::string key_, T data_);
	GameObject& toggleData(std::string key_);
	GameObject& getData(std::string key_);
	GameObject& setInteractive(Input::Configuration hitArea_, Input::HitAreaCallback hitAreaCallback_, bool dropZone_);
	GameObject& disableInteractive();
	GameObject& removeInteractive();
	*/

	/**
	 * This callback is invoked when this Game Object is added to a Scene.
	 *
	 * Can be overriden by custom Game Objects, but be aware of some Game Objects
	 * that will use this, such as Sprites, to add themselves into the Update List.
	 *
	 * You can also listen for the `added-to-scene` event from this Game Object.
	 *
	 * @since 0.0.0
	 */
	virtual void addedToScene ();

	virtual void removedFromScene ();

	virtual void preUpdate (Uint32 time_, Uint32 delta_);

	virtual void update (Uint32 time_, Uint32 delta_);

	bool willRender (Cameras::Scene2D::Camera& camera_);

	std::vector<int> getIndexList ();

	int getIndex (GameObject* child_);

	virtual void preDestroy ();

	void destroy ();
};

}	// namespace GameObjects
}	// namespace Zen

#endif
