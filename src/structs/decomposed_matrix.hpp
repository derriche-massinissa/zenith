/**
 * @file
 * @author		__AUTHOR_NAME_ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD_
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_STRUCTS_DECOMPOSEDMATRIX_HPP
#define ZEN_STRUCTS_DECOMPOSEDMATRIX_HPP

namespace Zen {

/**
 * A structure containing the transformations of a composed transform matrix.
 *
 * @struct DecomposedMatrix
 * @since 0.0.0
 */
struct DecomposedMatrix
{
	double translateX = 0.,
		   translateY = 0.,
		   scaleX = 1.,
		   scaleY = 1.,
		   rotation = 0.;
};

}

#endif
