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
#include "gameobject.fwd.h"

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

	std::vector<GameObject*> pending;

	std::vector<GameObject*> active;

	std::vector<GameObject*> destroy;

	int toProcess = 0;

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
	void add (GameObject* gameObject_);

	/**
	 * Remove a GameObject instance from the list.
	 *
	 * @since 0.0.0
	 *
	 * @param gameObject_ A pointer to the GameObject instance.
	 */
	void remove (GameObject* gameObject_);

	void removeAll ();

	std::vector<GameObject*> update ();

	std::vector<GameObject*> getActive ();

	int getLength ();
};

}	// namespace Input
}	// namespace Zen

#endif
