/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "key_combo.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include "../../../utils/messages.hpp"
#include "../../../utils/assert.hpp"
#include "../keyboard_plugin.hpp"

namespace Zen {

KeyCombo::KeyCombo (const char * keys_, KeyComboConfig config_)
{
	std::string sKeys_ (keys_);

	ZEN_ASSERT((sKeys_.size() > 2), "A combo cannot have a zero or single length \
			combo");
	
	std::string temp_;

	// Keys
	for (const char& key_ : sKeys_)
	{
		temp_ = key_;
		SDL_Keycode k_ = SDL_GetKeyFromName(temp_.c_str());

		if (k_ == SDLK_UNKNOWN)
		{
			MessageError("Key '", k_, "' is not recognized, and couldn't be \
					used for combo creation"); }
		else
		{
			keyCodes.emplace_back(k_);
		}
	}

	// Config
	resetOnWrongKey = config_.resetOnWrongKey;
	maxKeyDelay = config_.maxKeyDelay;
	resetOnMatch = config_.resetOnMatch;
	deleteOnMatch = config_.deleteOnMatch;

	size = keyCodes.size();
}

KeyCombo::KeyCombo (std::vector<const char*> keys, KeyComboConfig config)
{
	ZEN_ASSERT((keys.size() > 2), "A combo cannot have a zero or single length \
			combo");
	
	// Keys
	for (const auto& key : keys)
	{
		SDL_Keycode k = SDL_GetKeyFromName(key);

		if (k == SDLK_UNKNOWN)
		{
			MessageError("Key '", k, "' is not recognized, and couldn't be used \
					for combo creation");
		}
		else
		{
			keyCodes.emplace_back(k);
		}
	}

	// Config
	resetOnWrongKey = config.resetOnWrongKey;
	maxKeyDelay = config.maxKeyDelay;
	resetOnMatch = config.resetOnMatch;
	deleteOnMatch = config.deleteOnMatch;

	size = keyCodes.size();
}

KeyCombo::KeyCombo (std::vector<SDL_Keycode> keys, KeyComboConfig config)
	: keyCodes (keys)
{
	ZEN_ASSERT((keys.size() > 2), "A combo cannot have a zero or single length \
			combo");
	
	// Keys
	keyCodes = keys;

	// Config
	resetOnWrongKey = config.resetOnWrongKey;
	maxKeyDelay = config.maxKeyDelay;
	resetOnMatch = config.resetOnMatch;
	deleteOnMatch = config.deleteOnMatch;

	size = keyCodes.size();
}

double KeyCombo::getProgress ()
{
	return ( static_cast<double>(index) / static_cast<double>(size) );
}

bool KeyCombo::onKeyDown (KeyboardPlugin* kbPlugin_, const SDL_Event * const event_)
{
	if (matched || !enabled)
		return false;

	matched = process(event_);

	if (matched)
	{
		emit("match", event_);
		kbPlugin_->emit("keycombomatch", this, event_);

		if (resetOnMatch)
			reset();
		else if (deleteOnMatch)
			return true;
	}

	return false;
}

bool KeyCombo::process (const SDL_Event * const event_)
{
	if (matched)
		return true;

	bool comboMatched = false;
	bool keyMatched = false;

	if (event_->key.keysym.sym == keyCodes[index])
	{
		// Key was correct
		if (index > 0 && maxKeyDelay > 0)
		{
			// We have to check to see if the delay between the new key and the old
			// one was too long (if enabled)
			Uint32 timeLimit = timeLastMatched + maxKeyDelay;

			// Check if they pressed it in time or not
			if (event_->key.timestamp <= timeLimit)
			{
				keyMatched = true;
				comboMatched = advance(event_);
			}
		}
		else
		{
			keyMatched = true;

			// We don't check the time for the first key pressed, so just advance it
			comboMatched = advance(event_);
		}
	}

	if (!keyMatched && resetOnWrongKey)
	{
		// Wrong key was pressed
		index = 0;
	}

	if (comboMatched)
	{
		timeLastMatched = event_->key.timestamp;
		matched = true;
		timeMatched = event_->key.timestamp;
	}

	return comboMatched;
}

bool KeyCombo::advance (const SDL_Event * const event_)
{
	timeLastMatched = event_->key.timestamp;
	index++;

	if (index == size)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void KeyCombo::reset ()
{
	index = 0;
	timeLastMatched = 0;
	matched = false;
	timeMatched = 0;
}

}	// namespace Zen
