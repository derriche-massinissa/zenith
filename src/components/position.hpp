/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_COMPONENTS_POSITION_HPP
#define ZEN_COMPONENTS_POSITION_HPP

namespace Zen {
namespace Components {

struct Position
{
	double x = 0.,
		   y = 0.,
		   z = 0.,
		   w = 0.;

	bool roundPixels = false;
};

}	// namespace Components
}	// namespace Zen

#endif
