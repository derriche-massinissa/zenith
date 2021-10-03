/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "is_size_power_of_two.hpp"

namespace Zen {
namespace Math {

bool IsSizePowerOfTwo (double width, double height)
{
	int w = width;
	int h = height;

	return (w > 0 && (w & (w - 1)) == 0 &&
			h > 0 && (h & (h - 1)) == 0);
}

}	// namespace Math
}	// namespace Zen
