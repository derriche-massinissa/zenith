/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "smooth_step_interpolation.hpp"

#include "../smooth_step.hpp"

namespace Zen {
namespace Math {

double SmoothStepInterpolation (double t, double min, double max)
{
	return min + (max - min) * SmoothStep(t, 0., 1.);
}

}	// namespace Math
}	// namespace Zen
