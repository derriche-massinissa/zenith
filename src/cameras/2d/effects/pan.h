/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_CAMERAS_SCENE2D_EFFECTS_PAN_H
#define ZEN_CAMERAS_SCENE2D_EFFECTS_PAN_H

#include <functional>
#include <string>

#include "../camera.fwd.h"

namespace Zen {
namespace Cameras {
namespace Scene2D {
namespace Effects {

class Pan
{
public:
	Pan (Camera& camera_)
	{}

	Camera& start (int x_, int y_, int duration_, std::string ease_, bool force_, std::function<void(Camera, double, int, int)> callback_ = nullptr)
	{}

	bool isRunning = false;

	void reset ()
	{}

	void update (Uint32 time_, Uint32 delta_)
	{}
};

}	// namespace Effects
}	// namespace Scene2D
}	// namespace Cameras
}	// namespace Zen

#endif
