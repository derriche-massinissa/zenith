/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_COMPONENT_TINT_H
#define ZEN_GAMEOBJECTS_COMPONENT_TINT_H

namespace Zen {
namespace GameObjects {
namespace Components {

template <typename T>
class Tint
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

	T& clearTint ()
	{
		setTint(0xffffff);

		return *This();
	}

	T& setTint (int topLeft_ = 0xffffff, int topRight_ = -1, int bottomLeft_ = -1, int bottomRight_ = -1)
	{
		tintTopLeft = topLeft_;

		if (topRight_ < 0)
		{
			tintTopRight = topLeft_;
			tintBottomLeft = topLeft_;
			tintBottomRight = topLeft_;
		}

		tintFill = false;

		return *This();
	}

	T& setTintFill (int topLeft_ = 0xffffff, int topRight_ = 0xffffff, int bottomLeft_ = 0xffffff, int bottomRight_ = 0xffffff)
	{
		setTint(topLeft_, topRight_, bottomLeft_, bottomRight_);

		tintFill = true;

		return *This();
	}

	bool isTinted ()
	{
		return (
			tintFill ||
			tintTopLeft != 0xffffff ||
			tintTopRight != 0xffffff ||
			tintBottomLeft != 0xffffff ||
			tintBottomRight != 0xffffff
			);
	}

	int tintTopLeft = 0xffffff;
	int tintTopRight = 0xffffff;
	int tintBottomLeft = 0xffffff;
	int tintBottomRight = 0xffffff;
	bool tintFill = false;
};

}	// namespace Components
}	// namespace Textures
}	// namespace Zen

#endif
