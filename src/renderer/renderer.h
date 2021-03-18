/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_RENDERER_H
#define ZEN_RENDERER_H

#include <SDL2/SDL.h>
#include <functional>
#include <vector>
#include <cmath>
#include <algorithm>

#include "../event/event_emitter.h"
#include "../math/vector2.h"
#include "../display/color.h"
#include "../texture/frame.h"
#include "../gameobjects/components/transform_matrix.h"
#include "../structs/size.h"
#include "../gameobjects/gameobject.h"

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

	std::function<void(SDL_Surface*)> callback = nullptr;

	std::function<void(Display::Color)> callbackPixel = nullptr;

	/**
	 * An internal surface pointer that keeps that last snapshot taken, or has
	 * `nullptr` if no snapshot has been taken since the creation of this renderer.
	 *
	 * @since 0.0.0
	 */
	SDL_Surface *surface = nullptr;

	bool active = false;
};

class Renderer : public Events::EventEmitter
{
public:
	Renderer (Game& game_);
	~Renderer ();

	/**
	 * The Game instance that owns this renderer.
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
	 * An intermediary render target, used to render any masked GameObject to it.
	 *
	 * The masked GameObject is drawn first, then the Masking GameObject is drawn, 
	 * with a blend operation that zeros out its RGB channels, so the color
	 * of its pixels do not matter. The operation multiplies the mask's alpha to 
	 * the destination's alpha, hiding anything not covered by the mask.
	 *
	 * The frame buffer is then itself rendered to the renderer of the window.
	 *
	 * @since 0.0.0
	 */
	SDL_Texture *maskBuffer = nullptr;

	/**
	 * An intermediary render target, used to render any camera views to it.
	 *
	 * The same operation than the maskBuffer happens here, but with the Camera view
	 * instead of a masked GameObject.
	 *
	 * The frame buffer is then itself rendered to the renderer of the window.
	 *
	 * @since 0.0.0
	 */
	SDL_Texture *cameraBuffer = nullptr;

	/**
	 * This texture receives the mask texture before being rendered over the masked
	 * GameObject.
	 *
	 * The reason the masking GameObject isn't directly used is because if it 
	 * doesn't cover the entirety of the masked GameObject, whatever isn't covered
	 * won't be hidden.
	 *
	 * This texture has the same size as the renderer, solving this problem.
	 *
	 * @since 0.0.0
	 */
	SDL_Texture *maskTexture = nullptr;

	/**
	 * The mask texture's blend mode.
	 *
	 * It is a custom SDL blend mode, configured in the Renderer's constructor,
	 * using the following components:
	 * * The pixel colors:
	 *   - Zero out the mask's color, as only the transparency matters.
	 *   - Keep the target's color by multiplying it by 1.
	 *   - Sum the two values, which will simply result in the target's color.
	 * * The pixel alpha (transparency):
	 *   - Zero out the mask's alpha, so that it won't be rendered, hidding the
	 *   target under it.
	 *   - Multiply the target's alpha by the mask's alpha (Original, not zeroed 
	 *   out). So the target will appear fully where the mask has an opacity of
	 *   1, and be completely hidden where the mask is fully transparent (alpha
	 *   of 0).
	 *   - Sum the two values, which will be the target's alpha multiplied by the
	 *   mask's alpha, plus 0.
	 *
	 * @since 0.0.0
	 */
	SDL_BlendMode maskBlendMode;

	/**
	 * The width of the renderer of the window.
	 *
	 * @since 0.0.0
	 */
	int width = 0;

	/**
	 * The width of the renderer of the window.
	 *
	 * @since 0.0.0
	 */
	int height = 0;
	
	/**
	 * The pixel format of the SDL_Window.
	 *
	 * @since 0.0.0
	 */
	SDL_PixelFormat pixelFormat;

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
	GameObjects::Components::TransformMatrix tempMatrix3;

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
	 * @param gameSize_ The default Game Size object. This is the un-modified
	 * game dimensions.
	 * @param baseSize_ The base Size object. The game dimensions multiplied by
	 * the resolution. The window width / height values match this.
	 */
	void onResize (Structs::Size gameSize_, Structs::Size baseSize_);

	/**
	 * Resize the main game canvas.
	 *
	 * @since 0.0.0
	 *
	 * @param width_ The new width of the renderer.
	 * @param height_ The new height of the renderer.
	 */
	void resize (int width_, int height_);

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
	 * @param scene_ The Scene to render.
	 * @param children_ An array of filtered Game Objects that can be rendered by the given Camera.
	 * @param camera_ The Scene Camera to render with.
	 */
	void render (Scene& scene_, std::vector<GameObjects::GameObject*> children_, Cameras::Scene2D::Camera& camera_);

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
	 * @param callback_ The function to invoke once the snapshot is created.
	 * @param path_ The file path to save the image to. Leave empty to not save any 
	 * file, just an SDL_Surface.
	 *
	 * @return This Renderer instance.
	 */
	Renderer& snapshot (std::string path_ = "", std::function<void(SDL_Surface*)> callback_ = nullptr);

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
	 * @param x_ The x coordinate to grab from.
	 * @param y_ The y coordinate to grab from.
	 * @param width_ The width of the area to grab.
	 * @param height_ The height of the area to grab.
	 * @param callback_ The function to invoke once the snapshot is created.
	 * @param path_ The file path to save the image to. Leave empty to not save any 
	 * file, just an SDL_Surface.
	 *
	 * @return This Renderer instance.
	 */
	Renderer& snapshotArea (int x_, int y_, int width_, int height_, std::string path_ = "", std::function<void(SDL_Surface*)> callback_ = nullptr);

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
	 * @param x_ - The x coordinate of the pixel to get.
	 * @param y_ - The y coordinate of the pixel to get.
	 * @param callback_ The function to invoke once the snapshot is created.
	 *
	 * @return This Renderer instance.
	 */
	Renderer& snapshotPixel (int x_, int y_, std::function<void(Display::Color)> callback_);

	/**
	 * Takes a Sprite Game Object, or any object that extends it, and draws it to the window.
	 *
	 * @since 0.0.0
	 *
	 * @param sprite_ The texture based Game Object to draw.
	 * @param frame_ The frame to draw, doesn't have to be owned by the		
	 * GameObject.
	 * @param camera_ The Camera to use for the rendering transform.
	 * @param parentTransformMatrix_ The transform matrix of the parent container, 
	 * if set.
	 */
	void batchSprite (
			GameObjects::GameObject& sprite_,
			Textures::Frame& frame_,
			Cameras::Scene2D::Camera& camera_,
			GameObjects::Components::TransformMatrix* parentTransformMatrix_ = nullptr);

	void preRenderMask (
			GameObjects::GameObject *maskedObject_ = nullptr);

	void postRenderMask (
			GameObjects::GameObject *maskObject_,
			GameObjects::GameObject *maskedObject_,
			Cameras::Scene2D::Camera *camera_);

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
