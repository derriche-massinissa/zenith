/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "wrap.hpp"

#include <cmath>

namespace Zen {
namespace Math {

double Wrap (double value, double min, double max)
{
    double range = max - min;

    return min + std::fmod(std::fmod(value - min, range) + range, range);
}

}	// namespace Math
}	// namespace Zen
