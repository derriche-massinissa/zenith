/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "wrap_degrees.hpp"

#include "../wrap.hpp"

namespace Zen {
namespace Math {

double WrapDegrees (double angle)
{
    return Math::Wrap(angle, -180, 180);
}

}	// namespace Math
}	// namespace Zen
