/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_INPUT_HPP
#define ZEN_SYSTEMS_INPUT_HPP

#include "../ecs/entity.hpp"
#include "../components/input.hpp"

namespace Zen {

bool IsInputEnabled (Entity entity);

void MakeDropZone (Entity entity, bool value = true);

Components::Input* GetInput (Entity entity);

}	// namespace Zen

#endif
