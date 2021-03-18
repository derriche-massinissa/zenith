/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_CAMERAS_SCENE2D_CAMERA_H
#define ZEN_CAMERAS_SCENE2D_CAMERA_H

#include <SDL2/SDL_types.h>

#include <memory>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>

#include "../../defs.h"
#include "../../event/event_emitter.h"
#include "../../geom/rectangle.h"

#include "camera_manager.fwd.h"
#include "../../renderer/renderer.fwd.h"

#include "../../scene/scene.fwd.h"
#include "../../scene/scene_manager.fwd.h"

#include "../../display/color.h"
#include "../../math/vector2.h"

#include "../../gameobjects/components/transform_matrix.h"
#include "../../gameobjects/components/alpha.h"
#include "../../gameobjects/components/visible.h"
#include "../../gameobjects/components/flip.h"
#include "../../gameobjects/components/tint.h"

#include "../../gameobjects/gameobject.h"
#include "../../gameobjects/group/group.h"

#include "../../scene/scene_manager.fwd.h"
#include "../../scale/scale_manager.h"

#include "effects/fade.h"
#include "effects/flash.h"
#include "effects/pan.h"
#include "effects/rotate_to.h"
#include "effects/shake.h"
#include "effects/zoom.h"

/**
 * @todo Move all TODO to Doxygen documentation!
 *
 * @todo GameObjects::GameObject
 * @todo GameObjects::RenderTexture
 * @todo GameObjects::Group
 * @todo GameObjects::DisplayList
 * @todo GameObjects::Components::Flip
 * @todo GameObjects::Components::Tint
 * @todo Input::Pointer
 * @todo Cameras::Scene2D::Effects::Fade
 * @todo Cameras::Scene2D::Effects::Flash
 * @todo Cameras::Scene2D::Effects::Pan
 * @todo Cameras::Scene2D::Effects::Shake
 * @todo Cameras::Scene2D::Effects::RotateTo
 * @todo Cameras::Scene2D::Effects::Zoom
 */
namespace Zen {
namespace Cameras {
namespace Scene2D {

/**
 * A Camera.
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
 * A Camera also has built-in special effects including Fade, Flash and Camera Shake, and can follow GameObjects.
 *
 * @class Camera
 * @since 0.0.0
 */
class Camera
	: public Events::EventEmitter
	, public GameObjects::Components::Alpha<Camera>
	, public GameObjects::Components::Visible<Camera>
	, public GameObjects::Components::Flip<Camera>
	, public GameObjects::Components::Tint<Camera>
{
public:
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
	Camera* maskCamera = nullptr;

	/**
	 * Does this Camera allow the Game Objects it renders to receive input events?
	 *
	 * @since 0.0.0
	 */
	bool inputEnabled = true;

	/**
	 * The Camera Fade effect handler.
	 * To fade this camera see the `Camera.fade` methods.
	 *
	 * @since 0.0.0
	 */
	Effects::Fade fadeEffect;

	/**
	 * The Camera Flash effect handler.
	 * To flash this camera see the `Camera.flash` method.
	 *
	 * @since 0.0.0
	 */
	Effects::Flash flashEffect;

	/**
	 * The Camera Shake effect handler.
	 * To shake this camera see the `Camera.shake` method.
	 *
	 * @since 0.0.0
	 */
	Effects::Shake shakeEffect;

	/**
	 * The Camera Pan effect handler.
	 * To pan this camera see the `Camera.pan` method.
	 *
	 * @since 0.0.0
	 */
	Effects::Pan panEffect;

	/**
	 * The Camera Rotate To effect handler.
	 * To rotate this camera see the `Camera.rotateTo` method.
	 *
	 * @since 0.0.0
	 */
	Effects::RotateTo rotateToEffect;

	/**
	 * The Camera Zoom effect handler.
	 * To zoom this camera see the `Camera.zoom` method.
	 *
	 * @since 0.0.0
	 */
	Effects::Zoom zoomEffect;

	/**
	 * The linear interpolation value to use when following a target.
	 *
	 * Can also be set via `setLerp` or as part of the `startFollow` call.
	 *
	 * The default values of 1 means the camera will instantly snap to the target coordinates.
	 * A lower value, such as 0.1 means the camera will more slowly track the target, giving
	 * a smooth transition. You can set the horizontal and vertical values independently, and also
	 * adjust this value in real-time during your game.
	 *
	 * Be sure to keep the value between 0 and 1. A value of zero will disable tracking on that axis.
	 *
	 * @since 0.0.0
	 */
	Math::Vector2 lerp;

	/**
	 * The values stored in this property are subtracted from the Camera targets position, allowing you to
	 * offset the camera from the actual target x/y coordinates by this amount.
	 * Can also be set via `setFollowOffset` or as part of the `startFollow` call.
	 *
	 * @since 0.0.0
	 */
	Math::Vector2 followOffset;

	/**
	 * The Camera dead zone.
	 *
	 * The deadzone is only used when the camera is following a target.
	 *
	 * It defines a rectangular region within which if the target is present, the camera will not scroll.
	 * If the target moves outside of this area, the camera will begin scrolling in order to follow it.
	 *
	 * The `lerp` values that you can set for a follower target also apply when using a deadzone.
	 *
	 * You can set this property by using the `setDeadzone` method for a chainable approach.
	 *
	 * The rectangle you provide can have its dimensions adjusted dynamically, however, please
	 * note that its position is updated every frame, as it is constantly re-centered on the cameras mid point.
	 *
	 * Calling `setDeadzone` with no arguments will reset an active deadzone, as will setting this property
	 * to `null`.
	 *
	 * @since 0.0.0
	 */
	std::shared_ptr<Geom::Rectangle> deadzone;

	/**
	 * Internal follow target reference.
	 *
	 * @since 0.0.0
	 */
	GameObjects::GameObject* follow = nullptr;
	
	int renderFlags = 15;

	/**
	 * Internal method called automatically when the viewport changes.
	 *
	 * @since 0.0.0
	 */
	void updateSystem ();

public:
	/**
	 * This mask is used to determine what GameObject compenents this class 
	 * inherits from.
	 *
	 * It is a bitmask.
	 *
	 * @since 0.0.0
	 */
	static const int COMPONENT_MASK =
		COMPONENT_MASK_ALPHA	|
		COMPONENT_MASK_VISIBLE	|
		COMPONENT_MASK_FLIP		|
		COMPONENT_MASK_TINT		;

	/**
	 * Returns the object's Component Bitmask.
	 *
	 * @since 0.0.0
	 *
	 * @return The object's Component Bitmask.
	 */
	int getComponentMask ();

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
	Camera (int x_ = 0, int y_ = 0, int width_ = 0, int height_ = 0);

	/**
	 * @since 0.0.0
	 *
	 * @param other_ A Camera object to copy from.
	 */
	Camera (const Camera& other_);

	/**
	 * @since 0.0.0
	 */
	~Camera();

	/**
	 * A pointer to the Scene this camera belongs to.
	 *
	 * @since 0.0.0
	 */
	Scene *scene = nullptr;

	/**
	 * A pointer to the Game's SceneManager.
	 *
	 * @since 0.0.0
	 */
	Scenes::SceneManager *sceneManager = nullptr;

	/**
	 * A pointer to the Game's ScaleManager.
	 *
	 * @since 0.0.0
	 */
	Scale::ScaleManager *scaleManager = nullptr;

	/**
	 * A pointer to the Scene's CameraManager.
	 *
	 * @since 0.0.0
	 */
	Cameras::Scene2D::CameraManager *cameraManager = nullptr;

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
	 * The mask this Camera is using during render.
	 * Set the mask using the `setMask` method. Remove the mask using the 
	 * `clearMask` method.
	 *
	 * @since 0.0.0
	 */
	GameObjects::GameObject* mask = nullptr;

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
	 * @fn Camera::setAlpha (double value = 1)
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
	Camera& setOrigin (double x_, double y_);

	/**
	 * @overload
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The horizontal & vertical origin value.
	 *
	 * @return This Camera instance.
	 */
	Camera& setOrigin (double x_ = 0.5);

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
	Camera& centerOnX (int x_);

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
	Camera& centerOnY (int y_);

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
	Camera& centerOn (int x_, int y_);

	/**
	 * Moves the Camera so that it is looking at the center of the Camera Bounds, if enabled.
	 *
	 * @since 0.0.0
	 *
	 * @return This Camera instance.
	 */
	Camera& centerToBounds ();

	/**
	 * Moves the Camera so that it is re-centered based on its viewport size.
	 *
	 * @since 0.0.0
	 *
	 * @return This Camera instance.
	 */
	Camera& centerToSize ();

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
	Camera& ignore (GameObjects::GameObject* entry_);

	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param entries_ A vector of GameObjects, to be ignored by this Camera.
	 *
	 * @return This Camera instance.
	 */
	Camera& ignore (std::vector<GameObjects::GameObject*>& entries_);

	/**
	 * @overload
	 * @since 0.0.0
	 *
	 * @param entries_ A Group game object to be ignored by this Camera.
	 *
	 * @return This Camera instance.
	 */
	Camera& ignore (GameObjects::Group& entries_);

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
	Camera& removeBounds ();

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
	Camera& setAngle (double value_ = 0);

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
	Camera& setBackgroundColor (int color_);

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
	Camera& setBackgroundColor (int r_ = 0, int g_ = 0, int b_ = 0, int a_ = 0);

	/**
	 * @overload
	 *
	 * @since 0.0.0
	 *
	 * @param color_ The color object to use.
	 *
	 * @return This Camera instance.
	 */
	Camera& setBackgroundColor (Display::Color color_);

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
	Camera& setBounds (int x_, int y_, int width_, int height_, bool centerOn_ = false);

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
	Camera& setName (std::string value_ = "");

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
	Camera& setPosition (int x_, int  y_);

	/**
	 * @overload
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The top-left x coordinate of the Camera viewport. Will also set y to be the same as x.
	 *
	 * @return This Camera instance.
	 */
	Camera& setPosition (int x_);

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
	Camera& setRotation (double value_ = 0.0);

	/**
	 * Sets the Scene the Camera is bound to.
	 *
	 * @since 0.0.0
	 *
	 * @param scene_ The Scene the camera is bound to.
	 *
	 * @return This Camera instance.
	 */
	Camera& setScene (Scene* scene_);

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
	Camera& setScroll (int x_, int y_);

	/**
	 * @overload
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The x & y coordinate of the Camera in the game world.
	 *
	 * @return This Camera instance.
	 */
	Camera& setScroll (int x_);

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
	Camera& setSize (int width_, int height_);

	/**
	 * @overload
	 *
	 * @since 0.0.0
	 *
	 * @param size_ The width and height of the Camera viewport.
	 *
	 * @return This Camera instance.
	 */
	Camera& setSize (int size_);

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
	Camera& setViewport (int x_, int y_, int width_, int height_ = -1);

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
	Camera& setZoom (double x_ = 1.0, double y_ = 1.0);

	/**
	 * Sets the mask to be applied to this Camera during rendering.
	 *
	 * The mask is simply a GameObject.
	 *
	 * If a mask is already set on this Camera it will be immediately replaced.
	 *
	 * Masks have no impact on physics or input detection. They are purely a 
	 * rendering component that allows you to limit what is visible during the 
	 * render pass.
	 *
	 * @since 0.0.0
	 *
	 * @param mask_ The GameObject this Camera will use as a mask when rendering.
	 * @param fixedPosition_ Should the mask translate along with the Camera, or be fixed in place and not impacted by the Cameras transform?
	 *
	 * @return This Camera instance.
	 */
	Camera& setMask (GameObjects::GameObject& mask_, bool fixedPosition_ = true);

	/**
	 * Clears the mask that this Camera was using.
	 *
	 * @since 0.0.0
	 *
	 * @return This Camera instance.
	 */
	Camera& clearMask ();

	/**
	 * Sets the visibility of this Camera.
	 *
	 * An invisible Camera will skip rendering and input tests of everything it can see.
	 *
	 * @fn Camera::setVisible (bool value)
	 * @since 0.0.0
	 *
	 * @param value_ The visible state of the Camera.
	 *
	 * @return This Camera instance.
	 */

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
	 * @return This Camera instance.
	 */
	Camera& setX (int x_);

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
	 * @return This Camera instance.
	 */
	Camera& setY (int y_);

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
	 * @return This Camera instance.
	 */
	Camera& setWidth (int width_);

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
	 * @return This Camera instance.
	 */
	Camera& setHeight (int height_);

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
	 * @return This Camera instance.
	 */
	Camera& setScrollX (int value_ = 0);

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
	 * @return This Camera instance.
	 */
	Camera& setScrollY (int value_ = 0);

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
	 * Setter for private member Camera::zoomX.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ The value to set `zoomX` to.
	 *
	 * @return This Camera instance.
	 */
	Camera& setZoomX (double value_ = 1.0);

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
	 * @return This Camera instance.
	 */
	Camera& setZoomY (double value_ = 1.0);

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

    /**
     * Sets the Camera dead zone.
     *
     * The deadzone is only used when the camera is following a target.
     *
     * It defines a rectangular region within which if the target is present, the camera will not scroll.
     * If the target moves outside of this area, the camera will begin scrolling in order to follow it.
     *
     * The deadzone rectangle is re-positioned every frame so that it is centered on the mid-point
     * of the camera. This allows you to use the object for additional game related checks, such as
     * testing if an object is within it or not via a Rectangle.contains call.
     *
     * The `lerp` values that you can set for a follower target also apply when using a deadzone.
     *
     * Calling this method with no arguments will reset an active deadzone.
     *
     * @since 0.0.0
     *
     * @param width_ The width of the deadzone rectangle in pixels. If not specified the deadzone is removed.
     * @param height_ The height of the deadzone rectangle in pixels.
     *
     * @return This Camera instance.
     */
    Camera& setDeadzone (int width_ = -1, int height_ = -1);

    /**
     * Fades the Camera in from the given color over the duration specified.
     *
     * @since 0.0.0
     *
     * @param duration_ The duration of the effect in milliseconds.
     * @param red_ The amount to fade the red channel towards. A value between 0 and 255.
     * @param green_ The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue_ The amount to fade the blue channel towards. A value between 0 and 255.
     *
     * @return This Camera instance.
     */
    Camera& fadeIn (
			int duration_ = 1000, int red_ = 0, int green_ = 0, int blue_ = 0);

	/**
	 * @overload
     * @since 0.0.0
     *
     * @param duration_ The duration of the effect in milliseconds.
     * @param red_ The amount to fade the red channel towards. A value between 0 and 255.
     * @param green_ The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue_ The amount to fade the blue channel towards. A value between 0 and 255.
     * @param callback_ This callback will be invoked every frame for the duration of the effect.
     * It is sent two arguments: A reference to the camera and a progress amount between 0 and 1 indicating how complete the effect is.
     * @param context_ The context in which the callback is invoked.
     *
     * @return This Camera instance.
	 */
	template <typename T>
    Camera& fadeIn (int duration_, int red_, int green_, int blue_, void (T::* callback_)(Camera, double), T* context_)
	{
		std::function<void(Camera, double)> cb_ = std::bind(
				callback_,
				context_,
				std::placeholders::_1,
				std::placeholders::_2);

		return fadeEffect.start(false, duration_, red_, green_, blue_, true, cb_);
	}

    /**
     * Fades the Camera out to the given color over the duration specified.
     * This is an alias for Camera.fade that forces the fade to start, regardless of existing fades.
     *
     * @since 0.0.0
     *
     * @param duration_ The duration of the effect in milliseconds.
     * @param red_ The amount to fade the red channel towards. A value between 0 and 255.
     * @param green_ The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue_ The amount to fade the blue channel towards. A value between 0 and 255.
     *
     * @return This Camera instance.
     */
    Camera& fadeOut (int duration_ = 1000, int red_ = 0, int green_ = 0, int blue_ = 0);

	/**
	 * @overload
     * @since 0.0.0
     *
     * @param duration_ The duration of the effect in milliseconds.
     * @param red_ The amount to fade the red channel towards. A value between 0 and 255.
     * @param green_ The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue_ The amount to fade the blue channel towards. A value between 0 and 255.
     * @param callback_ This callback will be invoked every frame for the duration of the effect.
     * It is sent two arguments: A reference to the camera and a progress amount between 0 and 1 indicating how complete the effect is.
     * @param context_ The context in which the callback is invoked.
     *
     * @return This Camera instance.
	 */
	template <typename T>
    Camera& fadeOut (int duration_, int red_, int green_, int blue_, void (T::* callback_)(Camera, double), T* context_)
	{
		std::function<void(Camera, double)> cb_ = std::bind(
				callback_,
				context_,
				std::placeholders::_1,
				std::placeholders::_2);

		return fadeEffect.start(true, duration_, red_, green_, blue_, true, cb_);
	}

    /**
     * Fades the Camera from the given color to transparent over the duration specified.
     *
     * @since 0.0.0
     *
     * @param duration_ The duration of the effect in milliseconds.
     * @param red_ The amount to fade the red channel towards. A value between 0 and 255.
     * @param green_ The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue_ The amount to fade the blue channel towards. A value between 0 and 255.
     * @param force_ Force the effect to start immediately, even if already running.
     *
     * @return This Camera instance.
     */
    Camera& fadeFrom (
			int duration_ = 1000,
			int red_ = 0,
			int green_ = 0,
			int blue_ = 0,
			bool force_ = false);

	/**
	 * @overload
	 *
     * @since 0.0.0
     *
     * @param duration_ The duration of the effect in milliseconds.
     * @param red_ The amount to fade the red channel towards. A value between 0 and 255.
     * @param green_ The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue_ The amount to fade the blue channel towards. A value between 0 and 255.
     * @param force_ Force the effect to start immediately, even if already running.
     * @param callback_ This callback will be invoked every frame for the duration of the effect.
     * It is sent two arguments: A reference to the camera and a progress amount between 0 and 1 indicating how complete the effect is.
     * @param context_ The context in which the callback is invoked.
     *
     * @return This Camera instance.
	 */
	template <typename T>
    Camera& fadeFrom (int duration_, int red_, int green_, int blue_, bool force_, void (T::* callback_)(Camera, double), T* context_)
	{
		std::function<void(Camera, double)> cb_ = std::bind(
				callback_,
				context_,
				std::placeholders::_1,
				std::placeholders::_2);

		return fadeEffect.start(false, duration_, red_, green_, blue_, force_, cb_);
	}

    /**
     * Fades the Camera from transparent to the given color over the duration specified.
     *
     * @since 0.0.0
     *
     * @param duration_ The duration of the effect in milliseconds.
     * @param red_ The amount to fade the red channel towards. A value between 0 and 255.
     * @param green_ The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue_ The amount to fade the blue channel towards. A value between 0 and 255.
     * @param force_ Force the effect to start immediately, even if already running.
     *
     * @return This Camera instance.
     */
    Camera& fade (int duration_ = 1000, int red_ = 0, int green_ = 0, int blue_ = 0, bool force_ = false);

	/**
	 * @overload
	 *
     * @since 0.0.0
     *
     * @param duration_ The duration of the effect in milliseconds.
     * @param red_ The amount to fade the red channel towards. A value between 0 and 255.
     * @param green_ The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue_ The amount to fade the blue channel towards. A value between 0 and 255.
     * @param force_ Force the effect to start immediately, even if already running.
     * @param callback_ This callback will be invoked every frame for the duration of the effect.
     * It is sent two arguments: A reference to the camera and a progress amount between 0 and 1 indicating how complete the effect is.
     * @param context_ The context in which the callback is invoked.
     *
     * @return This Camera instance.
	 */
	template <typename T>
    Camera& fade (int duration_, int red_, int green_, int blue_, bool force_, void (T::* callback_)(Camera, double), T* context_)
	{
		std::function<void(Camera, double)> cb_ = std::bind(
				callback_,
				context_,
				std::placeholders::_1,
				std::placeholders::_2);

		return fadeEffect.start(true, duration_, red_, green_, blue_, force_, cb_);
	}

    /**
     * Flashes the Camera by setting it to the given color immediately and then fading it away again quickly over the duration specified.
     *
     * @since 0.0.0
     *
     * @param duration_ The duration of the effect in milliseconds.
     * @param red_ The amount to fade the red channel towards. A value between 0 and 255.
     * @param green_ The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue_ The amount to fade the blue channel towards. A value between 0 and 255.
     * @param force_ Force the effect to start immediately, even if already running.
     *
     * @return This Camera instance.
     */
    Camera& flash (int duration_ = 250, int red_ = 0, int green_ = 0, int blue_ = 0, bool force_ = false);

	/**
	 * @overload
	 *
     * @since 0.0.0
     *
     * @param duration_ The duration of the effect in milliseconds.
     * @param red_ The amount to fade the red channel towards. A value between 0 and 255.
     * @param green_ The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue_ The amount to fade the blue channel towards. A value between 0 and 255.
     * @param force_ Force the effect to start immediately, even if already running.
     * @param callback_ This callback will be invoked every frame for the duration of the effect.
     * It is sent two arguments: A reference to the camera and a progress amount between 0 and 1 indicating how complete the effect is.
     * @param context_ The context in which the callback is invoked.
     *
     * @return This Camera instance.
	 */
	template <typename T>
    Camera& flash (int duration_, int red_, int green_, int blue_, bool force_, void (T::* callback_)(Camera, double), T* context_)
	{
		std::function<void(Camera, double)> cb_ = std::bind(
				callback_,
				context_,
				std::placeholders::_1,
				std::placeholders::_2);

		return flashEffect.start(duration_, red_, green_, blue_, force_, cb_);
	}

    /**
     * Shakes the Camera by the given intensity over the duration specified.
     *
     * @since 0.0.0
     *
     * @param duration_ The duration of the effect in milliseconds.
     * @param intensity_ The intensity of the shake.
     * @param force_ Force the shake effect to start immediately, even if already running.
     *
     * @return This Camera instance.
     */
    Camera& shake (int duration_ = 100, Math::Vector2 intensity_ = Math::Vector2 (0.05), bool force_ = false);

	/**
	 * @overload
	 *
     * @since 0.0.0
     *
     * @param duration_ The duration of the effect in milliseconds.
     * @param intensity_ The intensity of the shake.
     * @param force_ Force the effect to start immediately, even if already running.
     * @param callback_ This callback will be invoked every frame for the duration of the effect.
     * It is sent two arguments: A reference to the camera and a progress amount between 0 and 1 indicating how complete the effect is.
     * @param context_ The context in which the callback is invoked.
     *
     * @return This Camera instance.
	 */
	template <typename T>
    Camera& shake (int duration_, Math::Vector2 intensity_, bool force_, void (T::* callback_)(Camera, double), T* context_)
	{
		std::function<void(Camera, double)> cb_ = std::bind(
				callback_,
				context_,
				std::placeholders::_1,
				std::placeholders::_2);

		return shakeEffect.start(duration_, intensity_, force_, cb_);
	}

    /**
     * This effect will scroll the Camera so that the center of its viewport finishes at the given destination,
     * over the duration and with the ease specified.
     *
     * @since 0.0.0
     *
     * @param x_ The destination x coordinate to scroll the center of the Camera viewport to.
     * @param y_ The destination y coordinate to scroll the center of the Camera viewport to.
     * @param duration_ The duration of the effect in milliseconds.
     * @param ease_ The ease to use for the pan.
     * @param force_ Force the pan effect to start immediately, even if already running.
     *
     * @return This Camera instance.
     */
    Camera& pan (int x_, int y_, int duration_ = 1000, std::string ease_ = "Linear", bool force_ = false);

    /**
	 * @overload
     *
     * @since 0.0.0
     *
     * @param x_ The destination x coordinate to scroll the center of the Camera viewport to.
     * @param y_ The destination y coordinate to scroll the center of the Camera viewport to.
     * @param duration_ The duration of the effect in milliseconds.
     * @param ease_ The ease to use for the pan.
     * @param force_ Force the pan effect to start immediately, even if already running.
     * @param callback_ This callback will be invoked every frame for the duration of the effect.
     * It is sent four arguments: A reference to the camera, a progress amount between 0 and 1 indicating how complete the effect is,
     * the current camera scroll x coordinate and the current camera scroll y coordinate.
     * @param context_ The context in which the callback is invoked.
     *
     * @return This Camera instance.
     */
	template <typename T>
    Camera& pan (int x_, int y_, int duration_, std::string ease_, bool force_, void (T::* callback_)(Camera, double, int, int), T* context_)
	{
		std::function<void(Camera, double, int, int)> cb_ = std::bind(
				callback_,
				context_,
				std::placeholders::_1,
				std::placeholders::_2,
				std::placeholders::_3,
				std::placeholders::_4);

		return panEffect.start(x_, y_, duration_, ease_, force_, cb_);
	}

    /**
     * This effect will rotate the Camera so that the viewport finishes at the given angle in radians,
     * over the duration and with the ease specified.
     *
     * @since 0.0.0
     *
     * @param radians_ The destination angle in radians to rotate the Camera viewport to. If the angle is positive then the rotation is clockwise else anticlockwise
     * @param shortestPath_ If shortest path is set to true the camera will rotate in the quickest direction clockwise or anti-clockwise.
     * @param duration_ - The duration of the effect in milliseconds.
     * @param ease_ The ease to use for the rotation.
     * @param force_ Force the rotation effect to start immediately, even if already running.
     *
     * @return This Camera instance.
     */
    Camera& rotateTo (
			double radians_,
			bool shortestPath_ = false,
			int duration_ = 1000,
			std::string ease_ = "Linear",
			bool force_ = false);

    /**
	 * @overload
     *
     * @since 0.0.0
     *
     * @param radians_ The destination angle in radians to rotate the Camera viewport to. If the angle is positive then the rotation is clockwise else anticlockwise
     * @param shortestPath_ If shortest path is set to true the camera will rotate in the quickest direction clockwise or anti-clockwise.
     * @param duration_ - The duration of the effect in milliseconds.
     * @param ease_ The ease to use for the rotation.
     * @param force_ Force the rotation effect to start immediately, even if already running.
     * @param callback_ This callback will be invoked every frame for the duration of the effect.
     * It is sent three arguments: A reference to the camera, a progress amount between 0 and 1 indicating how complete the effect is,
     * the current camera rotation angle in radians.
     * @param context_ The context in which the callback is invoked.
     *
     * @return This Camera instance.
     */
	template <typename T>
    Camera& rotateTo (double radians_, bool shortestPath_, int duration_, std::string ease_, bool force_, void (T::* callback_)(Camera, double, double), T* context_)
	{
		std::function<void(Camera, double, double)> cb_ = std::bind(
				callback_,
				context_,
				std::placeholders::_1,
				std::placeholders::_2,
				std::placeholders::_3);

        return rotateToEffect.start(radians_, shortestPath_, duration_, ease_, force_, cb_);
	}

    /**
     * This effect will zoom the Camera to the given scale, over the duration and with the ease specified.
     *
     * @since 0.0.0
     *
     * @param zoom_ The target Camera zoom value.
     * @param duration_ The duration of the effect in milliseconds.
     * @param ease_ The ease to use for the pan.
     * @param force_ Force the pan effect to start immediately, even if already running.
     *
     * @return This Camera instance.
     */
    Camera& zoomTo (double zoom_, int duration_, std::string ease_, bool force_);

    /**
     * @overload
     *
     * @since 0.0.0
     *
     * @param zoom_ The target Camera zoom value.
     * @param duration_ The duration of the effect in milliseconds.
     * @param ease_ The ease to use for the pan.
     * @param force_ Force the pan effect to start immediately, even if already running.
     * @param callback_ - This callback will be invoked every frame for the duration of the effect.
     * It is sent four arguments: A reference to the camera, a progress amount between 0 and 1 indicating how complete the effect is,
     * the current camera scroll x coordinate and the current camera scroll y coordinate.
     * @param context_ - The context in which the callback is invoked. Defaults to the Scene to which the Camera belongs.
     *
     * @return This Camera instance.
     */
	template <typename T>
    Camera& zoomTo (double zoom_, int duration_, std::string ease_, bool force_, void (T::* callback_)(Camera, double, int, int), T* context_)
	{
		std::function<void(Camera, double, double)> cb_ = std::bind(
				callback_,
				context_,
				std::placeholders::_1,
				std::placeholders::_2,
				std::placeholders::_3,
				std::placeholders::_4);

		return zoomEffect.start(zoom_, duration_, ease_, force_, cb_);
	}

    /**
     * Sets the linear interpolation value to use when following a target.
     *
     * The default values of 1 means the camera will instantly snap to the target coordinates.
     * A lower value, such as 0.1 means the camera will more slowly track the target, giving
     * a smooth transition. You can set the horizontal and vertical values independently, and also
     * adjust this value in real-time during your game.
     *
     * Be sure to keep the value between 0 and 1. A value of zero will disable tracking on that axis.
     *
     * @since 0.0.0
     *
     * @param x_ The amount added to the horizontal linear interpolation of the follow target.
     * @param y_ The amount added to the vertical linear interpolation of the follow target.
     *
     * @return This Camera instance.
     */
    Camera& setLerp (double x_ = 1.0, double y_ = 1.0);

    /**
     * Sets the horizontal and vertical offset of the camera from its follow target.
     * The values are subtracted from the targets position during the Cameras update step.
     *
     * @since 0.0.0
     *
     * @param x_ The horizontal offset from the camera follow target.x position.
     * @param y_ The vertical offset from the camera follow target.y position.
     *
     * @return This Camera instance.
     */
    Camera& setFollowOffset (int x_ = 0, int y_ = 0);

    /**
     * Sets the Camera to follow a Game Object.
     *
     * When enabled the Camera will automatically adjust its scroll position to keep the target Game Object
     * in its center.
     *
     * You can set the linear interpolation value used in the follow code.
     * Use low lerp values (such as 0.1) to automatically smooth the camera motion.
     *
     * If you find you're getting a slight "jitter" effect when following an object it's probably to do with sub-pixel
     * rendering of the targets position. This can happen if you have specified a non-integer zoom
     * value on the camera. So be sure to keep the camera zoom to integers.
     *
     * @since 0.0.0
     *
     * @param target_ - The target for the Camera to follow.
     * @param lerpX_ A value between 0 and 1. This value specifies the amount of linear interpolation to use when horizontally tracking the target. The closer the value to 1, the faster the camera will track.
     * @param lerpY_ A value between 0 and 1. This value specifies the amount of linear interpolation to use when vertically tracking the target. The closer the value to 1, the faster the camera will track.
     * @param offsetX_ The horizontal offset from the camera follow target.x position.
     * @param offsetY_ The vertical offset from the camera follow target.y position.
     *
     * @return This Camera instance.
     */
    Camera& startFollow (GameObjects::GameObject& target_, double lerpX_ = 1.0, double lerpY_ = 1.0, int offsetX_ = 0, int offsetY_ = 0);

    /**
     * Stops a Camera from following a Game Object, if previously set via `Camera.startFollow`.
     *
     * @since 0.0.0
     *
     * @return This Camera instance.
     */
    Camera& stopFollow ();

    /**
     * Resets any active FX, such as a fade, flash or shake. Useful to call after a fade in order to
     * remove the fade.
     *
     * @since 0.0.0
     *
     * @return This Camera instance.
     */
    Camera& resetFX ();

    /**
     * Internal method called automatically by the Camera Manager.
     *
     * @since 0.0.0
     *
     * @param time_ The amount of time since SDL started in ms.
     * @param delta_ The delta time, in ms, elapsed since the last frame.
     */
    void update (Uint32 time_, Uint32 delta_);
};

}	// namespace Scene2D
}	// namespace Cameras
}	// namespace Zen

// Declaration of forward declared elements
#include "camera_manager.h"
//#include "../../scene/scene.h"

#endif
