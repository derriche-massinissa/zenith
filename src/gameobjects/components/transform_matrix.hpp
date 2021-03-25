/**
 * @file
 * @author		__AUTHOR_NAME_ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD_
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_TRANSFORMMATRIX_HPP
#define ZEN_GAMEOBJECTS_TRANSFORMMATRIX_HPP

namespace Zen {

/**
 * A Matrix used for display transformations for rendering.
 *
 * Its components are as follow:
 * - a: Scale X
 * - b: Skew Y
 * - c: Skew X
 * - d: Scale Y
 * - e: Translate X
 * - f: Translate Y
 *
 * It is represented like so:
 * ```
 * | a | c | e |
 * | b | d | f |
 * | 0 | 0 | 1 |
 * ```
 * ... or:
 * ```
 * | a | c | tx |
 * | b | d | ty |
 * | 0 | 0 | 1  |
 * ```
 *
 * @struct TransformMatrix
 * @since 0.0.0
 */
struct TransformMatrix
{
	double a = 1.,
		   b = 0.,
		   c = 0.,
		   d = 1.,
		   e = 0.,
		   f = 0.;
};

}

#endif
