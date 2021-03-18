/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_COMPONENTS_ALPHA_H
#define ZEN_GAMEOBJECTS_COMPONENTS_ALPHA_H

#include "../../math/clamp.h"
#include "../../defs.h"

namespace Zen {
namespace GameObjects {
namespace Components {

/**
 * Provides methods used for setting the alpha properties of a Game Object.
 * Should be inherited and not used directly.
 *
 * @since 0.0.0
 */
template <typename T>
class Alpha
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
	 * Clears all alpha values associated with this Game Object.
	 *
	 * Immediately sets the alpha levels back to 1 (fully opaque).
	 *
	 * @since 0.0.0
	 *
	 * @return This Game Object instance.
	 */
	T& clearAlpha () {
		return setAlpha(1);
	}

	/**
	 * Set the Alpha level of this Game Object. The alpha controls the opacity of the Game Object as it renders.
	 * Alpha values are provided as a double between 0, fully transparent, and 1, fully opaque.
	 *
	 * You can optionally specify four different alpha values, each of which
	 * correspond to the four corners of the Game Object.
	 *
	 * @since 0.0.0
	 *
	 * @param topLeft_ The alpha value used for the top-left of the Game Object. If this is the only value given it's applied across the whole Game Object.
	 * @param topRight_ The alpha value used for the top-right of the Game Object.
	 * @param bottomLeft_ The alpha value used for the bottom-left of the Game Object.
	 * @param bottomRight_ The alpha value used for the bottom-right of the Game Object.
	 *
	 * @return This Game Object instance.
	 */
	T& setAlpha (double topLeft_ = 1, double topRight_ = -1, double bottomLeft_ = -1, double bottomRight_ = -1)
	{
		if (topRight_ == -1) {
			alpha = 1.0;
		} else {
			alphaTL = Math::clamp(topLeft_, 0.0, 1.0);
			alphaTR = Math::clamp(topRight_, 0.0, 1.0);
			alphaBL = Math::clamp(bottomLeft_, 0.0, 1.0);
			alphaBR = Math::clamp(bottomRight_, 0.0, 1.0);
		}

		return *This();
	}

	/**
	 * Returns alpha values.
	 *
	 * @since 0.0.0
	 *
	 * @return Alpha value.
	 */
	double getAlpha () { return alpha; }
	double getAlphaTopLeft () { return alphaTL; }
	double getAlphaTopRight () { return alphaTR; }
	double getAlphaBottomLeft () { return alphaBL; }
	double getAlphaBottomRight () { return alphaBR; }

	/**
	 * Sets alpha values.
	 *
	 * @since 0.0.0
	 */
	T& setAlpha (double value_)
	{
		double v_ = Math::clamp(value_, 0.0, 1.0);

		alpha = v_;
		alphaTL = v_;
		alphaTR = v_;
		alphaBL = v_;
		alphaBR = v_;

		if (!v_)
			// Turn the alpha bit to 0
			This()->renderFlags &= ~FLAG;
		else
			// Turn the alpha bit to 1
			This()->renderFlags |= FLAG;

		return *This();
	}

	T& setAlphaTopLeft (double value_)
	{
		double v_ = Math::clamp(value_, 0.0, 1.0);

		alphaTL = v_;

		if (v_)
			// Turn the alpha bit to 1
			This()->renderFlags |= FLAG;

		return *This();
	}

	T& setAlphaTopRight (double value_)
	{
		double v_ = Math::clamp(value_, 0.0, 1.0);

		alphaTR = v_;

		if (v_)
			// Turn the alpha bit to 1
			This()->renderFlags |= FLAG;

		return *This();
	}

	T& setAlphaBottomLeft (double value_)
	{
		double v_ = Math::clamp(value_, 0.0, 1.0);

		alphaBL = v_;

		if (v_)
			// Turn the alpha bit to 1
			This()->renderFlags |= FLAG;

		return *This();
	}

	T& setAlphaBottomRight (double value_)
	{
		double v_ = Math::clamp(value_, 0.0, 1.0);

		alphaBR = v_;

		if (v_)
			// Turn the alpha bit to 1
			This()->renderFlags |= FLAG;

		return *This();
	}

protected:
	/**
	 * Holds the global alpha value.
	 *
	 * @since 0.0.0
	 */
	double alpha = 1;

	/**
	 * Holds the top-left alpha value.
	 *
	 * @since 0.0.0
	 */
	double alphaTL = 1;

	/**
	 * Holds the top-right alpha value.
	 *
	 * @since 0.0.0
	 */
	double alphaTR = 1;

	/**
	 * Holds the bottom-left alpha value.
	 *
	 * @since 0.0.0
	 */
	double alphaBL = 1;

	/**
	 * Holds the bottom-right alpha value.
	 *
	 * @since 0.0.0
	 */
	double alphaBR = 1;

private:
	static const int FLAG = 0b0010;
};

}	// namespace Components
}	// namespace GameObjects
}	// namespace Zen

#endif
