/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_INPUT_INPUTMANAGER_HPP
#define ZEN_INPUT_INPUTMANAGER_HPP

#include "../event/event_emitter.hpp"
#include "pointer.fwd.hpp"
#include <SDL2/SDL_stdinc.h>
#include "../math/types/vector2.hpp"
#include "../components/transform_matrix.hpp"
#include "types/event.hpp"
#include "../core/config.fwd.hpp"
#include "const.hpp"

namespace Zen {

class InputManager
{
public:
	GameConfig& config;

	bool enabled = true;

	EventEmitter events;

	bool isOver = true;

	char defaultCursor[20] = "";

	std::vector<Pointer> pointers;

	Pointer* mousePointer;

	Pointer* activePointer;

	bool globalTopOnly = true;

	Uint32 time = 0;

	Math::Vector2 tempPoint;

	std::vector<Entity> tempHitTest;

	Components::TransformMatrix tempMatrix;

	Components::TransformMatrix tempMatrix2;

	bool tempSkip = false;

	InputManager (GameConfig& cfg_);

	~InputManager ();

	void setWindowOver (InputEvent event_);

	void setWindowOut (InputEvent event_);

	void preRender (Uint32 time_, Uint32 delta_);

	void setDefaultCursor ();

	void setCursor ();

	void resetCursor ();

	std::vector<Pointer*> addPointer (int quantity_ = 1);

	void updateInputPlugins (INPUT type_, std::vector<Pointer*> pointers_);

	void onTouchStart (InputEvent event_);

	void onTouchMove (InputEvent event_);

	void onTouchEnd (InputEvent event_);

	void onTouchCancel (InputEvent event_);

	void onMouseDown (InputEvent event_);

	void onMouseMove (InputEvent event_);

	void onMouseUp (InputEvent event_);

	void onMouseWheel (InputEvent event_);

	void onPointerLockChange (bool locked_);

	bool inputCandidate (Entity entity_, Entity camera_);

	std::vector<Entity> hitTest (Pointer* pointer_, std::vector<Entity> gameObject_, Entity camera_);

	bool pointWithinHitArea (Entity gameObject_, double x_, double y_);

	void transformPointer (Pointer* pointer_, double windowX_, double windowY_, bool wasMove_);
};

}	// namespace Zen

#endif
