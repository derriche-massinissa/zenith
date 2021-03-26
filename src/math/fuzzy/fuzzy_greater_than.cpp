/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "fuzzy_greater_than.hpp"

namespace Zen {
namespace Math {

bool FuzzyGreaterThan (double a, double b, double epsilon)
{
	return a > b - epsilon;
}

}	// namespace Math
}	// namespace Zen
