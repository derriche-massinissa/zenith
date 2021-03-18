/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_CAMERAS_SCENE2D_BASE_CAMERA_H
#define ZEN_CAMERAS_SCENE2D_BASE_CAMERA_H

#include <SDL2/SDL_types.h>

#include <memory>
#include <vector>
#include <cmath>
#include <algorithm>

#include "../../defs.h"
#include "../../event/event_emitter.h"
#include "../../gameobjects/components/alpha.h"
#include "../../gameobjects/components/visible.h"
#include "../../gameobjects/components/transform_matrix.h"
#include "../../geom/rectangle.h"
#include "../../scene/scene.fwd.h"
#include "../../scene/scene_manager.fwd.h"
#include "camera_manager.fwd.h"
#include "../../display/color.h"
#include "../../display/masks/mask.h"

#include "../../gameobjects/gameobject.h"
#include "../../gameobjects/group/group.h"

#include "../../scene/scene_manager.fwd.h"
#include "../../scale/scale_manager.h"

namespace Zen {
namespace Cameras {
namespace Scene2D {

/**
 * A Base Camera class.
 *
 * The Camera is the way in which all games are rendered in Zenith. They provide a view into your game world,
 * and can be positioned, rotated, zoomed and scrolled accordingly.
 *
 * A Camera consists of two elements: The viewport and the scroll values.
 *
 * The viewport is the physical position and size of the Camera within your game. Cameras, by default, are
 * created the same size as your game, but their position and size can be set to anything. This means if you
 * wanted to create a camera that was 320x200 in size, positioned in the bottom-right corner of your game,
 * you'd adjust the viewport to do that (using methods like `setViewport` and `setSize`).
 *
 * If you wish to change where the Camera is looking in your game, then you scroll it. You can do this
 * via the properties `scrollX` and `scrollY` or the method `setScroll`. Scrolling has no impact on the
 * viewport, and changing the viewport has no impact on the scrolling.
 *
 * By default a Camera will render all Game Objects it can see. You can change this using the `ignore` method,
 * allowing you to filter Game Objects out on a per-Camera basis.
 *
 * The Base Camera is extended by the Camera class, which adds in special effects including Fade,
 * Flash and Camera Shake, as well as the ability to follow Game Objects.
 *
 * @class BaseCamera
 * @since 0.0.0
 */
class BaseCamera
	: public Events::EventEmitter
	, public GameObjects::Components::Alpha<BaseCamera>
	, public GameObjects::Components::Visible<BaseCamera>
{
protected:
	/**
	 * The x position of the Camera viewport, relative to the top-left of the game canvas.
	 * The viewport is the area into which the camera renders.
	 * To adjust the position the camera is looking at in the game world, see the `scrollX` value.
	 *
	 * @since 0.0.0
	 */
	int x;

	/**
	 * The y position of the Camera, relative to the top-left of the game canvas.
	 * The viewport is the area into which the camera renders.
	 * To adjust the position the camera is looking at in the game world, see the `scrollY` value.
	 *
	 * @since 0.0.0
	 */
	int y;

	/**
	 * The width of the Camera viewport, in pixels.
	 *
	 * The viewport is the area into which the Camera renders. Setting the viewport does
	 * not restrict where the Camera can scroll to.
	 *
	 * @since 0.0.0
	 */
	int width;

	/**
	 * The height of the Camera viewport, in pixels.
	 *
	 * The viewport is the area into which the Camera renders. Setting the viewport does
	 * not restrict where the Camera can scroll to.
	 *
	 * @since 0.0.0
	 */
	int height;

	/**
	 * The horizontal scroll position of this Camera.
	 *
	 * Change this value to cause the Camera to scroll around your Scene.
	 *
	 * Alternatively, setting the Camera to follow a Game Object, via the `startFollow` method,
	 * will automatically adjust the Camera scroll values accordingly.
	 *
	 * You can set the bounds within which the Camera can scroll via the `setBounds` method.
	 *
	 * @since 0.0.0
	 */
	int scrollX = 0;

	/**
	 * The vertical scroll position of this Camera.
	 *
	 * Change this value to cause the Camera to scroll around your Scene.
	 *
	 * Alternatively, setting the Camera to follow a Game Object, via the `startFollow` method,
	 * will automatically adjust the Camera scroll values accordingly.
	 *
	 * You can set the bounds within which the Camera can scroll via the `setBounds` method.
	 *
	 * @since 0.0.0
	 */
	int scrollY = 0;

	/**
	 * The Camera horizontal zoom value. Change this value to zoom in, or out of, a Scene.
	 *
	 * A value of 0.5 would zoom the Camera out, so you can now see twice as much
	 * of the Scene as before. A value of 2 would zoom the Camera in, so every pixel
	 * now takes up 2 pixels when rendered.
	 *
	 * Set to 1 to return to the default zoom level.
	 *
	 * Be careful to never set this value to zero.
	 *
	 * @since 0.0.0
	 */
	double zoomX = 1;

	/**
	 * The Camera vertical zoom value. Change this value to zoom in, or out of, a Scene.
	 *
	 * A value of 0.5 would zoom the Camera out, so you can now see twice as much
	 * of the Scene as before. A value of 2 would zoom the Camera in, so every pixel
	 * now takes up 2 pixels when rendered.
	 *
	 * Set to 1 to return to the default zoom level.
	 *
	 * Be careful to never set this value to zero.
	 *
	 * @since 0.0.0
	 */
	double zoomY = 1;

	/**
	 * The rotation of the Camera in radians.
	 *
	 * Camera rotation always takes place based on the Camera viewport. By default, rotation happens
	 * in the center of the viewport. You can adjust this with the `originX` and `originY` properties.
	 *
	 * Rotation influences the rendering of _all_ Game Objects visible by this Camera. However, it does not
	 * rotate the Camera viewport itself, which always remains an axis-aligned rectangle.
	 *
	 * @since 0.0.0
	 */
	double rotation = 0;

	/**
	 * The bounds the camera is restrained to during scrolling.
	 *
	 * @since 0.0.0
	 */
	Geom::Rectangle bounds;

	/**
	 * A local transform matrix used for internal calculations.
	 *
	 * @since 0.0.0
	 */
	GameObjects::Components::TransformMatrix matrix;

	/**
	 * A temporary array of culled objects.
	 *
	 * @since 0.0.0
	 */
	std::vector<GameObjects::GameObject*> culledObjects;

	/**
	 * Does this Camera have a custom viewport?
	 *
	 * @since 0.0.0
	 */
	bool customViewport = false;

	/**
	 * The Camera that this Camera uses for translation during masking.
	 *
	 * If the mask is fixed in position this will be a reference to
	 * the `CameraManager.def` instance. Otherwise, it'll be a reference
	 * to itself.
	 *
	 * @since 0.0.0
	 */
	BaseCamera* maskCamera = nullptr;

	/**
	 * Internal method called automatically when the viewport changes.
	 *
	 * @since 0.0.0
	 */
	void updateSystem ();

public:
	/**
	 * This mask is used to determine what GameObject compenents this class inherits from.
	 *
	 * It is a bitmask.
	 *
	 * @since 0.0.0
	 */
	const int COMPONENT_MASK = COMPONENT_MASK_ALPHA | COMPONENT_MASK_VISIBLE;
	virtual int getComponentMask ();

	/**
	 * @since 0.0.0
	 *
	 * @param x_ The x position of the Camera, relative to the top-left of the 
	 * game window.
	 * @param y_ The y position of the Camera, relative to the top-left of the 
	 * game window.
	 * @param width_ The width of the Camera, in pixels.
	 * @param height_ The height of the Camera, in pixels.
	 */
	BaseCamera (int x_ = 0, int y_ = 0, int width_ = 0, int height_ = 0);

	/**
	 * @since 0.0.0
	 */
	~BaseCamera();

	/**
	 * A pointer to the Scene this camera belongs to.
	 *
	 * @since 0.0.0
	 */
	Scene *scene;

	/**
	 * A pointer to the Game's SceneManager.
	 *
	 * @since 0.0.0
	 */
	Scenes::SceneManager *sceneManager;

	/**
	 * A pointer to the Game's ScaleManager.
	 *
	 * @since 0.0.0
	 */
	Scale::ScaleManager *scaleManager;

	/**
	 * A pointer to the Scene's CameraManager.
	 *
	 * @since 0.0.0
	 */
	Cameras::Scene2D::CameraManager *cameraManager;

	/**
	 * The Camera ID. Assigned by the Camera Manager and used to handle camera exclusion.
	 * This value is a bitmask.
	 *
	 * @since 0.0.0
	 */
	int id = 0;

	/**
	 * The name of the Camera. This is left empty for your own use.
	 *
	 * @since 0.0.0
	 */
	std::string name = "";

	/**
	 * Is this Camera visible or not?
	 *
	 * A visible camera will render and perform input tests.
	 * An invisible camera will not render anything and will skip input tests.
	 *
	 * @var bool visible = true
	 * @since 0.0.0
	 */

	/**
	 * Is this Camera using a bounds to restrict scrolling movement?
	 *
	 * Set this property along with the bounds via `Camera.setBounds`.
	 *
	 * @since 0.0.0
	 */
	bool useBounds = false;

	/**
	 * The World View is a Rectangle that defines the area of the 'world' the 
	 * Camera is currently looking at.
	 * This factors in the Camera viewport size, zoom and scroll position and 
	 * is updated in the Camera preRender step.
	 * If you have enabled Camera bounds the worldview will be clamped to 
	 * those bounds accordingly.
	 * You can use it for culling or intersection checks.
	 *
	 * @since 0.0.0
	 */
	Geom::Rectangle worldView;

	/**
	 * Is this Camera dirty?
	 *
	 * A dirty Camera has had either its viewport size, bounds, scroll, 
	 * rotation or zoom levels changed since the last frame.
	 *
	 * This flag is cleared during the `postRenderCamera` method of the 
	 * renderer.
	 *
	 * @since 0.0.0
	 */
	bool dirty = true;

	/**
	 * Does this Camera have a transparent background?
	 *
	 * @since 0.0.0
	 */
	bool transparent = true;

	/**
	 * The background color of this Camera. Only used if `transparent` is `false`.
	 *
	 * @since 0.0.0
	 */
	Display::Color backgroundColor;

	/**
	 * The Camera alpha value. Setting this property impacts every single 
	 * object that this Camera
	 * renders. You can either set the property directly, i.e. via a Tween, to 
	 * fade a Camera in or out,
	 * or via the chainable `setAlpha` method instead.
	 *
	 * @var double alpha = 1.0
	 * @since 0.0.0
	 */

	/**
	 * Should the camera cull Game Objects before checking them for input hit 
	 * tests?
	 * In some special cases it may be beneficial to disable this.
	 *
	 * @since 0.0.0
	 */
	bool disableCull = false;

	/**
	 * The mid-point of the Camera in 'world' coordinates.
	 *
	 * Use it to obtain exactly where in the world the center of the camera is 
	 * currently looking.
	 *
	 * This value is updated in the preRender method, after the scroll values 
	 * and follower
	 * have been processed.
	 *
	 * @since 0.0.0
	 */
	Math::Vector2 midPoint;

	/**
	 * The horizontal origin of rotation for this Camera.
	 *
	 * By default the camera rotates around the center of the viewport.
	 *
	 * Changing the origin allows you to adjust the point in the viewport 
	 * from which rotation happens.
	 * A value of 0 would rotate from the top-left of the viewport. A value of 
	 * 1 from the bottom right.
	 *
	 * See `setOrigin` to set both origins in a single, chainable call.
	 *
	 * @since 0.0.0
	 */
	double originX = 0.5;

	/**
	 * The vertical origin of rotation for this Camera.
	 *
	 * By default the camera rotates around the center of the viewport.
	 *
	 * Changing the origin allows you to adjust the point in the viewport from 
	 * which rotation happens.
	 * A value of 0 would rotate from the top-left of the viewport. A value of 
	 * 1 from the bottom right.
	 *
	 * See `setOrigin` to set both origins in a single, chainable call.
	 *
	 * @since 0.0.0
	 */
	double originY = 0.5;

	/**
	 * The Mask this Camera is using during render.
	 * Set the mask using the `setMask` method. Remove the mask using the 
	 * `clearMask` method.
	 *
	 * @since 0.0.0
	 */
	std::unique_ptr<Display::Masks::Mask> mask;

	/**
	 * This array is populated with all of the Game Objects that this Camera 
	 * has rendered in the previous (or current, depending on when you
	 * inspect it) frame.
	 *
	 * It is cleared at the start of `Camera.preUpdate`, or if the Camera is 
	 * destroyed.
	 *
	 * You should not modify this array as it is used internally by the input 
	 * system, however you can read it as required. Note that Game Objects may 
	 * appear in this list multiple times if they belong to multiple non-
	 * exclusive Containers.
	 *
	 * @since 0.0.0
	 */
	std::vector<GameObjects::GameObject*> renderList;

	/**
	 * Adds the given Game Object to this cameras render list.
	 *
	 * This is invoked during the rendering stage. Only objects that are 
	 * actually rendered will appear in the render list.
	 *
	 * @since 0.0.0
	 *
	 * @param child_ The Game Object to add to the render list.
	 */
	void addToRenderList (GameObjects::GameObject& child_);

	/**
	 * Set the Alpha level of this Camera. The alpha controls the opacity of 
	 * the Camera as it renders.
	 * Alpha values are provided as a double between 0, fully transparent, and 
	 * 1, fully opaque.
	 *
	 * @fn BaseCamera::setAlpha (double value = 1)
	 * @since 0.0.0
	 *
	 * @param value_ The Camera alpha value.
	 *
	 * @return This Camera instance.
	 */

	/**
	 * Sets the rotation origin of this Camera.
	 *
	 * The values are given in the range 0 to 1 and are only used when calculating Camera rotation.
	 *
	 * By default the camera rotates around the center of the viewport.
	 *
	 * Changing the origin allows you to adjust the point in the viewport from which rotation happens.
	 * A value of 0 would rotate from the top-left of the viewport. A value of 1 from the bottom right.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The horizontal origin value.
	 * @param y_ The vertical origin value. If not defined it will be set to the value of `x`.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setOrigin (double x_, double y_);

	/**
	 * @overload
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The horizontal & vertical origin value.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setOrigin (double x_ = 0.5);

	/**
	 * Calculates what the Camera.scrollX and scrollY values would need to be in order to move
	 * the Camera so it is centered on the given x and y coordinates, without actually moving
	 * the Camera there. The results are clamped based on the Camera bounds, if set.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The horizontal coordinate to center on.
	 * @param y_ The vertical coordinate to center on.
	 *
	 * @return The scroll coordinates stored in the `x` and `y` properties.
	 */
	Math::Vector2 getScroll (int x_, int y_);

	/**
	 * Moves the Camera horizontally so that it is centered on the given x coordinate, bounds allowing.
	 * Calling this does not change the scrollY value.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The horizontal coordinate to center on.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& centerOnX (int x_);

	/**
	 * Moves the Camera vertically so that it is centered on the given y coordinate, bounds allowing.
	 * Calling this does not change the scrollX value.
	 *
	 * @since 0.0.0
	 *
	 * @param y_ The vertical coordinate to center on.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& centerOnY (int y_);

	/**
	 * Moves the Camera so that it is centered on the given coordinates, bounds allowing.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The horizontal coordinate to center on.
	 * @param y_ The vertical coordinate to center on.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& centerOn (int x_, int y_);

	/**
	 * Moves the Camera so that it is looking at the center of the Camera Bounds, if enabled.
	 *
	 * @since 0.0.0
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& centerToBounds ();

	/**
	 * Moves the Camera so that it is re-centered based on its viewport size.
	 *
	 * @since 0.0.0
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& centerToSize ();

	/**
	 * Takes a vector of Game Objects pointers and returns a new vector featuring 
	 * only those objects visible by this camera.
	 *
	 * @since 0.0.0
	 *
	 * @param renderableObjects_ An array of Game Objects to cull.
	 *
	 * @return A vector of Game Objects visible to this Camera.
	 */
	std::vector<GameObjects::GameObject*> cull (
			std::vector<GameObjects::GameObject*> renderableObjects_);

	/**
	 * Converts the given `x` and `y` coordinates into World space, based on this Cameras transform.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The x position to convert to world space.
	 * @param y_ The y position to convert to world space.
	 *
	 * @return An object holding the converted values in its `x` and `y` properties.
	 */
	Math::Vector2 getWorldPoint (int x_, int y_);

	/**
	 * Given a Game Object, or an array of Game Objects, it will update all of their camera filter settings
	 * so that they are ignored by this Camera. This means they will not be rendered by this Camera.
	 *
	 * @since 0.0.0
	 *
	 * @param entry_ The GameObject to be ignored by this Camera.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& ignore (GameObjects::GameObject* entry_);

	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param entries_ A vector of GameObjects, to be ignored by this Camera.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& ignore (std::vector<GameObjects::GameObject*>& entries_);

	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param entries_ A Group game object to be ignored by this Camera.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& ignore (GameObjects::Group& entries_);

	/**
	 * Internal preRender step.
	 *
	 * @since 0.0.0
	 */
	void preRender ();

	/**
	 * Takes an x value and checks it's within the range of the Camera bounds, adjusting if required.
	 * Do not call this method if you are not using camera bounds.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The value to horizontally scroll clamp.
	 *
	 * @return The adjusted value to use as scrollX.
	 */
	int clampX (int x_);

	/**
	 * Takes a y value and checks it's within the range of the Camera bounds, adjusting if required.
	 * Do not call this method if you are not using camera bounds.
	 *
	 * @since 0.0.0
	 *
	 * @param y_ The value to vertically scroll clamp.
	 *
	 * @return The adjusted value to use as scrollY.
	 */
	int clampY (int y_);

	/**
	 * If this Camera has previously had movement bounds set on it, this will remove them.
	 *
	 * @since 0.0.0
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& removeBounds ();

	/**
	 * Set the rotation of this Camera. This causes everything it renders to appear rotated.
	 *
	 * Rotating a camera does not rotate the viewport itself, it is applied during rendering.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ The cameras angle of rotation, given in degrees.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setAngle (double value_ = 0);

	/**
	 * Sets the background color for this Camera.
	 *
	 * By default a Camera has a transparent background but it can be given a 
	 * solid color, with any level of transparency, via this method.
	 *
	 * The color value can be specified using hex or numbers.
	 *
	 * @since 0.0.0
	 *
	 * @param color_ The color value. In hex color notation.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setBackgroundColor (int color_);

	/**
	 * @overload
	 *
	 * @since 0.0.0
	 *
	 * @param r_ The red color value.
	 * @param g_ The red color value.
	 * @param b_ The red color value.
	 * @param a_ The alpha value.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setBackgroundColor (int r_ = 0, int g_ = 0, int b_ = 0, int a_ = 0);

	/**
	 * Set the bounds of the Camera. The bounds are an axis-aligned rectangle.
	 *
	 * The Camera bounds controls where the Camera can scroll to, stopping it from scrolling off the
	 * edges and into blank space. It does not limit the placement of Game Objects, or where
	 * the Camera viewport can be positioned.
	 *
	 * Temporarily disable the bounds by changing the boolean `Camera.useBounds`.
	 *
	 * Clear the bounds entirely by calling `Camera.removeBounds`.
	 *
	 * If you set bounds that are smaller than the viewport it will stop the Camera from being
	 * able to scroll. The bounds can be positioned where-ever you wish. By default they are from
	 * 0x0 to the canvas width x height. This means that the coordinate 0x0 is the top left of
	 * the Camera bounds. However, you can position them anywhere. So if you wanted a game world
	 * that was 2048x2048 in size, with 0x0 being the center of it, you can set the bounds x/y
	 * to be -1024, -1024, with a width and height of 2048. Depending on your game you may find
	 * it easier for 0x0 to be the top-left of the bounds, or you may wish 0x0 to be the middle.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The top-left x coordinate of the bounds.
	 * @param y_ The top-left y coordinate of the bounds.
	 * @param width_ The width of the bounds, in pixels.
	 * @param height_ The height of the bounds, in pixels.
	 * @param centerOn_ If `true` the Camera will automatically be centered on the new bounds.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setBounds (int x_, int y_, int width_, int height_, bool centerOn_ = false);

	/**
	 * Returns a rectangle containing the bounds of the Camera.
	 *
	 * If the Camera does not have any bounds the rectangle will have -1 on all components.
	 *
	 * The rectangle is a copy of the bounds, so is safe to modify.
	 *
	 * @since 0.0.0
	 *
	 * @return A rectangle containing the bounds of this Camera.
	 */
	Geom::Rectangle getBounds ();

	/**
	 * Sets the name of this Camera.
	 * This value is for your own use and isn't used internally.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ The name of the Camera.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setName (std::string value_ = "");

	/**
	 * Set the position of the Camera viewport within the game.
	 *
	 * This does not change where the camera is 'looking'. See `setScroll` to control that.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The top-left x coordinate of the Camera viewport.
	 * @param y_ The top-left y coordinate of the Camera viewport.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setPosition (int x_, int  y_);

	/**
	 * @overload
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The top-left x coordinate of the Camera viewport. Will also set y to be the same as x.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setPosition (int x_);

	/**
	 * Set the rotation of this Camera. This causes everything it renders to appear rotated.
	 *
	 * Rotating a camera does not rotate the viewport itself, it is applied during rendering.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ The rotation of the Camera, in radians.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setRotation (double value_ = 0.0);

	/**
	 * Sets the Scene the Camera is bound to.
	 *
	 * @since 0.0.0
	 *
	 * @param scene_ The Scene the camera is bound to.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setScene (Scene* scene_);

	/**
	 * Set the position of where the Camera is looking within the game.
	 * You can also modify the properties `Camera.scrollX` and `Camera.scrollY` directly.
	 * Use this method, or the scroll properties, to move your camera around the game world.
	 *
	 * This does not change where the camera viewport is placed. See `setPosition` to control that.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The x coordinate of the Camera in the game world.
	 * @param y_ The y coordinate of the Camera in the game world.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setScroll (int x_, int y_);

	/**
	 * @overload
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The x & y coordinate of the Camera in the game world.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setScroll (int x_);

	/**
	 * Set the size of the Camera viewport.
	 *
	 * By default a Camera is the same size as the game, but can be made smaller via this method,
	 * allowing you to create mini-cam style effects by creating and positioning a smaller Camera
	 * viewport within your game.
	 *
	 * @since 0.0.0
	 *
	 * @param width_ The width of the Camera viewport.
	 * @param height_ The height of the Camera viewport.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setSize (int width_, int height_);

	/**
	 * @overload
	 *
	 * @since 0.0.0
	 *
	 * @param size_ The width and height of the Camera viewport.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setSize (int size_);

	/**
	 * This method sets the position and size of the Camera viewport in a single 
	 * call.
	 *
	 * If you're trying to change where the Camera is looking at in your game, 
	 * then see the method `Camera.setScroll` instead. This method is for changing 
	 * the viewport itself, not what the camera can see.
	 *
	 * By default a Camera is the same size as the game, but can be made smaller 
	 * via this method, allowing you to create mini-cam style effects by creating 
	 * and positioning a smaller Camera viewport within your game.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The top-left x coordinate of the Camera viewport.
	 * @param y_ The top-left y coordinate of the Camera viewport.
	 * @param width_ The width of the Camera viewport.
	 * @param height_ The height of the Camera viewport.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setViewport (int x_, int y_, int width_, int height_ = -1);

	/**
	 * Set the zoom value of the Camera.
	 *
	 * Changing to a smaller value, such as 0.5, will cause the camera to
	 * 'zoom out'.
	 * Changing to a larger value, such as 2, will cause the camera to 'zoom in'.
	 *
	 * A value of 1 means 'no zoom' and is the default.
	 *
	 * Changing the zoom does not impact the Camera viewport in any way, it is 
	 * only applied during rendering.
	 *
	 * You can set the horizontal and vertical zoom values independently.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The horizontal zoom value of the Camera. The minimum it can be is 
	 * 0.001.
	 * @param y_ The vertical zoom value of the Camera. The minimum it can be is
	 * 0.001.
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setZoom (double x_ = 1.0, double y_ = 1.0);

	/**
	 * Sets the mask to be applied to this Camera during rendering.
	 *
	 * The mask must have been previously created and can be either a GeometryMask or a BitmapMask.
	 *
	 * Bitmap Masks only work on WebGL. Geometry Masks work on both WebGL and Canvas.
	 *
	 * If a mask is already set on this Camera it will be immediately replaced.
	 *
	 * Masks have no impact on physics or input detection. They are purely a rendering component
	 * that allows you to limit what is visible during the render pass.
	 *
	 * @since 0.0.0
	 *
	 * @param mask_ The mask this Camera will use when rendering.
	 * @param fixedPosition_ Should the mask translate along with the Camera, or be fixed in place and not impacted by the Cameras transform?
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& setMask (Display::Masks::Mask mask_, bool fixedPosition_ = true);

	/**
	 * Clears the mask that this Camera was using.
	 *
	 * @since 0.0.0
	 *
	 * @param destroyMask_ Destroy the mask before clearing it?
	 *
	 * @return This Camera instance.
	 */
	BaseCamera& clearMask ();

	/**
	 * Sets the visibility of this Camera.
	 *
	 * An invisible Camera will skip rendering and input tests of everything it can see.
	 *
	 * @fn BaseCamera::setVisible (bool value)
	 * @since 0.0.0
	 *
	 * @param value_ The visible state of the Camera.
	 *
	 * @return This Camera instance.
	 */

	/**
	 * Internal method called automatically by the Camera Manager.
	 *
	 * @since 0.0.0
	 *
	 * @param time_ The amount of ms since SDL started.
	 * @param delta_ The delta time, in ms, elapsed since the last frame.
	 */
	virtual void update (Uint32 time_, Uint32 delta_);

	/**
	 * Getter for private member `x`.
	 *
	 * @since 0.0.0
	 *
	 * @return The value of `x`.
	 */
	int getX ();

	/**
	 * Setter for private member `x`.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The value to set `x` to.
	 *
	 * @return This Base Camera instance.
	 */
	BaseCamera& setX (int x_);

	/**
	 * Getter for private member `y`.
	 *
	 * @since 0.0.0
	 *
	 * @return The value of `y`.
	 */
	int getY ();

	/**
	 * Setter for private member `y`.
	 *
	 * @since 0.0.0
	 *
	 * @param y_ The value to set `y` to.
	 *
	 * @return This Base Camera instance.
	 */
	BaseCamera& setY (int y_);

	/**
	 * Getter for private member `width`.
	 *
	 * @since 0.0.0
	 *
	 * @return The value of `width`.
	 */
	int getWidth ();

	/**
	 * Setter for private member `width`.
	 *
	 * @since 0.0.0
	 *
	 * @param width_ The value to set `width` to.
	 *
	 * @return This Base Camera instance.
	 */
	BaseCamera& setWidth (int width_);

	/**
	 * Getter for private member `height`.
	 *
	 * @since 0.0.0
	 *
	 * @return The value of `height`.
	 */
	int getHeight ();

	/**
	 * Setter for private member `height`.
	 *
	 * @since 0.0.0
	 *
	 * @param height_ The value to set `height` to.
	 *
	 * @return This Base Camera instance.
	 */
	BaseCamera& setHeight (int height_);

	/**
	 * Getter for private member `scrollX`.
	 *
	 * @since 0.0.0
	 *
	 * @return The value of `scrollX`.
	 */
	int getScrollX ();

	/**
	 * Setter for private member `scrollX`.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ The value to set `scrollX` to.
	 *
	 * @return This Base Camera instance.
	 */
	BaseCamera& setScrollX (int value_ = 0);

	/**
	 * Getter for private member `scrollY`.
	 *
	 * @since 0.0.0
	 *
	 * @return The value of `scrollY`.
	 */

	int getScrollY ();

	/**
	 * Setter for private member `scrollY`.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ The value to set `scrollY` to.
	 *
	 * @return This Base Camera instance.
	 */
	BaseCamera& setScrollY (int value_ = 0);

	/**
	 * Getter for private member `zoom`.
	 *
	 * @since 0.0.0
	 *
	 * @return The value of `zoom`.
	 */
	double getZoom ();

	/**
	 * Getter for private member `zoomX`.
	 *
	 * @since 0.0.0
	 *
	 * @return The value of `zoomX`.
	 */
	double getZoomX ();

	/**
	 * Setter for private member BaseCamera::zoomX.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ The value to set `zoomX` to.
	 *
	 * @return This Base Camera instance.
	 */
	BaseCamera& setZoomX (double value_ = 1.0);

	/**
	 * Getter for private member `zoomY`.
	 *
	 * @since 0.0.0
	 *
	 * @return The value of `zoomY`.
	 */
	double getZoomY ();

	/**
	 * Setter for private member `zoomY`.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ The value to set `zoomY` to.
	 *
	 * @return This Base Camera instance.
	 */
	BaseCamera& setZoomY (double value_ = 1.0);

	/**
	 * Getter for private member `rotation`.
	 *
	 * @since 0.0.0
	 *
	 * @return The value of `rotation`.
	 */
	double getRotation ();

	/**
	 * The horizontal position of the center of the Camera's viewport, relative to the left of the game canvas.
	 *
	 * @since 0.0.0
	 *
	 * @return The `x` coordinate of the center of this Camera's viewport.
	 */
	double getCenterX ();

	/**
	 * The vertical position of the center of the Camera's viewport, relative to the top of the game canvas.
	 *
	 * @since 0.0.0
	 *
	 * @return The `y` coordinate of the center of this Camera's viewport.
	 */
	double getCenterY ();

	/**
	 * The displayed width of the camera viewport, factoring in the camera zoom level.
	 *
	 * If a camera has a viewport width of 800 and a zoom of 0.5 then its display width
	 * would be 1600, as it's displaying twice as many pixels as zoom level 1.
	 *
	 * Equally, a camera with a width of 800 and zoom of 2 would have a display width
	 * of 400 pixels.
	 *
	 * @since 0.0.0
	 *
	 * @return The display width.
	 */
	int getDisplayWidth ();

	/**
	 * The displayed height of the camera viewport, factoring in the camera zoom level.
	 *
	 * If a camera has a viewport height of 600 and a zoom of 0.5 then its display height
	 * would be 1200, as it's displaying twice as many pixels as zoom level 1.
	 *
	 * Equally, a camera with a height of 600 and zoom of 2 would have a display height
	 * of 300 pixels.
	 *
	 * @since 0.0.0
	 *
	 * @return The display height.
	 */
	int getDisplayHeight ();
};

}	// namespace Scene2D
}	// namespace Cameras
}	// namespace Zen

// Declaration of forward declared elements
//#include "../../scene/scene_manager.h"

#endif
