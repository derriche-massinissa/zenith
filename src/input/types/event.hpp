/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_INPUT_TYPES_EVENT_HPP
#define ZEN_INPUT_TYPES_EVENT_HPP

#include <vector>
#include <SDL2/SDL_types.h>
#include "../pointer.fwd.hpp"
#include "../../ecs/entity.hpp"

namespace Zen {

struct InputEvent {
	Pointer *pointer = nullptr;

	Entity gameObject = entt::null;

	std::vector<Entity> gameObjectList {};

	Entity target = entt::null;

	bool stopFlag = false;

	Uint32 timestamp = 0;

	double x = 0.;
	double y = 0.;
	double z = 0.;

	void stopPropagation ()
	{
		stopFlag = true;
	}
};

}	// namespace Zen

#endif
