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

#include "../ecs/entity.hpp"
#include "../structs/list.hpp"

namespace Zen {

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
class DisplayList : public List<Entity>
{
public:
	/**
	 * The flag the determines whether GameObjects should be sorted when
	 * `depthSort()` is called.
	 *
	 * @since 0.0.0
	 */
	bool sortChildrenFlag = false;

	/**
	 * @since 0.0.0
	 */
	DisplayList ();

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
	static bool sortByDepth (Entity childA, Entity childB);

	/**
	 * Returns a vector which contains all objects currently on the DisplayList.
	 *
	 * @since 0.0.0
	 *
	 * @return The GameObject instances.
	 */
	std::vector<Entity> getChildren ();

	int getIndex (Entity child);

	/**
     * The depth of this Game Object within the Scene.
     *
     * The depth is also known as the 'z-index' in some environments, and allow
	 * you to change the rendering order of Game Objects, without actually
	 * moving their position in the display list.
     *
     * The default depth is zero. A Game Object with a higher depth
     * value will always render in front of one with a lower value.
     *
     * Setting the depth will queue a depth sort event within the Scene.
     *
	 * @since 0.0.0
	 */
	void setDepth (Entity entity, int depth);
};

}	// namespace Zen

#endif
