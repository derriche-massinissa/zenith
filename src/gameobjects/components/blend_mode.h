/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_COMPONENTS_BLENDMODE_H
#define ZEN_GAMEOBJECTS_COMPONENTS_BLENDMODE_H

#include "../../const.h"

namespace Zen {
namespace GameObjects {
namespace Components {

template <typename T>
class BlendMode
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

	BLEND_MODE getBlendMode ()
	{
		return blendMode;
	}

	T& setBlendMode (BLEND_MODE value)
	{
		blendMode = value;

		return *This();
	}

protected:
	BLEND_MODE blendMode = BLEND_MODE::NORMAL;
};

}	// namespace Components
}	// namespace GameObjects
}	// namespace Zen

#endif
