/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "keyboard_plugin.hpp"
#include <SDL2/SDL_keyboard.h>
#include <string>
#include "../../scene/scene.h"
#include "../input_manager.hpp"
#include "../../window/window.hpp"

namespace Zen {

extern InputManager g_input;
extern Window g_window;

KeyboardPlugin::KeyboardPlugin (Scene* scene)
	: scene(scene)
{
	scene->input.pluginEvents.on("start", &KeyboardPlugin::start, this);
}

KeyboardPlugin::~KeyboardPlugin ()
{
	shutdown();
}

void KeyboardPlugin::start ()
{
	scene->input.pluginEvents.once("shutdown", &KeyboardPlugin::shutdown, this);

	lKeyDown = g_input.events.on("keydown", &KeyboardPlugin::processKeyDown, this);
	lKeyUp = g_input.events.on("keyup", &KeyboardPlugin::processKeyUp, this);

	lBlur = g_window.on("blur", &KeyboardPlugin::resetKeys, this);

	lPause = scene->sys.events.on("pause", &KeyboardPlugin::resetKeys, this);
	lSleep = scene->sys.events.on("sleep", &KeyboardPlugin::resetKeys, this);
}

void KeyboardPlugin::shutdown ()
{
	resetKeys();

	g_input.events.off(lKeyDown);
	g_input.events.off(lKeyUp);

	g_window.off(lBlur);

	scene->sys.events.off(lPause);
	scene->sys.events.off(lSleep);

	removeAllListeners();
}

bool KeyboardPlugin::isActive ()
{
	return (enabled && scene->sys.isActive());
}

CursorKeys KeyboardPlugin::createCursorKeys ()
{
	CursorKeys cursorKeys;

	cursorKeys.up = addKey(SDLK_UP);
	cursorKeys.down = addKey(SDLK_DOWN);
	cursorKeys.left = addKey(SDLK_LEFT);
	cursorKeys.right = addKey(SDLK_RIGHT);
	cursorKeys.space = addKey(SDLK_SPACE);
	cursorKeys.lshift = addKey(SDLK_LSHIFT);
	cursorKeys.rshift = addKey(SDLK_RSHIFT);

	return cursorKeys;
}

Key* KeyboardPlugin::addKey (SDL_Keycode key_, bool emitOnRepeat_)
{
	// Check if the key was already added to this plugin and return it instead
	for (Key& k_ : keys)
	{
		if (k_.keyCode == key_)
			return &k_;
	}

	keys.emplace_back(key_, emitOnRepeat_);

	return &keys.back();
}

Key* KeyboardPlugin::addKey (std::string key_, bool emitOnRepeat_)
{
	return addKey( SDL_GetKeyFromName(key_.c_str()), emitOnRepeat_ );
}

Key* KeyboardPlugin::addKey (char key_, bool emitOnRepeat_)
{
	std::string sKey (1, key_);
	return addKey( SDL_GetKeyFromName(sKey.c_str()), emitOnRepeat_ );
}

void KeyboardPlugin::removeKey (Key* key_)
{
	for (size_t i_ = 0; i_ < keys.size(); i_++)
	{
		if (key_ == &keys[i_])
		{
			keys.erase(keys.begin() + i_);
			return;
		}
	}
}

void KeyboardPlugin::removeKey (SDL_Keycode key_)
{
	for (size_t i_ = 0; i_ < keys.size(); i_++)
	{
		if (key_ == keys[i_].keyCode)
		{
			keys.erase(keys.begin() + i_);
			return;
		}
	}
}

void KeyboardPlugin::removeKey (std::string key_)
{
	removeKey( SDL_GetKeyFromName(key_.c_str()) );
}

void KeyboardPlugin::removeKey (char key_)
{
	removeKey( SDL_GetKeyFromName(&key_) );
}

void KeyboardPlugin::removeAllKeys ()
{
	keys.clear();
}

KeyCombo* KeyboardPlugin::createCombo (
	std::string keys_,
	KeyComboConfig config_)
{
	combos.emplace_back(
		std::make_unique<KeyCombo>(keys_, config_)
	);

	return combos.back().get();
}

KeyCombo* KeyboardPlugin::createCombo (
	std::vector<std::string> keys_,
	KeyComboConfig config_)
{
	combos.emplace_back(
		std::make_unique<KeyCombo>(keys_, config_)
	);

	return combos.back().get();
}

KeyCombo* KeyboardPlugin::createCombo (
	std::vector<SDL_Keycode> keys_,
	KeyComboConfig config_)
{
	combos.emplace_back(
		std::make_unique<KeyCombo>(keys_, config_)
	);

	return combos.back().get();
}

void KeyboardPlugin::processKeyDown (const SDL_Event * const event_)
{
	if (!isActive())
		return;

	// Key specific callback first
	Key * key_ = getKeyFromKeycode(event_->key.keysym.sym);

	if (key_ != nullptr)
	{
		key_->onDown(event_);
	}

	// Combos
	for (auto c_ = combos.begin(); c_ != combos.end();)
	{
		if (c_->get()->onKeyDown(this, event_))
		{
			combos.erase(c_);
		}
		else
			c_++;
	}

	// Only emit when first down, not on repeat
	if (event_->key.repeat == 0)
	{
		emit(std::string("keydown-") + SDL_GetKeyName(event_->key.keysym.sym), event_);

		emit("keydown", event_);
	}
}

void KeyboardPlugin::processKeyUp (const SDL_Event * const event_)
{
	if (!isActive())
		return;

	// Key specific callback first
	Key * key_ = getKeyFromKeycode(event_->key.keysym.sym);

	if (key_ != nullptr)
	{
		key_->onUp(event_);
	}

	emit(std::string("keyup-") + SDL_GetKeyName(event_->key.keysym.sym), event_);

	emit("keyup", event_);
}

Key* KeyboardPlugin::getKeyFromKeycode (SDL_Keycode keycode_)
{
	for (Key& key_ : keys)
	{
		if (key_.keyCode == keycode_)
			return &key_;
	}

	return nullptr;
}

void KeyboardPlugin::resetKeys ()
{
	for (auto& key_ : keys)
	{
		key_.reset();
	}
}

}	// namespace Zen
