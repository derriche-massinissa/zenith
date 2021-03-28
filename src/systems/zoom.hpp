/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_ZOOM_HPP
#define ZEN_SYSTEMS_ZOOM_HPP

#include "../ecs/entity.hpp"

namespace Zen {

void SetZoomX (Entity entity, double value = 0.);

void SetZoomY (Entity entity, double value = 0.);

void SetZoom (Entity entity, double x = 0., double y = 0.);

double GetZoomX (Entity entity);

double GetZoomY (Entity entity);

double GetZoom (Entity entity);

}	// namespace Zen

#endif
