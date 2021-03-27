/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_SCALE_HPP
#define ZEN_SYSTEMS_SCALE_HPP

#include "../ecs/entity.hpp"

namespace Zen {

void SetScale (Entity entity, double value);

double GetScale (Entity entity);

void SetScaleX (Entity entity, double value);

double GetScaleX (Entity entity);

void SetScaleY (Entity entity, double value);

double GetScaleY (Entity entity);

}	// namespace Zen

#endif
