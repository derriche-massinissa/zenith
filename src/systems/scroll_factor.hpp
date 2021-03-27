/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_SCROLLFACTOR_HPP
#define ZEN_SYSTEMS_SCROLLFACTOR_HPP

#include "../ecs/entity.hpp"

namespace Zen {

void SetScrollFactor (Entity entity, double x, double y);

void SetScrollFactor (Entity entity, double value = 1.0);

double GetScrollFactorX (Entity entity);

double GetScrollFactorY (Entity entity);

}	// namespace Zen

#endif
