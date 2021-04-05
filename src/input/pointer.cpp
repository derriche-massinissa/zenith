
#include "pointer.hpp"

#include <cmath>
#include "../math/interpolation/smooth_step_interpolation.hpp"
#include "../math/fuzzy/fuzzy_equal.hpp"
#include "../math/angle/between.hpp"
#include "../math/distance/distance_between.hpp"
#include "../cameras/2d/systems/camera.hpp"
#include "input_manager.hpp"

namespace Zen {

extern InputManager g_input;

Pointer::Pointer ()
{
	active = (id == 0) ? true : false;
}

void Pointer::updateWorldPoint (Entity camera_)
{
	auto temp = GetWorldPoint(camera_, position.x, position.y);

	worldX = temp.x;
	worldY = temp.y;
}

Math::Vector2 Pointer::positionToCamera (Entity camera_)
{
	return GetWorldPoint(camera_, position.x, position.y);
}

void Pointer::updateMotion ()
{
	double cx_ = position.x;
	double cy_ = position.y;

	double mx_ = midPoint.x;
	double my_ = midPoint.y;

	if (cx_ == mx_ && cy_ == my_)
		// Nothing to do here
		return;

	// Moving towards our goal...
	double vx_ = Math::SmoothStepInterpolation(motionFactor, mx_, cx_);
	double vy_ = Math::SmoothStepInterpolation(motionFactor, my_, cy_);

	if (Math::FuzzyEqual(vx_, cx_, 0.1))
	{
		vx_ = cx_;
	}

	if (Math::FuzzyEqual(vy_, cy_, 0.1))
	{
		vy_ = cy_;
	}

	midPoint.x = vx_;
	midPoint.y = vy_;

	double dx_ = cx_ - vx_;
	double dy_ = cy_ - vy_;

	velocity.x = dx_;
	velocity.y = dy_;

	angle = Math::AngleBetween(vx_, vy_, cx_, cy_);

	distance = std::sqrt(dx_ * dx_ + dy_ * dy_);
}

void Pointer::up (InputEvent event_)
{
	buttons = event_.buttons;

	event = event_;

	button = event_.button;

	// Sets the local x/y properties
	g_input.transformPointer(this, event.x, event.y, false);

	// 0: Main button pressed, usually the left button or the un-initialized state
	if (event_.button == 0)
	{
		primaryDown = false;
		upX = position.x;
		upY = position.y;
	}

	if (buttons == 0)
	{
		// No more buttons are still down
		isDown = false;

		upTime = event_.timestamp;

		wasTouch = false;
	}
}

void Pointer::down (InputEvent event_)
{
	buttons = event_.buttons;

	event = event_;

	button = event_.button;

	// Sets the local x/y properties
	g_input.transformPointer(this, event.x, event.y, false);

	// 0: Main button pressed, usually the left button or the un-initialized state
	if (event_.button == 0)
	{
		primaryDown = false;
		upX = position.x;
		upY = position.y;
	}

	if (!isDown)
	{
		isDown = true;

		downTime = event_.timestamp;
	}

	wasTouch = false;
}

void Pointer::move (InputEvent event_)
{
	buttons = event_.buttons;

	event = event_;

	// Sets the local x/y properties
	g_input.transformPointer(this, event.x, event.y, true);

	if (locked)
	{
		movementX = event_.deltaX;
		movementY = event_.deltaY;
	}

	moveTime = event_.timestamp;

	wasTouch = false;
}

void Pointer::wheel (InputEvent event_)
{
	buttons = event_.buttons;

	event = event_;

	// Sets the local x/y properties
	g_input.transformPointer(this, event.x, event.y, true);

	deltaX = event_.deltaX;
	deltaY = event_.deltaY;
	deltaZ = event_.deltaZ;

	wasTouch = false;
}

void Pointer::touchstart (InputEvent touch_, InputEvent event_)
{
	// TODO touch
}

void Pointer::touchmove (InputEvent touch_, InputEvent event_)
{
	// TODO touch
}

void Pointer::touchend (InputEvent touch_, InputEvent event_)
{
	// TODO touch
}

void Pointer::touchcancel (InputEvent touch_, InputEvent event_)
{
	// TODO touch
}

bool Pointer::noButtonDown ()
{
	return buttons == 0;
}

bool Pointer::leftButtonDown ()
{
	return (buttons & 0b00001) ? true : false;
}

bool Pointer::rightButtonDown ()
{
	return (buttons & 0b00010) ? true : false;
}

bool Pointer::middleButtonDown ()
{
	return (buttons & 0b00100) ? true : false;
}

bool Pointer::backButtonDown ()
{
	return (buttons & 0b01000) ? true : false;
}

bool Pointer::forwardButtonDown ()
{
	return (buttons & 0b10000) ? true : false;
}

bool Pointer::leftButtonReleased ()
{
	return (button == 0 && !isDown);
}

bool Pointer::rightButtonReleased ()
{
	return (button == 2 && !isDown);
}

bool Pointer::middleButtonReleased ()
{
	return (button == 1 && !isDown);
}

bool Pointer::backButtonReleased ()
{
	return (button == 3 && !isDown);
}

bool Pointer::forwardButtonReleased ()
{
	return (button == 4 && !isDown);
}

double Pointer::getDistance ()
{
	if (isDown)
	{
		return Math::DistanceBetween(downX, downY, position.x, position.y);
	}
	else
	{
		return Math::DistanceBetween(downX, downY, upX, upY);
	}
}

double Pointer::getDistanceX ()
{
	if (isDown)
	{
		return std::abs(downX - position.x);
	}
	else
	{
		return std::abs(downX - upX);
	}
}

double Pointer::getDistanceY ()
{
	if (isDown)
	{
		return std::abs(downY - position.y);
	}
	else
	{
		return std::abs(downY - upY);
	}
}

Uint32 Pointer::getDuration ()
{
	if (isDown)
	{
		return (g_input.time - downTime);
	}
	else
	{
		return (upTime - downTime);
	}
}

double Pointer::getAngle ()
{
	if (isDown)
	{
		return Math::AngleBetween(downX, downY, position.x, position.y);
	}
	else
	{
		return Math::AngleBetween(downX, downY, upX, upY);
	}
}

Uint32 Pointer::getTime ()
{
	return event.timestamp;
}

}	// namespace Zen
