/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_VIEWPORT_HPP
#define ZEN_SYSTEMS_VIEWPORT_HPP

#include "../ecs/entity.hpp"

namespace Zen {

void SetViewport (Entity entity, double x, double y, double width, double height = 0.);

void RemoveViewport (Entity entity);

bool GetViewport (Entity entity);

}	// namespace Zen

#endif
