/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECT_UPDATELIST_H
#define ZEN_GAMEOBJECT_UPDATELIST_H

#include <vector>
#include <SDL2/SDL_types.h>

#include "../ecs/entity.hpp"
#include "../scene/scene.fwd.h"
#include "../data.h"
#include "../event/event_listener.hpp"

namespace Zen {
namespace GameObjects {

/**
 * The Update List plugin.
 *
 * Update Lists belong to a Scene and maintain the list GameObjects to be updated
 * every frame.
 *
 * Some or all of these Game Objects may also be part of the Scene's DisplayList,
 * for Rendering.
 *
 * @class UpdateList
 * @since 0.0.0
 *
 * @param scene The Scene that the UpdateList belongs to.
 */
class UpdateList
{
public:
	UpdateList (Scene* scene_);
	~UpdateList ();

	Scene* scene = nullptr;

	bool chechQueue = true;

	std::vector<Entity> pending;

	std::vector<Entity> active;

	std::vector<Entity> destroy;

	int toProcess = 0;

	ListenerBase* lStart = nullptr;
	ListenerBase* lPreUpdate = nullptr;
	ListenerBase* lUpdate = nullptr;
	ListenerBase* lShutdown = nullptr;

	void start ();

	void sceneUpdate (Uint32 time_, Uint32 delta_);

	void shutdown ();

	/**
	 * Add a GameObject instance to this display list.
	 *
	 * @since 0.0.0
	 *
	 * @param gameObject_ A unique pointer to a GameObject instance.
	 */
	void add (Entity gameObject_);

	/**
	 * Remove a GameObject instance from the list.
	 *
	 * @since 0.0.0
	 *
	 * @param gameObject_ A pointer to the GameObject instance.
	 */
	void remove (Entity gameObject_);

	void removeAll ();

	void update (Uint32 time_, Uint32 delta_);

	std::vector<Entity> getActive ();

	int getLength ();
};

}	// namespace GameObjects
}	// namespace Zen

#endif
