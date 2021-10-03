/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_ENUMS_BLENDMODES_HPP
#define ZEN_ENUMS_BLENDMODES_HPP

namespace Zen {

enum class BLEND_MODE {
	SKIP_CHECK = -1,
	NORMAL = 0,
	BLEND,
	ADD,
	MULTIPLY,
	SCREEN,
	SUBTRACT,
	ERASE,
	SOURCE_OVER,
	SOURCE_IN,
	SOURCE_OUT,
	SOURCE_ATOP,
	DESTINATION_OVER,
	DESTINATION_IN,
	DESTINATION_OUT,
	DESTINATION_ATOP,
	LIGHTER,
	COPY,
	XOR
};

}	// namespace Zen

#endif
