/**
 * @file		game.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "game.h"

namespace Zen {

Game::Game (GameConfig& cfg)
	: config(cfg)
	, window(*this)
	, scene(*this, cfg.sceneFactory)
{
	// Initialize some members
	pendingShutdown = false;
	isBooted = false;
	isRunning = false;
	hasFocus = false;
	isVisible = false;

	messageNote("Game constructed.");

	boot();
}

Game::~Game ()
{
	//scene.destroy();

	events.emit("SYS_DESTROY");

	events.removeAllListeners();

	//renderer.destroy();

	//window.destroy();

	//loop.destroy();

	messageNote("Game destructed.");
}

void Game::boot ()
{
	isBooted = true;

	//scale.preBoot();

	window.create(config);

	//DebugHeader();

	events.emit("boot");

	start();
}

void Game::start ()
{
	isRunning = true;

	//VisibilityHandler();

	//events.on("hidden", std::bind(&Game::onHidden, this));
	//events.on("visible", std::bind(&Game::onVisible, this));
	//events.on("blur", std::bind(&Game::onBlur, this));
	//events.on("focus", std::bind(&Game::onFocus, this));

	events.once("SYS_QUIT", &Game::shutdown, this);

	// Start the main game loop
	loop.start(std::bind(
		&Game::step,
		this,
		std::placeholders::_1,
		std::placeholders::_2
		));
}

void Game::step (Uint32 time, Uint32 delta)
{
	if (pendingShutdown) {
		runShutdown();
		return;
	}

	t_threashold += delta;
	count_step++;
	if (t_threashold >= 1000) {
		std::cout << "Second passed... Steps: " << count_step << std::endl;
		t_threashold = 0;
		count_step = 0;
	}

	// Only run the logic without rendering anything if the window is hidden
	//if (!isVisible) {
	//	headlessStep(time, delta);
	//	return;
	//}

	// Managers like Input and Sound in the prestep
	window.handleSDLEvents();
	events.emit("SYS_PRE_STEP", {{time, delta}});

	// Mostly meant for user-land code and plugins
	events.emit("SYS_STEP", {{time, delta}});

	// Update the Scene Manager and all active Scenes
	//////scene.update(time, delta);

	// Final event before rendering starts
	events.emit("SYS_POST_STEP", {{time, delta}});

	// Run the Pre-Renderer (Clearing the canvas, setting background colors,
	// etc...)
	//////renderer.preRender();
	events.emit("SYS_PRE_RENDER", {{time, delta}});

	// The main render loop. Iterates all Scenes and all Cameras in those
	// scenes, rendering to the renderer instance.
	//////scene.render(renderer);

	// The Post-Render call. Tidies up loose end, takes snapshots, etc...
	//////renderer.postRender();

	// Final event before the step repeats. Last chance to do anything before
	// it all starts again.
	events.emit("SYS_POST_RENDER", {{time, delta}});
}

void Game::headlessStep (Uint32 time, Uint32 delta)
{
	/*
	if (pendingDestroy) {
		runDestroy();
		return;
	}

	// Managers
	events.emit(EVENTS::PRE_STEP, time, delta);
	events.emit(EVENTS::STEP, time, delta);

	// Scenes
	//scene.update(time, delta);

	events.emit(EVENTS::POST_STEP, time, delta);

	// Render
	events.emit(EVENTS::PRE_RENDER);
	events.emit(EVENTS::POST_RENDER);
	*/
}

Uint32 Game::getFrame ()
{
	//return loop.frame;
}

Uint32 Game::getTime ()
{
	//return loop.now;
}

void Game::shutdown (Data data)
{
	pendingShutdown = true;
}

void Game::runShutdown ()
{
	loop.shutdown();

	pendingShutdown = false;
}

}	// namespace Zen
