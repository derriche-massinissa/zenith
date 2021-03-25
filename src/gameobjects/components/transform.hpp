/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_COMPONENTS_TRANSFORM_HPP
#define ZEN_GAMEOBJECTS_COMPONENTS_TRANSFORM_HPP

namespace Zen {

struct TransformComponent
{
	double rotation = 0.,
		   x = 0.,
		   y = 0.,
		   z = 0.,
		   w = 0.,
		   scaleX = 1.,
		   scaleY = 1.;
};

}	// namespace Zen

#endif
