/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_INPUT_KEYBOARD_PLUGIN_HPP
#define ZEN_INPUT_KEYBOARD_PLUGIN_HPP

#include "../../event/event_emitter.hpp"
#include <SDL2/SDL_keycode.h>

namespace Zen {

struct CursorKeys;
struct Key;
struct KeyCombo;
struct KeyComboConfig;

class KeyboardPlugin : public EventEmitter
{
public:
	KeyboardPlugin ();

	void boot ();

	bool isActive ();

	CursorKeys createCursorKeys ();

	std::vector<Key*> addKeys (std::vector<SDL_Keycode> keys_, bool emitOnRepeat_);

	Key* addKey (std::vector<SDL_Keycode> keys_, bool emitOnRepeat_);

	void removeKey (Key* key_);

	void removeAllKeys ();

	KeyCombo createCombo (std::vector<SDL_Keycode> keys_, KeyComboConfig config_);

	bool checkDown (Key* key, Uint32 duration);

	void update ();

	void resetKeys ();

	bool enabled = true;
};

}	// namespace Zen

#endif
