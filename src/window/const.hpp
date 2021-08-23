/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_WINDOW_CONST_HPP
#define ZEN_WINDOW_CONST_HPP

namespace Zen {

/**
 * Window states.
 *
 * @since 0.0.0
 */
enum class WINDOW_STATE {
	HIDDEN,
	VISIBLE,
	BLURRED,
	FOCUSED
};

/**
 * The elements to cleanup once the window closed.
 *
 * @since 0.0.0
 */
enum class WINDOW_CLEANUP {
	SDL,
	IMG
};

}	// namespace Zen

#endif
