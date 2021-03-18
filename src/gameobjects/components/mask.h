/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_COMPONENTS_MASK_H
#define ZEN_GAMEOBJECTS_COMPONENTS_MASK_H

namespace Zen {
namespace GameObjects {
namespace Components {

template <typename T>
class Mask
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

	GameObject* getMask ()
	{
		return mask;
	}

	T& setMask (GameObject& mask_)
	{
		mask = &mask_;

		return *This();
	}

	T& clearMask ()
	{
		mask = nullptr;

		return *This();
	}

protected:
	GameObject* mask = nullptr;
};

}	// namespace Components
}	// namespace GameObjects
}	// namespace Zen

#endif
