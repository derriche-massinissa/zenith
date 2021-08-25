/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_ACTOR_HPP
#define ZEN_SYSTEMS_ACTOR_HPP

#include "../ecs/entity.hpp"
#include "../scene/scene.fwd.hpp"

namespace Zen {

void SetScene (Entity entity, Scene *scene);

Scene* GetScene (Entity entity);

}	// namespace Zen

#endif
