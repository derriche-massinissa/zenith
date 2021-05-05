/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_EASING_ELASTIC_IN
#define ZEN_MATH_EASING_ELASTIC_IN

namespace Zen {
namespace Math {
namespace Easing {
namespace Elastic {

/**
 * Elastic ease-in.
 *
 * @since 0.0.0
 *
 * @param value The value to be eased.
 * @param amplitude The amplitude of the elastic ease.
 * @param period Sets how tight the sine-wave is, where smaller values are tighter
 * waves, which result in more cycles.
 *
 * @return The eased value.
 */
double In (double value, double amplitude = 0.1, double period = 0.1);

}	// namespace Elastic
}	// namespace Easing
}	// namespace Math
}	// namespace Zen

#endif
