/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "less_than.h"

namespace Zen {
namespace Math {
namespace Fuzzy {

bool lessThan (double a, double b, double epsilon)
{
	return a < b + epsilon;
}

}	// namespace Fuzzy
}	// namespace Math
}	// namespace Zen
