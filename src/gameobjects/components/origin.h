/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_COMPONENTS_ORIGIN_H
#define ZEN_GAMEOBJECTS_COMPONENTS_ORIGIN_H

namespace Zen {
namespace GameObjects {
namespace Components {

template <typename T>
class Origin
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

	int getDisplayOriginX ()
	{
		return displayOriginX;
	}

	T& setDisplayOriginX (int value_)
	{
		displayOriginX = value_;
		originX = value_ / This()->getWidth();

		return *This();
	}

	int getDisplayOriginY ()
	{
		return displayOriginY;
	}

	T& setDisplayOriginY (int value_)
	{
		displayOriginY = value_;
		originY = value_ / This()->getHeight();

		return *This();
	}

	T& setOrigin (double x_, double y_)
	{
		originX = x_;
		originY = y_;

		return updateDisplayOrigin();
	}

	T& setOrigin (double value_ = 0.5)
	{
		return setOrigin(value_, value_);
	}

	T& setOriginFromFrame ()
	{
		if (This()->frame == nullptr || !This()->frame->customPivot)
		{
			return setOrigin();
		}
		else
		{
			originX = This()->frame->pivotX;
			originY = This()->frame->pivotY;
		}

		return updateDisplayOrigin();
	}

	T& setDisplayOrigin (int x_, int y_)
	{
		displayOriginX = x_;
		displayOriginY = y_;

		return *This();
	}

	T& setDisplayOrigin (int value_ = 0)
	{
		return *This();
	}

	T& updateDisplayOrigin ()
	{
		displayOriginX = originX * This()->width;
		displayOriginY = originY * This()->height;

		return *This();
	}

	double getOriginX ()
	{
		return originX;
	}

	double getOriginY ()
	{
		return originY;
	}

protected:
	double originX = 0.5;
	double originY = 0.5;
	int displayOriginX = 0;
	int displayOriginY = 0;
};

}	// namespace Components
}	// namespace GameObjects
}	// namespace Zen

#endif
