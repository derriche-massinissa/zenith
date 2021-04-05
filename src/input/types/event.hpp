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

class InputEvent {
public:
	Pointer *pointer = nullptr;

	Entity gameObject = entt::null;

	std::vector<Entity> gameObjectList {};

	Entity target = entt::null;

	bool stopFlag = false;

	Uint32 timestamp = 0;

	double x = 0.;
	double y = 0.;
	double z = 0.;

	double deltaX = 0.;
	double deltaY = 0.;
	double deltaZ = 0.;

	int button = 0;

	int buttons = 0;

	int clicks = 0;

	void stopPropagation ()
	{
		stopFlag = true;
	}

	void reset ()
	{
		pointer = nullptr;

		gameObject = entt::null;

		gameObjectList.clear();

		target = entt::null;

		stopFlag = false;

		timestamp = 0;

		x = 0.;
		y = 0.;
		z = 0.;
		
		deltaX = 0.;
		deltaY = 0.;
		deltaZ = 0.;

		button = 0;

		buttons = 0;

		clicks = 0;
	}
};

}	// namespace Zen

#endif
