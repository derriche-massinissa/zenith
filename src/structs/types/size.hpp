/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_STRUCTS_TYPES_SIZE_HPP
#define ZEN_STRUCTS_TYPES_SIZE_HPP

#include "../../scale/scale_modes.hpp"

namespace Zen {

struct Size
{
	double width = 0.,
		   height = 0.,
		   aspectRatio = 1.,
		   minWidth = 0.,
		   minHeight = 0.,
		   maxWidth = 0.,
		   maxHeight = 0.;

	SCALE_MODE aspectMode = SCALE_MODE::RESIZE;

	/**
	 * Dimension snapping, for x (index 0) and y (index 1).
	 *
	 * @since 0.0.0
	 */
	double snapTo[2] {0., 0.};
};

}	// namespace Zen

#endif
