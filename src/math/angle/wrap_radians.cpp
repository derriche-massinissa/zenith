/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "wrap.h"

#include <cmath>
#include "wrap.h"

namespace Zen {
namespace Math {

double WrapRadians (double radians)
{
    return Math::Wrap(angle, -M_PI, M_PI);
}

}	// namespace Math
}	// namespace Zen
