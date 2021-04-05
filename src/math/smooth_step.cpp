/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "smooth_step.hpp"

namespace Zen {
namespace Math {

double SmoothStep (double x, double min, double max)
{
	if (x <= min)
	{
		return 0.;
	}
	else if (x >= max)
	{
		return 1.;
	}

	x = (x - min) / (max - min);

	return x * x * (3. - 2. * x);
}

}	// namespace Math
}	// namespace Zen
