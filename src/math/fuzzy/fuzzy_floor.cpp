/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "fuzzy_floor.hpp"

#include <cmath>

namespace Zen {
namespace Math {

bool FuzzyFloor (double value, double epsilon)
{
	return std::floor(value + epsilon);
}

}	// namespace Math
}	// namespace Zen
