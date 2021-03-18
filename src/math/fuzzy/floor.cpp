/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "floor.h"
#include <cmath>

namespace Zen {
namespace Math {
namespace Fuzzy {

bool floor (double value, double epsilon)
{
	return std::floor(value + epsilon);
}

}	// namespace Fuzzy
}	// namespace Math
}	// namespace Zen
