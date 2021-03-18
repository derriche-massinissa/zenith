/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECT_DISPLAYLIST_H
#define ZEN_GAMEOBJECT_DISPLAYLIST_H

#include <memory>
#include <vector>

#include "gameobject.fwd.h"

namespace Zen {
namespace GameObjects {

/**
 * The Display List plugin.
 *
 * A DisplayList belongs to a Scene and maintain the list of Game Objects to render
 * every frame.
 *
 * Some of these GameObjects may also be part of the Scene's UpdateList, for
 * updating.
 *
 * @class DisplayList
 * @since 0.0.0
 */
class DisplayList
{
public:
	/**
	 * The list owning the GameObject instances.
	 *
	 * @since 0.0.0
	 */
	std::vector<std::unique_ptr<GameObject>> list;

	/**
	 * The flag the determines whether GameObjects should be sorted when
	 * `depthSort()` is called.
	 *
	 * @since 0.0.0
	 */
	bool sortChildrenFlag = false;

	/**
	 * Add a GameObject instance to this display list.
	 *
	 * @since 0.0.0
	 *
	 * @param gameObject_ A unique pointer to a GameObject instance.
	 */
	GameObject* add (std::unique_ptr<GameObject> gameObject_);

	/**
	 * Remove a GameObject instance from the list.
	 *
	 * @since 0.0.0
	 *
	 * @param gameObject_ A pointer to the GameObject instance.
	 */
	void remove (GameObject* gameObject_);

	/**
	 * Force a sort of the display list on the next call to depthSort.
	 *
	 * @since 0.0.0
	 */
	void queueDepthSort ();

	/**
	 * Immediately sorts the display list if the flag is set.
	 *
	 * @since 0.0.0
	 */
	void depthSort ();

	/**
	 * Compare the depth of two GameObjects.
	 *
	 * @since 0.0.0
	 *
	 * @param childA The first GameObject.
	 * @param childB The second GameObject.
	 *
	 * @return `true` if childA has a smaller depth than childB, and `false`
	 * otherwise.
	 */
	static bool sortByDepth (
			const std::unique_ptr<GameObject>& childA,
			const std::unique_ptr<GameObject>& childB);

	/**
	 * Returns a vector which contains all objects currently on the DisplayList.
	 *
	 * @since 0.0.0
	 *
	 * @return The GameObject instances.
	 */
	std::vector<GameObject*> getChildren ();

	int getIndex (GameObject* child_);
};

}	// namespace Input
}	// namespace Zen

#endif
