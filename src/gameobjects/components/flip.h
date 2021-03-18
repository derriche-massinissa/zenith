/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_COMPONENT_FLIP_H
#define ZEN_GAMEOBJECTS_COMPONENT_FLIP_H

namespace Zen {
namespace GameObjects {
namespace Components {

template <typename T>
class Flip
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

	T& toggleFlipX ()
	{
		flipX = !flipX;

		return This();
	}

	T& toggleFlipY ()
	{
		flipY = !flipY;

		return This();
	}

	T& setFlipX (bool value_)
	{
		flipX = value_;

		return This();
	}

	T& setFlipY (bool value_)
	{
		flipY = value_;

		return This();
	}

	T& setFlip (bool x_, bool y_)
	{
		flipX = x_;
		flipY = y_;

		return This();
	}

	T& resetFlip ()
	{
		flipX = false;
		flipY = false;

		return *This();
	}

	bool flipX = false;
	bool flipY = false;
};

}	// namespace Components
}	// namespace Textures
}	// namespace Zen

#endif
