/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_COMPONENTS_SCROLLFACTOR_H
#define ZEN_GAMEOBJECTS_COMPONENTS_SCROLLFACTOR_H

namespace Zen {
namespace GameObjects {
namespace Components {

template <typename T>
class ScrollFactor
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

	T& setScrollFactor (double x_, double y_)
	{
		scrollFactorX = x_;
		scrollFactorY = y_;

		return *This();
	}

	T& setScrollFactor (double value_)
	{
		return setScrollFactor(value_, value_);
	}

	double getScrollFactorX ()
	{
		return scrollFactorX;
	}

	double getScrollFactorY ()
	{
		return scrollFactorY;
	}

	double scrollFactorX = 1.0;
	double scrollFactorY = 1.0;
};

}	// namespace Components
}	// namespace GameObjects
}	// namespace Zen

#endif
