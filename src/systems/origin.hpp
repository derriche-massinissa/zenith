/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_ORIGIN_HPP
#define ZEN_SYSTEMS_ORIGIN_HPP

#include "../ecs/entity.hpp"

namespace Zen {

void SetDisplayOriginX (Entity entity, double value);

void SetDisplayOriginY (Entity entity, double value);

void SetDisplayOrigin (Entity entity, double x, double y);

void SetDisplayOrigin (Entity entity, double value = 0);

void SetOrigin (Entity entity, double x, double y);

void SetOrigin (Entity entity, double value = 0.5);

void SetOriginFromFrame (Entity entity);

double GetOriginX (Entity entity);

double GetOriginY (Entity entity);

double GetDisplayOriginX (Entity entity);

double GetDisplayOriginY (Entity entity);

void UpdateDisplayOrigin (Entity entity);

}	// namespace Zen

#endif
