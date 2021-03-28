/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_FOLLOW_HPP
#define ZEN_SYSTEMS_FOLLOW_HPP

#include "../ecs/entity.hpp"

namespace Zen {

void StartFollow (Entity entity, Entity target, double lerpX = 1., double lerpY = 1., double offsetX = 0., double offsetY = 0.);

void StopFollow (Entity entity);

void SetFollowOffset (Entity entity, double x = 0., double y = 0.);

void SetFollowLerp (Entity entity, double x = 1., double y = 1.);

}	// namespace Zen

#endif
