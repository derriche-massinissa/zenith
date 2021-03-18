/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "snap_floor.h"
#include <cmath>

namespace Zen {
namespace Math {
namespace Snap {

double floor (double value, double gap, double start, bool divide)
{
	if (!gap)
		return value;

	value -= start;
	value = gap * std::floor(value / gap);

	return (divide) ? (start + value) / gap : start + value;
}

}	// namespace Snap
}	// namespace Math
}	// namespace Zen
