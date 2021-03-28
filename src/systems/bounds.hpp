/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_BOUNDS_HPP
#define ZEN_SYSTEMS_BOUNDS_HPP

#include "../ecs/entity.hpp"
#include "../geom/types/rectangle.hpp"

namespace Zen {

void SetBounds (Entity entity, double x, double y, double width, double height);

void RemoveBounds (Entity entity);

Rectangle GetBounds (Entity entity);

double ClampX (Entity entity, double x);

double ClampY (Entity entity, double y);

}	// namespace Zen

#endif
