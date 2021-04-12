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
#include "../../scene/scene.fwd.h"
#include "types/cursor_keys.hpp"
#include "types/key_combo_config.hpp"
#include "keys/key.hpp"
#include "combo/key_combo.hpp"
#include "../../event/event_listener.fwd.hpp"
#include <memory>

namespace Zen {

/**
 * The KeyboardPlugin is an input plugin that belongs to the Scene-owned
 * InputPlugin.
 *
 * Its role is to listen for native Keyboard Events and then process them.
 *
 * You do not need to create this class directly, the InputPlugin will create an
 * instance of it automatically.
 *
 * You can access it from within a Scene using `input.keyboard`. For example, you
 * can do:
 *
 * ```cpp
 * input.keyboard.on("keydown", callback, context);
 * ```
 *
 * You can also create Key objects, which you can then poll in your game loop:
 *
 * ```cpp
 * Key* spaceBar = input.keyboard.addKey(SDLK_SPACE);
 * ```
 *
 * @class KeyboardPlugin
 * @since 0.0.0
 *
 * @param sceneInputPlugin A pointer to the Scene InputPlugin that the
 * KeyboardPlugin belongs to.
 */
class KeyboardPlugin : public EventEmitter
{
public:
	KeyboardPlugin (Scene* scene);

	~KeyboardPlugin ();

	void start ();

	void shutdown ();

	bool isActive ();

	CursorKeys createCursorKeys ();

	Key* addKey (SDL_Keycode keys_, bool emitOnRepeat_ = false);

	Key* addKey (std::string keys_, bool emitOnRepeat_ = false);

	Key* addKey (char keys_, bool emitOnRepeat_ = false);

	void removeKey (Key* key_);

	void removeKey (SDL_Keycode key_);

	void removeKey (std::string key_);

	void removeKey (char key_);

	void removeAllKeys ();

	KeyCombo* createCombo (std::string keys_, KeyComboConfig config_ = {});

	KeyCombo* createCombo (std::vector<std::string> keys_, KeyComboConfig config_ = {});

	KeyCombo* createCombo (std::vector<SDL_Keycode> keys_, KeyComboConfig config_ = {});

	void processKeyDown (const SDL_Event * const event_);

	void processKeyUp (const SDL_Event * const event_);

	Key* getKeyFromKeycode (SDL_Keycode keycode_);

	void resetKeys ();

	Scene* scene;

	bool enabled = true;

	std::vector<Key> keys;

	std::vector<std::unique_ptr<KeyCombo>> combos;

	/**
	 * Internal repeat key flag.
	 *
	 * @since 0.0.0
	 */
	SDL_KeyCode prevCode = SDLK_UNKNOWN;

	/**
	 * Internal repeat key flag.
	 *
	 * @since 0.0.0
	 */
	Uint32 prevTime = 0;

	ListenerBase* lKeyUp = nullptr;

	ListenerBase* lKeyDown = nullptr;

	ListenerBase* lBlur = nullptr;

	ListenerBase* lPause = nullptr;

	ListenerBase* lSleep = nullptr;
};

}	// namespace Zen

#endif
