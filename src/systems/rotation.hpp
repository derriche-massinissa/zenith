/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_ROTATION_HPP
#define ZEN_SYSTEMS_ROTATION_HPP

#include "../ecs/entity.hpp"

namespace Zen {

void SetAngle (Entity entity, double value);

double GetAngle (Entity entity);

void SetRotation (Entity entity, double value = 0.0);

double GetRotation (Entity entity);

double GetParentRotation (Entity entity);

}	// namespace Zen

#endif
