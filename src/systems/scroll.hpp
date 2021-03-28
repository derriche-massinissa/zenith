/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_SCROLL_HPP
#define ZEN_SYSTEMS_SCROLL_HPP

#include "../ecs/entity.hpp"
#include "../math/types/vector2.hpp"

namespace Zen {

void SetScrollX (Entity entity, double value);

void SetScrollY (Entity entity, double value);

void SetScroll (Entity entity, double x, double y);

void SetScroll (Entity entity, double value = 0.);

double GetScrollX (Entity entity);

double GetScrollY (Entity entity);

void CenterOnX (Entity entity, double x);

void CenterOnY (Entity entity, double y);

void CenterOn (Entity entity, double x, double y);

Math::Vector2 GetScroll (Entity entity, double x, double y);

void CenterToBounds(Entity entity);

void CenterToSize (Entity entity);

}	// namespace Zen

#endif
