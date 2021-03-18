/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "rad_to_deg.h"
#include "const.h"

namespace Zen {
namespace Math {

double radToDeg (double radians)
{
    return radians * RAD_TO_DEG;
}

}	// namespace Math
}	// namespace Zen
