/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "get_power_of_two.hpp"

#include <cmath>

namespace Zen {
namespace Math {

int GetPowerOfTwo (double value)
{
	value = std::log(value) / 0.6931471805599453;
	int index = std::ceil(value);

	return (1 << index);
}

}	// namespace Math
}	// namespace Zen
