/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "wrap.h"

#include <cmath>
#include "../wrap.h"

namespace Zen {
namespace Math {
namespace Angle {

double wrap (double angle)
{
    return Math::wrap(angle, -M_PI, M_PI);
}

}	// namespace Angle
}	// namespace Math
}	// namespace Zen
