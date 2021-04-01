/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_INPUT_POINTER_HPP
#define ZEN_INPUT_POINTER_HPP

#include "../ecs/entity.hpp"

#include <SDL2/SDL_types.h>
#include "../math/vector2.hpp"

namespace Zen {

class Pointer
{
public:
	int id = -1;

	Entity camera;

	int button = 0;

	/**
	 * 0b 00000: No button or un-initialized
	 * 0b 00001: Left button
	 * 0b 00010: Right button
	 * 0b 00100: Wheel button or middle button
	 * 0b 01000: 4th button (typically the "Browser Back" button)
	 * 0b 10000: 5th button (typically the "Browser Forward" button)
	 *
	 * For a mouse configured for left-handed use, the button actions are reversed.
	 * In this case, the values are read from right to left.
	 *
	 * @since 0.0.0
	 */
	int buttons = 0;
	

	Math::Vector2 position;

	/**
	 * The x position of this Pointer, translated into the coordinate space of the most recent Camera it interacted with.
	 *
	 * If you wish to use this value _outside_ of an input event handler then you should update it first by calling
	 * the Pointer::updateWorldPoint method.
	 *
	 * @since 0.0.0
	 */
	double worldX = 0.;

	double worldY = 0.;

	double downX = 0.;

	double downY = 0.;

	Uint32 downTime = 0;

	double upX = 0.;

	double upY = 0.;

	Uint32 upTime = 0;

	bool primaryDown = false;
};

}	// namespace Zen

#endif
