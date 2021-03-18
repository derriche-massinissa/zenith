/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_COMPONENT_VISIBLE_H
#define ZEN_GAMEOBJECTS_COMPONENT_VISIBLE_H

#define FLAG 1 // 0001

namespace Zen {
namespace GameObjects {
namespace Components {

/**
 * Provides methods used for setting the visibility of a Game Object.
 * An invisible Game Object will skip rendering, but will still process update logic.
 *
 * Should be inherited and not used directly.
 * 
 * @since 0.0.0
 */
template <typename T>
class Visible
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
	 * Returns the visibility of this Game Object.
	 *
	 * @since 0.0.0
	 *
	 * @return The visible state of this Game Object.
	 */
	bool getVisible ()
	{
		return visible;
	}

	/**
	 * Sets the visibility of this Game Object.
	 *
	 * @since 0.0.0
	 * @param value_ The visible state of this Game Object.
	 *
	 * @return A reference to this Game Object instance.
	 */
	T& setVisible (bool value_)
	{
		if (value_) {
			visible = true;
			This()->renderFlags |= FLAG;
		} else {
			visible = false;
			This()->renderFlags &= ~FLAG;
		}

		return *This();
	}

protected:
	/**
	 * Private internal value. Holds the visible value.
	 * 
	 * @since 0.0.0
	 */
	bool visible = true;
};

}	// namespace Components
}	// namespace GameObjects
}	// namespace Zen

#undef FLAG

#endif
