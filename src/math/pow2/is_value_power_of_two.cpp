/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "is_value_power_of_two.hpp"

namespace Zen {
namespace Math {

bool IsValuePowerOfTwo (double value)
{
	int v = value;

	return (v > 0 && (v & (v - 1)) == 0);
}

}	// namespace Math
}	// namespace Zen
