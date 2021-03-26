/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "snap_to.hpp"

#include <cmath>

namespace Zen {
namespace Math {

double SnapTo (double value, double gap, double start, bool divide)
{
	if (!gap)
		return value;

	value -= start;
	value = gap * std::round(value / gap);

	return (divide) ? (start + value) / gap : start + value;
}

}	// namespace Math
}	// namespace Zen
