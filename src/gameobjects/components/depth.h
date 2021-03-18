/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_COMPONENTS_DEPTH_H
#define ZEN_GAMEOBJECTS_COMPONENTS_DEPTH_H

namespace Zen {
namespace GameObjects {
namespace Components {

template <typename T>
class Depth
{
public:
	/**
	 * Returns a cast `this` pointer, with the type of the descendant class.
	 *
	 * @since 0.0.0
	 * @return Pointer to this instance of T.
	 */
	T* This ()
	{
		return static_cast<T*>(this);
	}

	int getDepth ()
	{
		return depth;
	}

	T& setDepth (int value = 0)
	{
		depth = value;

		return *This();
	}

protected:
	int depth = 0;
};

}	// namespace Components
}	// namespace GameObjects
}	// namespace Zen

#endif
