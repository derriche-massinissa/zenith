/**
 * @file		renderer.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <functional>
#include <vector>
#include "../event/event_emitter.h"

namespace Zen {
/**
 * @since 0.0.0
 *
 * @property x The x coordinate to start the snapshot from.
 * @property y The y coordinate to start the snapshot from.
 * @property width The width of the snapshot.
 * @property height The height of the snapshot.
 * @property getPixel Is this a snapshot to get a single pixel, or an area?
 * @property path The path to save the image to. This also defines the file type,
 * or `png` if only directories are given.
 * @property callback The function to call once the snapshot is created.
 * @property callbackPixel The function to call once a pixel snapshot is created.
 * @property surface A pointer to the surface the snapshot was saved in.
 * @property active If set to `true`, a snapshot will be taken after the current
 * frame is fully rendered. It will then be set back to `false`;
 */
struct SnapshotState 
{
	int x = 0;
	 
	int y = 0;

	int width = 1;

	int height = 1;

	bool getPixel = false;

	std::string path = "";

	std::function<void(SDL_Surface*)> callback = nullptr,

	std::function<void(Display::Color)> callbackPixel = nullptr,

	/**
	 * An internal surface pointer that keeps that last snapshot taken, or has
	 * `nullptr` if no snapshot has been taken since the creation of this renderer.
	 *
	 * @since 0.0.0
	 */
	SDL_Surface *surface = nullptr;

	bool active = false;
};

class Renderer : public EventEmitter
{
public:
	Renderer (Game& game_);
	~Renderer ();

	/**
	 * The Phaser Game instance that owns this renderer.
	 *
	 * @since 0.0.0
	 */
	Game& game;

	/**
	 * The Window that the Game uses.
	 *
	 * @since 0.0.0
	 */
	Window& window;

	/**
	 * The color used to clear the screen.
	 *
	 * @since 0.0.0
	 */
	Display::Color backgroundColor;

	/**
	 * The total number of Game Objects which were rendered in a frame.
	 *
	 * @since 0.0.0
	 */
	unsigned int drawCount = 0;

	/**
	 * This global alpha value is applied to anything rendered by this Renderer.
	 *
	 * @since 0.0.0
	 */
	float globalAlpha = 1.0;

	/**
	 * A temporary Transform Matrix, re-used internally during batching.
	 *
	 * @since 0.0.0
	 */
	GameObjects::Components::TransformMatrix tempMatrix1;

	/**
	 * A temporary Transform Matrix, re-used internally during batching.
	 *
	 * @since 0.0.0
	 */
	GameObjects::Components::TransformMatrix tempMatrix2;

	/**
	 * A temporary Transform Matrix, re-used internally during batching.
	 *
	 * @since 0.0.0
	 */
	GameObjects::Components::TransformMatrix tempMatrix2;

	/**
	 * Details about the currently scheduled snapshot.
	 *
	 * If `active` is set to `true` this object, a snapshot of the canvas will be 
	 * taken after the current frame is fully rendered.
	 *
	 * @since 0.0.0
	 */
	SnapshotState snapshotState;

	/**
	 * The event handler that manages the `resize` event dispatched by the Scale Manager.
	 *
	 * @since 0.0.0
	 *
	 * @param gameSize The default Game Size object. This is the un-modified
	 * game dimensions.
	 * @param baseSize The base Size object. The game dimensions multiplied by
	 * the resolution. The window width / height values match this.
	 */
	void onResize (Structs::Size gameSize, Structs::Size baseSize);

	/**
	 * Resize the main game canvas.
	 *
	 * @since 0.0.0
	 *
	 * @param width The new width of the renderer.
	 * @param height The new height of the renderer.
	 */
	void resize (int width, int height);

	/**
	 * Called at the start of the render loop.
	 *
	 * @since 0.0.0
	 */
	void preRender ();

	/**
	 * The core render step for a Scene Camera.
	 *
	 * Iterates through the given array of Game Objects and renders them with
	 * the given Camera.
	 *
	 * This is called by the CameraManager::render method. The Camera Manager
	 * instance belongs to a Scene, and is invoked by the
	 * SceneSystems::render method.
	 *
	 * This method is not called if Camera::visible is `false`, or
	 * Camera::alpha is zero.
	 *
	 * @since 0.0.0
	 *
	 * @param scene The Scene to render.
	 * @param children An array of filtered Game Objects that can be rendered by the given Camera.
	 * @param camera The Scene Camera to render with.
	 */
	void render (Scene& scene, std::vector<GameObjects::GameObject*> children, Cameras::Scene2D::Camera& camera);

	/**
	 * Takes a snapshot if one is scheduled.
	 *
	 * The post-render step happens after all Cameras in all Scenes have been
	 * rendered.
	 *
	 * @since 0.0.0
	 */
	void postRender ();

	/**
	 * Schedules a snapshot of the entire game window to be taken after the
	 * current frame is rendered.
	 *
	 * To capture a specific area see the snapshotArea method. To capture a
	 * specific pixel, see snapshotPixel.
	 *
	 * Only one snapshot can be active _per frame_. If you have already called 
	 * snapshotPixel, for example, then calling this method will override it.
	 *
	 * @since 0.0.0
	 *
	 * @param callback The function to invoke once the snapshot is created.
	 * @param path The file path to save the image to. Leave empty to not save any 
	 * file, just an SDL_Surface.
	 *
	 * @return This Renderer instance.
	 */
	Renderer& snapshot (std::function<void(SDL_Surface*)> callback, std::string path = "");

	/**
	 * Schedules a snapshot of the given area of the game viewport to be taken 
	 * after the current frame is rendered.
	 *
	 * To capture the whole game viewport see the snapshot method. To capture a 
	 * specific pixel, see snapshotPixel.
	 *
	 * Only one snapshot can be active _per frame_. If you have already called 
	 * snapshotPixel, for example, then calling this method will override it.
	 *
	 * @since 0.0.0
	 *
	 * @param x The x coordinate to grab from.
	 * @param y The y coordinate to grab from.
	 * @param width The width of the area to grab.
	 * @param height The height of the area to grab.
	 * @param callback The function to invoke once the snapshot is created.
	 * @param path The file path to save the image to. Leave empty to not save any 
	 * file, just an SDL_Surface.
	 *
	 * @return This Renderer instance.
	 */
	Renderer& snapshotArea (int x, int y, int width, int height, std::function<void(SDL_Surface*)> callback, std::string path = "");

	/**
	 * Schedules a snapshot of the given pixel from the game viewport to be taken 
	 * after the current frame is rendered.
	 *
	 * To capture the whole game viewport see the `snapshot` method. To capture a
	 * specific area, see `snapshotArea`.
	 *
	 * Only one snapshot can be active _per frame_. If you have already called
	 * snapshotArea, for example, then
	 * calling this method will override it.
	 *
	 * Unlike the other two snapshot methods, this one will return a Color
	 * object containing the color data for the requested pixel.
	 *
	 * @since 0.0.0
	 *
	 * @param x - The x coordinate of the pixel to get.
	 * @param y - The y coordinate of the pixel to get.
	 * @param callback The function to invoke once the snapshot is created.
	 *
	 * @return This Renderer instance.
	 */
	Renderer& snapshotPixel (int x, int y, std::function<void(Display::Color)> callback);

	/**
	 * Takes a Sprite Game Object, or any object that extends it, and draws it to the window.
	 *
	 * @since 0.0.0
	 *
	 * @param sprite The texture based Game Object to draw.
	 * @param frame The frame to draw, doesn't have to be owned by the		
	 * GameObject.
	 * @param camera The Camera to use for the rendering transform.
	 * @param parentTransformMatrix The transform matrix of the parent container, 
	 * if set.
	 */
	void batchSprite (
			GameObjects::GameObject& sprite,
			Textures::Frame& frame,
			Cameras::Scene2D::Camera& camera,
			GameObjects::Components::TransformMatrix* parentTransformMatrix = nullptr);

private:
	/**
	 * Takes a snapshot of an area from the current frame.
	 *
	 * @since 0.0.0
	 */
	void takeSnapshot ();

	/**
	 * Saves the snapshot surface in an image file.
	 *
	 * @since 0.0.0
	 */
	void saveSnapshot ();
};
}	// namespace Zen

#endif
