/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_CONST_SCALEMODES_HPP
#define ZEN_CONST_SCALEMODES_HPP

namespace Zen {

/**
 * The available scale modes.
 *
 * @since 0.0.0
 */
enum class SCALE_MODE {
	RESIZE,
	WIDTH_CONTROLS_HEIGHT,
	HEIGHT_CONTROLS_WIDTH,
	FIT,
	ENVELOP
};

}	// namespace Zen

#endif
