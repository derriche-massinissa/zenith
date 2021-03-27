/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_POSITION_HPP
#define ZEN_SYSTEMS_POSITION_HPP

#include "../ecs/entity.hpp"
#include "../math/types/vector2.hpp"

namespace Zen {

void SetPosition (Entity entity, double x = 0., double y = 0., double z = 0., double w = 0.);

void SetPosition (Entity entity, Math::Vector2 source);

/**
 * TODO
 * @todo Vector3 & Vector4
 void SetPosition (Entity entity, Math::Vector3 source);

 void SetPosition (Entity, entity, Math::Vector4 source);
 */

void SetX (Entity entity, double value = 0.);

void SetY (Entity entity, double value = 0.);

void SetZ (Entity entity, double value = 0.);

void SetW (Entity entity, double value = 0.);

double GetX (Entity entity);

double GetY (Entity entity);

double GetZ (Entity entity);

double GetW (Entity entity);

}	// namespace Zen

#endif
