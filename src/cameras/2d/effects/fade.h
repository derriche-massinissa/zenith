/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_CAMERAS_SCENE2D_EFFECTS_FADE_H
#define ZEN_CAMERAS_SCENE2D_EFFECTS_FADE_H

#include <functional>
#include <SDL2/SDL_types.h>

#include "../camera.fwd.h"

namespace Zen {
namespace Cameras {
namespace Scene2D {
namespace Effects {

class Fade
{
public:
	Fade (Camera& camera_)
	{}

	Camera& start (bool direction_, int duration_, int red_, int green_, int blue_, bool force_, std::function<void(Camera, double)> callback_ = nullptr)
	{}

	void reset ()
	{}

	void update (Uint32 time_, Uint32 delta_)
	{}

	void postRender()
	{}
};

}	// namespace Effects
}	// namespace Scene2D
}	// namespace Cameras
}	// namespace Zen

#endif
