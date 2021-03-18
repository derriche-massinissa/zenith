/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_CAMERAS_SCENE2D_CAMERA_H
#define ZEN_CAMERAS_SCENE2D_CAMERA_H

#include <functional>
#include <string>
#include <memory>

#include "base_camera.h"

namespace Zen {
namespace Cameras {
namespace Scene2D {

/**
 * @todo TODO camera effects
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
 * A Camera also has built-in special effects including Fade, Flash and Camera Shake.
 *
 * @class Camera
 *
 * @since 0.0.0
 *
 * @param x_ The x position of the Camera, relative to the top-left of the game window.
 * @param y_ The y position of the Camera, relative to the top-left of the game window.
 * @param width_ The width of the Camera, in pixels.
 * @param height_ The height of the Camera, in pixels.
 */
class Camera
	: public BaseCamera
	, public GameObjects::Components::Flip<Camera>
	, public GameObjects::Components::Tint<Camera>
{
public:
	/**
	 * This mask is used to determine what GameObject compenents this class inherits from.
	 *
	 * It is a bitmask.
	 *
	 * @since 0.0.0
	 */
	const int COMPONENT_MASK = BaseCamera::COMPONENT_MASK | COMPONENT_MASK_FLIP | COMPONENT_MASK_TINT;

	/**
	 * Returns the object's Component Bitmask. This method is used to
	 * override the virtual version of the ancestors.
	 *
	 * @since 0.0.0
	 *
	 * @return The object's Component Bitmask.
	 */
	int getComponentMask ();

	/**
	 * @since 0.0.0
	 */
	Camera (int x_, int y_, int width_, int height_);

	/**
	 * @since 0.0.0
	 */
	~Camera ();

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
	Effect::Shake shakeEffect;

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
	Math::Vector2 lerp (1.0, 1.0);

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
    Camera& fadeIn (int duration_, int red_, int green_, int blue_, void (T::* callback_)(Camera&, double), T* context_)
	{
		std::function<void(Camera&, double)> cb_ = std::bind(
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
    Camera& fadeOut (int duration_, int red_, int green_, int blue_, void (T::* callback_)(Camera&, double), T* context_)
	{
		std::function<void(Camera&, double)> cb_ = std::bind(
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
    Camera& fadeFrom (int duration_, int red_, int green_, int blue_, bool force_, void (T::* callback_)(Camera&, double), T* context_)
	{
		std::function<void(Camera&, double)> cb_ = std::bind(
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
    Camera& fade (int duration_, int red_, int green_, int blue_, bool force_, void (T::* callback_)(Camera&, double), T* context_)
	{
		std::function<void(Camera&, double)> cb_ = std::bind(
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
    Camera& flash (int duration_, int red_, int green_, int blue_, bool force_, void (T::* callback_)(Camera&, double), T* context_)
	{
		std::function<void(Camera&, double)> cb_ = std::bind(
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
    Camera& shake (int duration_ = 100, Math::Vector2 intensity_ = {0.05}, bool force_ = false);

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
    Camera& shake (int duration_, Math::Vector2 intensity_, bool force_, void (T::* callback_)(Camera&, double), T* context_)
	{
		std::function<void(Camera&, double)> cb_ = std::bind(
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
    Camera& pan (int x_, int y_, int duration_, std::string ease_, bool force_, void (T::* callback_)(Camera&, double, int, int), T* context_)
	{
		std::function<void(Camera&, double, int, int)> cb_ = std::bind(
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
    Camera& rotateTo (double radians_, bool shortestPath_, int duration_, std::string ease_, bool force_, void (T::* callback_)(Camera&, double, double), T* context_)
	{
		std::function<void(Camera&, double, double)> cb_ = std::bind(
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
    Camera& zoomTo (double zoom_, int duration_, std::string ease_, bool force_, void (T::* callback_)(Camera&, double, int, int), T* context_)
	{
		std::function<void(Camera&, double, int, int)> cb_ = std::bind(
				callback_,
				context_,
				std::placeholders::_1,
				std::placeholders::_2,
				std::placeholders::_3,
				std::placeholders::_4);

		return zoomEffect.start(zoom_, duration_, ease_, force_, cb_);
	}

    /**
     * Internal preRender step.
     *
     * @since 0.0.0
     */
    void preRender ();

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

private:
	/**
	 * Internal follow target reference.
	 *
	 * @since 0.0.0
	 */
	GameObjects::GameObject* follow = nullptr;
};

}	// namespace Scene2D
}	// namespace Cameras
}	// namespace Zen

#endif
