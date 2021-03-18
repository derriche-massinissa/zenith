/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_CAMERAS_SCENE2D_EFFECTS_ZOOM_H
#define ZEN_CAMERAS_SCENE2D_EFFECTS_ZOOM_H

#include <functional>
#include <string>

#include "../camera.fwd.h"

namespace Zen {
namespace Cameras {
namespace Scene2D {
namespace Effects {

class Zoom
{
public:
	Zoom (Camera& camera_)
	{}

	Camera& start (double zoom_, int duration_, std::string ease_, bool force_, std::function<void(Camera, double, double)> callback_ = nullptr)
	{}

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
