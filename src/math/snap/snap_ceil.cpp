/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "snap_ceil.h"
#include <cmath>

namespace Zen {
namespace Math {

double SnapCeil (double value, double gap, double start, bool divide)
{
	if (!gap)
		return value;

	value -= start;
	value = gap * std::ceil(value / gap);

	return (divide) ? (start + value) / gap : start + value;
}

}	// namespace Math
}	// namespace Zen
