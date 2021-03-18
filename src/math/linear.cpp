/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "linear.h"
#include <cmath>

namespace Zen {
namespace Math {

double linear (double p0, double p1, double t)
{
	return (p1 - p0) * t + p0;
}

}	// namespace Math
}	// namespace Zen
