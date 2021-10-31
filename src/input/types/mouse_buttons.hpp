/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_INPUT_MOUSE_BUTTONS_HPP
#define ZEN_INPUT_MOUSE_BUTTONS_HPP

namespace Zen {

namespace MOUSE_BUTTONS {
	const int MASK = 0b11111;

	enum : int {
		MASK_LEFT		= 0b00001,
		MASK_RIGHT		= 0b00010,
		MASK_MIDDLE		= 0b00100,
		MASK_BACK		= 0b01000,
		MASK_FORWARD	= 0b10000
	};

	enum : int {
		MASK_NOT_LEFT		= ~MASK_LEFT		& MASK,
		MASK_NOT_RIGHT		= ~MASK_RIGHT		& MASK,
		MASK_NOT_MIDDLE		= ~MASK_MIDDLE		& MASK,
		MASK_NOT_BACK		= ~MASK_BACK		& MASK,
		MASK_NOT_FORWARD	= ~MASK_FORWARD		& MASK
	};

	enum : int {
		LEFT		= 0,
		RIGHT		= 1,
		MIDDLE		= 2,
		BACK		= 3,
		FORWARD		= 4
	};
}

}	// namespace Zen

#endif
