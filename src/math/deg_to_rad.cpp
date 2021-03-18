/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "deg_to_rad.h"
#include "../const.h"

namespace Zen {
namespace Math {

double degToRad (double degrees)
{
    return degrees * DEG_TO_RAD;
}

}	// namespace Math
}	// namespace Zen
