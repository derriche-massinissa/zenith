/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_TINT_HPP
#define ZEN_SYSTEMS_TINT_HPP

#include "../ecs/entity.hpp"
#include "../display/types/color.hpp"

namespace Zen {

void ClearTint (Entity entity);

void SetTint (
		Entity entity,
		int topLeft = 0xffffff,
		int topRight = -1,
		int bottomLeft = -1,
		int bottomRight = -1);

void SetTintFill (
		Entity entity,
		int topLeft = 0xffffff,
		int topRight = -1,
		int bottomLeft = -1,
		int bottomRight = -1);

Color GetTint (Entity);

bool IsTinted (Entity entity);

}	// namespace Zen

#endif
