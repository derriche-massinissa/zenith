/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "in.hpp"

namespace Zen {
namespace Math {
namespace Easing {
namespace Quartic {

double In (double value)
{
	return value * value * value * value;
}

}	// namespace Quartic
}	// namespace Easing
}	// namespace Math
}	// namespace Zen
