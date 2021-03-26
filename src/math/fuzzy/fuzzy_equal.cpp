/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "fuzzy_equal.hpp"

#include <cmath>

namespace Zen {
namespace Math {

bool FuzzyEqual (double a, double b, double epsilon)
{
	return std::abs(a - b) < epsilon;
}

}	// namespace Math
}	// namespace Zen
