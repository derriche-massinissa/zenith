/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "ceil.h"
#include <cmath>

namespace Zen {
namespace Math {
namespace Fuzzy {

bool ceil (double value, double epsilon)
{
	return std::ceil(value - epsilon);
}

}	// namespace Fuzzy
}	// namespace Math
}	// namespace Zen
