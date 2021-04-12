/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "key.hpp"
#include <SDL2/SDL_timer.h>
#include "../../../math/snap/snap_floor.hpp"

namespace Zen {

Key::Key (SDL_Keycode keyCode, bool emitOnRepeat)
	: keyCode (keyCode), emitOnRepeat (emitOnRepeat)
{}

void Key::onDown (const SDL_Event * const event)
{
	if (!enabled)
		return;

	keyMod = static_cast<SDL_Keymod>(event->key.keysym.mod);

	repeats++;

	if (!isDown)
	{
		isDown = true;
		isUp = false;
		timeDown = event->key.timestamp;
		duration = 0;
		_justDown = true;
		_justUp = false;

		emit("down", this, event);
	}
	else if (emitOnRepeat)
	{
		emit("down", this, event);
	}
}

void Key::onUp (const SDL_Event * const event)
{
	if (!enabled)
		return;

	isDown = false;
	isUp = true;
	timeUp = event->key.timestamp;
	duration = timeUp - timeDown;
	repeats = 0;

	_justDown = false;
	_justUp = true;
	_tick = -1;

	emit("up", this, event);
}

void Key::reset ()
{
	enabled = true;
	isDown = false;
	isUp = true;
	keyMod = KMOD_NONE;
	timeDown = 0;
	duration = 0;
	timeUp = 0;
	repeats = 0;
	_justDown = false;
	_justUp = false;
	_tick = -1;
}

Uint32 Key::getDuration ()
{
	if (isDown)
		return (SDL_GetTicks() - timeDown);
	else
		return 0;
}

bool Key::downDuration (Uint32 duration)
{
	Uint32 current = SDL_GetTicks() - timeDown;

	return (isDown && current < duration);
}

bool Key::upDuration (Uint32 duration)
{
	Uint32 current = SDL_GetTicks() - timeUp;

	return (isUp && current < duration);
}

bool Key::justDown ()
{
	if (_justDown)
	{
		_justDown = false;

		return true;
	}
	else
	{
		return false;
	}
}

bool Key::justUp ()
{
	if (_justUp)
	{
		_justUp = false;

		return true;
	}
	else
	{
		return false;
	}
}

bool Key::checkDown (Uint32 duration)
{
	if (isDown)
	{
		double t = Math::SnapFloor(SDL_GetTicks() - timeDown, duration);

		if (t > _tick)
		{
			_tick = t;

			return true;
		}
	}

	return false;
}

bool Key::altKey ()
{
	return (keyMod & KMOD_ALT) != KMOD_NONE;
}

bool Key::lAltKey ()
{
	return (keyMod & KMOD_LALT) != KMOD_NONE;
}

bool Key::rAltKey ()
{
	return (keyMod & KMOD_RALT) != KMOD_NONE;
}

bool Key::ctrlKey ()
{
	return (keyMod & KMOD_CTRL) != KMOD_NONE;
}

bool Key::lCtrlKey ()
{
	return (keyMod & KMOD_LCTRL) != KMOD_NONE;
}

bool Key::rCtrlKey ()
{
	return (keyMod & KMOD_RCTRL) != KMOD_NONE;
}

bool Key::shiftKey ()
{
	return (keyMod & KMOD_SHIFT) != KMOD_NONE;
}

bool Key::lShiftKey ()
{
	return (keyMod & KMOD_LSHIFT) != KMOD_NONE;
}

bool Key::rShiftKey ()
{
	return (keyMod & KMOD_RSHIFT) != KMOD_NONE;
}

bool Key::metaKey ()
{
	return (keyMod & KMOD_GUI) != KMOD_NONE;
}

bool Key::lMetaKey ()
{
	return (keyMod & KMOD_LGUI) != KMOD_NONE;
}

bool Key::rMetaKey ()
{
	return (keyMod & KMOD_RGUI) != KMOD_NONE;
}

bool Key::operator == (const Key& rhs)
{
	return (keyCode == rhs.keyCode);
}

}	// namespace Zen
