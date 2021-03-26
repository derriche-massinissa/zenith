/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "clamp.hpp"

#include <cmath>

namespace Zen {
namespace Math {

double Clamp (double value, double min, double max)
{
	return std::max(min, std::min(max, value));
}

}	// namespace Math
}	// namespace Zen
