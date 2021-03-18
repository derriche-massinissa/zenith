/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_COMPONENTS_TRANSFORM_H
#define ZEN_GAMEOBJECTS_COMPONENTS_TRANSFORM_H

#define FLAG 4 // 0100

namespace Zen {
namespace GameObjects {
namespace Components {

template <typename T>
class Transform
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

	/**
	 * @todo Finish the transfome GameObject component, and get an image to show up on the window!
	 * TODO
	 */

protected:
};

}	// namespace Components
}	// namespace GameObjects
}	// namespace Zen

#undef FLAG

#endif
