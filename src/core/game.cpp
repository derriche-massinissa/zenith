/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "game.hpp"

#include "../utils/messages.hpp"
#include "config.hpp"
#include "../ecs/entity.hpp"
#include "../event/event_emitter.hpp"
#include "../window/window.hpp"
#include "../texture/texture_manager.hpp"
#include "../scale/scale_manager.hpp"
#include "../renderer/renderer.hpp"
#include "../scene/scene_manager.hpp"
#include "../input/input_manager.hpp"
#include "../input/mouse/mouse_manager.hpp"
#include "../input/keyboard/keyboard_manager.hpp"
#include "../audio/audio_manager.hpp"
#include "../text/text_manager.hpp"

namespace Zen {

// Global Systems
GameConfig *g_config = nullptr;
entt::registry g_registry;
EventEmitter g_event;
Window g_window;
TextureManager g_texture;
ScaleManager g_scale;
Renderer g_renderer;
InputManager g_input;
MouseManager g_mouse;
KeyboardManager g_keyboard;
AudioManager g_audio;
SceneManager g_scene;
TextManager g_text;

Game::Game (GameConfig& config_)
	: config (config_)
{
	g_config = &config_;

	// Step delta for when the window is minimized/not visible
	hiddenDelta = config_.hiddenDelta;

	boot();
}

Game::~Game ()
{
	g_event.removeAllListeners();

	// FIXME
	// Destroying the SDL_Renderer from inside the Window global object makes
	// everything crash to the ground, it works from here, but it would be good to
	// know what's happening... So yeah future me, investigate!
	g_window.close();
}

void Game::boot ()
{
	isBooted = true;

	g_window.create(&config);

	g_texture.boot(&config);

	g_scale.boot(&config);

	g_renderer.start(&config);

	g_scene.boot(&config.sceneFactory);

	g_input.boot(&config);

	g_text.boot();

	if (config.inputMouse)
		g_mouse.boot();

	if (config.inputKeyboard)
		g_keyboard.boot();

	g_audio.boot();

	g_window.on("minimize", &Game::onMinimize, this);
	g_window.on("restore", &Game::onRestore, this);

	g_event.emit("boot");

	start();
}

void Game::start ()
{
	isRunning = true;

	g_event.emit("ready");

	// Start the main game loop
	loop.start(std::bind(
		&Game::step,
		this,
		std::placeholders::_1,
		std::placeholders::_2
		));
}

void Game::step (Uint32 time_, Uint32 delta_)
{
	if (pendingShutdown)
	{
		runShutdown();
		return;
	}

	// Handle SDL events
	handleSDLEvents();

	// Only run the logic without rendering anything if the window is hidden
	if (!isVisible)
	{
		headlessStep(time_, delta_);
		return;
	}

	// Managers like Input and Sound in the prestep
	g_event.emit("pre-step", time_, delta_);

	// Mostly meant for user-land code and plugins
	g_event.emit("step", time_, delta_);

	// Update the Scene Manager and all active Scenes
	g_scene.update(time_, delta_);
	g_audio.update(time_, delta_);

	// Final event before rendering starts
	g_event.emit("post-step", time_, delta_);

	// Run the Pre-Renderer (Clearing the window, setting background colors, etc...)
	g_input.preRender(time_, delta_);
	g_renderer.preRender();
	g_event.emit("pre-render", time_, delta_);

	// The main render loop. Iterates all Scenes and all Cameras in those
	// scenes, rendering to the renderer instance.
	g_scene.render();

	// The Post-Render call. Tidies up loose end, takes snapshots, etc...
	g_renderer.postRender();

	// Final event before the step repeats. Last chance to do anything before
	// it all starts again.
	g_event.emit("post-render", time_, delta_);
}

void Game::headlessStep (Uint32 time_, Uint32 delta_)
{
	// Managers
	g_event.emit("pre-step", time_, delta_);
	g_event.emit("step", time_, delta_);
	g_scene.update(time_, delta_);
	g_audio.update(time_, delta_);

	// Scenes
	g_scene.update(time_, delta_);
	g_event.emit("post-step", time_, delta_);

	// Render
	g_event.emit("pre-render", time_, delta_);
	g_event.emit("post-render", time_, delta_);

	if (hiddenDelta)
		SDL_Delay(hiddenDelta);
	else
		SDL_Delay(20);
}

Uint32 Game::getFrame ()
{
	return loop.frame;
}

Uint32 Game::getTime ()
{
	return loop.now;
}

void Game::onMinimize ()
{
	isVisible = false;
}

void Game::onRestore ()
{
	isVisible = true;
}

void Game::shutdown ()
{
	pendingShutdown = true;
}

void Game::runShutdown ()
{
	loop.shutdown();

	pendingShutdown = false;
}

}	// namespace Zen
