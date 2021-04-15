/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_INPUT_TYPE_HITCALLBACK_HPP
#define ZEN_INPUT_TYPE_HITCALLBACK_HPP

#include "functional"
#include "../../geom/types/shape.hpp"
#include "../../ecs/entity.hpp"

namespace Zen {

using HitCallback = std::function<bool(Shape, double, double, Entity)>;

}	// namespace Zen

#endif
