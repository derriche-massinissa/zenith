/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_SIZE_HPP
#define ZEN_SYSTEMS_SIZE_HPP

#include "../ecs/entity.hpp"

namespace Zen {

double GetDisplayWidth (Entity entity);

double GetDisplayHeight (Entity entity);

void SetDisplayWidth (Entity entity, double value);

void SetDisplayHeight (Entity entity, double value);

void SetSizeToFrame (Entity entity, Entity frame = entt::null);

void SetSize (Entity entity, double width, double height);

void SetSize (Entity entity, double value);

void SetDisplaySize (Entity entity, double width, double height);

void SetWidth (Entity entity, double value);

void SetHeight (Entity entity, double value);

double GetWidth (Entity entity);

double GetHeight (Entity entity);

}	// namespace Zen

#endif
