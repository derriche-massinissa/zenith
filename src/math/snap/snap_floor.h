/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_SNAP_FLOOR_H
#define ZEN_MATH_SNAP_FLOOR_H

namespace Zen {
namespace Math {
namespace Snap {

/**
 * Snap a value to nearest grid slice, using floor.
 *
 * Example: if you have an interval gap of `5` and a position of `12`... you will snap to `10`.
 * As will `14` snap to `10`... but `16` will snap to `15`.
 *
 * @since 0.0.0
 *
 * @param value The value to snap.
 * @param gap The interval gap of the grid.
 * @param start Optional starting offset for gap.
 * @param divide If `true` it will divide the snapped value by the gap before returning.
 *
 * @return The snapped value.
 */
double floor (double value, double gap, double start = 0, bool divide = false);

}	// namespace Snap
}	// namespace Math
}	// namespace Zen

#endif
