/**
 * @file		alpha.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef GAMEOBJECT_COMPONENT_ALPHA_H
#define GAMEOBJECT_COMPONENT_ALPHA_H

#include "math.h"
#include "../../defs.h"

#define FLAG 2 // 0010

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
	 * Alpha values are provided as a float between 0, fully transparent, and 1, fully opaque.
	 *
	 * You can optionally specify four different alpha values, each of which
	 * correspond to the four corners of the Game Object.
	 *
	 * @since 0.0.0
	 *
	 * @param topLeft The alpha value used for the top-left of the Game Object. If this is the only value given it's applied across the whole Game Object.
	 * @param topRight The alpha value used for the top-right of the Game Object.
	 * @param bottomLeft The alpha value used for the bottom-left of the Game Object.
	 * @param bottomRight The alpha value used for the bottom-right of the Game Object.
	 *
	 * @return This Game Object instance.
	 */
	T& setAlpha (float topLeft = 1, float topRight = -1, float bottomLeft = -1, float bottomRight = -1)
	{
		if (topRight == -1) {
			alpha_ = 1.0;
		} else {
			alphaTL_ = Math::clamp(topLeft, 0.0, 1.0);
			alphaTR_ = Math::clamp(topRight, 0.0, 1.0);
			alphaBL_ = Math::clamp(bottomLeft, 0.0, 1.0);
			alphaBR_ = Math::clamp(bottomRight, 0.0, 1.0);
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
	float getAlpha () { return alpha_; }
	float getAlphaTopLeft () { return alphaTL_; }
	float getAlphaTopRight () { return alphaTR_; }
	float getAlphaBottomLeft () { return alphaBL_; }
	float getAlphaBottomRight () { return alphaBR_; }

	/**
	 * Sets alpha values.
	 *
	 * @since 0.0.0
	 */
	T& setAlpha (float value)
	{
		float v = Math::clamp(value, 0.0, 1.0);

		alpha_ = v;
		alphaTL_ = v;
		alphaTR_ = v;
		alphaBL_ = v;
		alphaBR_ = v;

		if (!v)
			// Turn the alpha bit to 0
			This()->renderFlags &= ~FLAG;
		else
			// Turn the alpha bit to 1
			This()->renderFlags |= FLAG;

		return *This();
	}

	T& setAlphaTopLeft (float value)
	{
		float v = Math::clamp(value, 0.0, 1.0);

		alphaTL_ = v;

		if (v)
			// Turn the alpha bit to 1
			This()->renderFlags |= FLAG;

		return *This();
	}

	T& setAlphaTopRight (float value)
	{
		float v = Math::clamp(value, 0.0, 1.0);

		alphaTR_ = v;

		if (v)
			// Turn the alpha bit to 1
			This()->renderFlags |= FLAG;

		return *This();
	}

	T& setAlphaBottomLeft (float value)
	{
		float v = Math::clamp(value, 0.0, 1.0);

		alphaBL_ = v;

		if (v)
			// Turn the alpha bit to 1
			This()->renderFlags |= FLAG;

		return *This();
	}

	T& setAlphaBottomRight (float value)
	{
		float v = Math::clamp(value, 0.0, 1.0);

		alphaBR_ = v;

		if (v)
			// Turn the alpha bit to 1
			This()->renderFlags |= FLAG;

		return *This();
	}

private:
	/**
	 * Holds the global alpha value.
	 *
	 * @since 0.0.0
	 */
	float alpha_ = 1;

	/**
	 * Holds the top-left alpha value.
	 *
	 * @since 0.0.0
	 */
	float alphaTL_ = 1;

	/**
	 * Holds the top-right alpha value.
	 *
	 * @since 0.0.0
	 */
	float alphaTR_ = 1;

	/**
	 * Holds the bottom-left alpha value.
	 *
	 * @since 0.0.0
	 */
	float alphaBL_ = 1;

	/**
	 * Holds the bottom-right alpha value.
	 *
	 * @since 0.0.0
	 */
	float alphaBR_ = 1;
};

}	// namespace Components
}	// namespace GameObjects
}	// namespace Zen

#undef FLAG

#endif
