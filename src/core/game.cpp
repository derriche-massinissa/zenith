/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "game.hpp"

#include "../utils/messages.hpp"
#include "config.hpp"
#include "../input/input_manager.hpp"
#include "../input/mouse/mouse_manager.hpp"
#include "../input/keyboard/keyboard_manager.hpp"
#include "../audio/audio_manager.hpp"

namespace Zen {

// Global Systems
entt::registry g_registry;
EventEmitter g_event;
Window g_window;
TextureManager g_texture;
ScaleManager g_scale;
Renderer g_renderer;
extern Scenes::SceneManager g_scene;
InputManager g_input;
extern MouseManager g_mouse;
extern KeyboardManager g_keyboard;
extern AudioManager g_audio;

Game::Game (GameConfig& config_)
	: events (g_event)
	, config (config_)
	, window (g_window)
	, renderer (g_renderer)
	, textures (g_texture)
	, scene (g_scene)
	, scale (g_scale)
{
	// Step delta for when the window is minimized/not visible
	hiddenDelta = config_.hiddenDelta;

	boot();
}

Game::~Game ()
{
	events.emit("destroy");

	events.removeAllListeners();

	// FIXME
	// Destroying the SDL_Renderer from inside the Window global object makes everything crash to the ground, it works from here, but it would be good to know what's happening... So yeah future me, investigate!
	g_window.close();
}

void Game::boot ()
{
	isBooted = true;

	window.create(&config);

	renderer.start(&config);

	scale.boot(&config);

	scene.boot(this, &config.sceneFactory);

	g_input.boot(&config);

	if (config.inputMouse)
		g_mouse.boot();

	if (config.inputKeyboard)
		g_keyboard.boot();

	g_audio.boot();

	window.on("minimize", &Game::onMinimize, this);
	window.on("restore", &Game::onRestore, this);

	events.emit("boot");

	start();
}

void Game::start ()
{
	isRunning = true;

	events.emit("ready");

	window.once("quit", &Game::shutdown, this);

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
	events.emit("pre-step", time_, delta_);

	// Mostly meant for user-land code and plugins
	events.emit("step", time_, delta_);

	// Update the Scene Manager and all active Scenes
	scene.update(time_, delta_);
	g_audio.update(time_, delta_);

	// Final event before rendering starts
	events.emit("post-step", time_, delta_);

	// Run the Pre-Renderer (Clearing the window, setting background colors, etc...)
	g_input.preRender(time_, delta_);
	renderer.preRender();
	events.emit("pre-render", time_, delta_);

	// The main render loop. Iterates all Scenes and all Cameras in those
	// scenes, rendering to the renderer instance.
	scene.render();

	// The Post-Render call. Tidies up loose end, takes snapshots, etc...
	renderer.postRender();

	// Final event before the step repeats. Last chance to do anything before
	// it all starts again.
	events.emit("post-render", time_, delta_);
}

void Game::headlessStep (Uint32 time_, Uint32 delta_)
{
	if (hiddenDelta)
		SDL_Delay(hiddenDelta);

	// Managers
	events.emit("pre-step", time_, delta_);
	events.emit("step", time_, delta_);

	// Scenes
	scene.update(time_, delta_);
	events.emit("post-step", time_, delta_);

	// Render
	events.emit("pre-render", time_, delta_);
	events.emit("post-render", time_, delta_);
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
