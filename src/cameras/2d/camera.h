/**
 * @file		camera.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "base_camera.h"
#include <functional>
#include <string>
#include <memory>

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
 * A Camera also has built-in special effects including Fade, Flash and Camera Shake.
 *
 * @class Camera
 *
 * @since 0.0.0
 *
 * @param x The x position of the Camera, relative to the top-left of the game window.
 * @param y The y position of the Camera, relative to the top-left of the game window.
 * @param width The width of the Camera, in pixels.
 * @param height The height of the Camera, in pixels.
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
	Camera (int x, int y, int width, int height);

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
	Effects::Fade fadeEffect (*this);

	/**
	 * The Camera Flash effect handler.
	 * To flash this camera see the `Camera.flash` method.
	 *
	 * @since 0.0.0
	 */
	Effects::Flash flashEffect (*this);

	/**
	 * The Camera Shake effect handler.
	 * To shake this camera see the `Camera.shake` method.
	 *
	 * @since 0.0.0
	 */
	Effect::Shake shakeEffect (*this);

	/**
	 * The Camera Pan effect handler.
	 * To pan this camera see the `Camera.pan` method.
	 *
	 * @since 0.0.0
	 */
	Effects::Pan panEffect (*this);

	/**
	 * The Camera Rotate To effect handler.
	 * To rotate this camera see the `Camera.rotateTo` method.
	 *
	 * @since 0.0.0
	 */
	Effects::RotateTo rotateToEffect (*this);

	/**
	 * The Camera Zoom effect handler.
	 * To zoom this camera see the `Camera.zoom` method.
	 *
	 * @since 0.0.0
	 */
	Effects::Zoom zoomEffect (*this);

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
	Math::Vector2 lerp_ (1.0, 1.0);

	/**
	 * The values stored in this property are subtracted from the Camera targets position, allowing you to
	 * offset the camera from the actual target x/y coordinates by this amount.
	 * Can also be set via `setFollowOffset` or as part of the `startFollow` call.
	 *
	 * @since 0.0.0
	 */
	Math::Vector2 followOffset_;

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
	std::shared_ptr<Geom::Rectangle> deadzone_;

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
     * @param width The width of the deadzone rectangle in pixels. If not specified the deadzone is removed.
     * @param height The height of the deadzone rectangle in pixels.
     *
     * @return This Camera instance.
     */
    Camera& setDeadzone (int width = -1, int height = -1);

    /**
     * Fades the Camera in from the given color over the duration specified.
     *
     * @since 0.0.0
     *
     * @param duration The duration of the effect in milliseconds.
     * @param red The amount to fade the red channel towards. A value between 0 and 255.
     * @param green The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue The amount to fade the blue channel towards. A value between 0 and 255.
     *
     * @return This Camera instance.
     */
    Camera& fadeIn (int duration = 1000, int red = 0, int green = 0, int blue = 0)
	{
		return fadeEffect.start(false, duration, red, green, blue, true);
	}

	/**
	 * @overload
	 *
     * @since 0.0.0
     *
     * @param duration The duration of the effect in milliseconds.
     * @param red The amount to fade the red channel towards. A value between 0 and 255.
     * @param green The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue The amount to fade the blue channel towards. A value between 0 and 255.
     * @param callback This callback will be invoked every frame for the duration of the effect.
     * It is sent two arguments: A reference to the camera and a progress amount between 0 and 1 indicating how complete the effect is.
     * @param context The context in which the callback is invoked.
     *
     * @return This Camera instance.
	 */
	template <typename T>
    Camera& fadeIn (int duration, int red, int green, int blue, void (T::* callback)(Camera&, float), T* context)
	{
		std::function<void(Camera&, float)> cb = std::bind(
				callback,
				context,
				std::placeholders::_1,
				std::placeholders::_2);

		return fadeEffect.start(false, duration, red, green, blue, true, cb);
	}

    /**
     * Fades the Camera out to the given color over the duration specified.
     * This is an alias for Camera.fade that forces the fade to start, regardless of existing fades.
     *
     * @since 0.0.0
     *
     * @param duration The duration of the effect in milliseconds.
     * @param red The amount to fade the red channel towards. A value between 0 and 255.
     * @param green The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue The amount to fade the blue channel towards. A value between 0 and 255.
     *
     * @return This Camera instance.
     */
    Camera& fadeOut (int duration = 1000, int red = 0, int green = 0, int blue = 0)
	{
		return fadeEffect.start(true, duration, red, green, blue, true);
	}

	/**
	 * @overload
	 *
     * @since 0.0.0
     *
     * @param duration The duration of the effect in milliseconds.
     * @param red The amount to fade the red channel towards. A value between 0 and 255.
     * @param green The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue The amount to fade the blue channel towards. A value between 0 and 255.
     * @param callback This callback will be invoked every frame for the duration of the effect.
     * It is sent two arguments: A reference to the camera and a progress amount between 0 and 1 indicating how complete the effect is.
     * @param context The context in which the callback is invoked.
     *
     * @return This Camera instance.
	 */
	template <typename T>
    Camera& fadeOut (int duration, int red, int green, int blue, void (T::* callback)(Camera&, float), T* context)
	{
		std::function<void(Camera&, float)> cb = std::bind(
				callback,
				context,
				std::placeholders::_1,
				std::placeholders::_2);

		return fadeEffect.start(true, duration, red, green, blue, true, cb);
	}

    /**
     * Fades the Camera from the given color to transparent over the duration specified.
     *
     * @since 0.0.0
     *
     * @param duration The duration of the effect in milliseconds.
     * @param red The amount to fade the red channel towards. A value between 0 and 255.
     * @param green The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue The amount to fade the blue channel towards. A value between 0 and 255.
     * @param force Force the effect to start immediately, even if already running.
     *
     * @return This Camera instance.
     */
    Camera& fadeFrom (int duration = 1000, int red = 0, int green = 0, int blue = 0, bool force = false)
	{
		return fadeEffect.start(false, duration, red, green, blue, force);
	}

	/**
	 * @overload
	 *
     * @since 0.0.0
     *
     * @param duration The duration of the effect in milliseconds.
     * @param red The amount to fade the red channel towards. A value between 0 and 255.
     * @param green The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue The amount to fade the blue channel towards. A value between 0 and 255.
     * @param force Force the effect to start immediately, even if already running.
     * @param callback This callback will be invoked every frame for the duration of the effect.
     * It is sent two arguments: A reference to the camera and a progress amount between 0 and 1 indicating how complete the effect is.
     * @param context The context in which the callback is invoked.
     *
     * @return This Camera instance.
	 */
	template <typename T>
    Camera& fadeFrom (int duration, int red, int green, int blue, bool force, void (T::* callback)(Camera&, float), T* context)
	{
		std::function<void(Camera&, float)> cb = std::bind(
				callback,
				context,
				std::placeholders::_1,
				std::placeholders::_2);

		return fadeEffect.start(false, duration, red, green, blue, force, cb);
	}

    /**
     * Fades the Camera from transparent to the given color over the duration specified.
     *
     * @since 0.0.0
     *
     * @param duration The duration of the effect in milliseconds.
     * @param red The amount to fade the red channel towards. A value between 0 and 255.
     * @param green The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue The amount to fade the blue channel towards. A value between 0 and 255.
     * @param force Force the effect to start immediately, even if already running.
     *
     * @return This Camera instance.
     */
    Camera& fade (int duration = 1000, int red = 0, int green = 0, int blue = 0, bool force = false)
	{
		return fadeEffect.start(true, duration, red, green, blue, force);
	}

	/**
	 * @overload
	 *
     * @since 0.0.0
     *
     * @param duration The duration of the effect in milliseconds.
     * @param red The amount to fade the red channel towards. A value between 0 and 255.
     * @param green The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue The amount to fade the blue channel towards. A value between 0 and 255.
     * @param force Force the effect to start immediately, even if already running.
     * @param callback This callback will be invoked every frame for the duration of the effect.
     * It is sent two arguments: A reference to the camera and a progress amount between 0 and 1 indicating how complete the effect is.
     * @param context The context in which the callback is invoked.
     *
     * @return This Camera instance.
	 */
	template <typename T>
    Camera& fade (int duration, int red, int green, int blue, bool force, void (T::* callback)(Camera&, float), T* context)
	{
		std::function<void(Camera&, float)> cb = std::bind(
				callback,
				context,
				std::placeholders::_1,
				std::placeholders::_2);

		return fadeEffect.start(true, duration, red, green, blue, force, cb);
	}

    /**
     * Flashes the Camera by setting it to the given color immediately and then fading it away again quickly over the duration specified.
     *
     * @since 0.0.0
     *
     * @param duration The duration of the effect in milliseconds.
     * @param red The amount to fade the red channel towards. A value between 0 and 255.
     * @param green The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue The amount to fade the blue channel towards. A value between 0 and 255.
     * @param force Force the effect to start immediately, even if already running.
     *
     * @return This Camera instance.
     */
    Camera& flash (int duration = 250, int red = 0, int green = 0, int blue = 0, bool force = false)
	{
		return flashEffect.start(duration, red, green, blue, force);
	}

	/**
	 * @overload
	 *
     * @since 0.0.0
     *
     * @param duration The duration of the effect in milliseconds.
     * @param red The amount to fade the red channel towards. A value between 0 and 255.
     * @param green The amount to fade the green channel towards. A value between 0 and 255.
     * @param blue The amount to fade the blue channel towards. A value between 0 and 255.
     * @param force Force the effect to start immediately, even if already running.
     * @param callback This callback will be invoked every frame for the duration of the effect.
     * It is sent two arguments: A reference to the camera and a progress amount between 0 and 1 indicating how complete the effect is.
     * @param context The context in which the callback is invoked.
     *
     * @return This Camera instance.
	 */
	template <typename T>
    Camera& flash (int duration, int red, int green, int blue, bool force, void (T::* callback)(Camera&, float), T* context)
	{
		std::function<void(Camera&, float)> cb = std::bind(
				callback,
				context,
				std::placeholders::_1,
				std::placeholders::_2);

		return flashEffect.start(duration, red, green, blue, force, cb);
	}

    /**
     * Shakes the Camera by the given intensity over the duration specified.
     *
     * @since 0.0.0
     *
     * @param duration The duration of the effect in milliseconds.
     * @param intensity The intensity of the shake.
     * @param force Force the shake effect to start immediately, even if already running.
     *
     * @return This Camera instance.
     */
    Camera& shake (int duration = 100, Math::Vector2 intensity = {0.05}, bool force = false)
	{
		return shakeEffect.start(duration, intensity, force);
	}

	/**
	 * @overload
	 *
     * @since 0.0.0
     *
     * @param duration The duration of the effect in milliseconds.
     * @param intensity The intensity of the shake.
     * @param force Force the effect to start immediately, even if already running.
     * @param callback This callback will be invoked every frame for the duration of the effect.
     * It is sent two arguments: A reference to the camera and a progress amount between 0 and 1 indicating how complete the effect is.
     * @param context The context in which the callback is invoked.
     *
     * @return This Camera instance.
	 */
	template <typename T>
    Camera& shake (int duration, Math::Vector2 intensity, bool force, void (T::* callback)(Camera&, float), T* context)
	{
		std::function<void(Camera&, float)> cb = std::bind(
				callback,
				context,
				std::placeholders::_1,
				std::placeholders::_2);

		return shakeEffect.start(duration, intensity, force, cb);
	}

    /**
     * This effect will scroll the Camera so that the center of its viewport finishes at the given destination,
     * over the duration and with the ease specified.
     *
     * @since 0.0.0
     *
     * @param x The destination x coordinate to scroll the center of the Camera viewport to.
     * @param y The destination y coordinate to scroll the center of the Camera viewport to.
     * @param duration The duration of the effect in milliseconds.
     * @param ease The ease to use for the pan.
     * @param force Force the pan effect to start immediately, even if already running.
     *
     * @return This Camera instance.
     */
    Camera& pan (int x, int y, int duration = 1000, std::string ease = "Linear", bool force = false)
	{
		return panEffect.start(x, y, duration, ease, force);
	}

    /**
	 * @overload
     *
     * @since 0.0.0
     *
     * @param x The destination x coordinate to scroll the center of the Camera viewport to.
     * @param y The destination y coordinate to scroll the center of the Camera viewport to.
     * @param duration The duration of the effect in milliseconds.
     * @param ease The ease to use for the pan.
     * @param force Force the pan effect to start immediately, even if already running.
     * @param callback This callback will be invoked every frame for the duration of the effect.
     * It is sent four arguments: A reference to the camera, a progress amount between 0 and 1 indicating how complete the effect is,
     * the current camera scroll x coordinate and the current camera scroll y coordinate.
     * @param context The context in which the callback is invoked.
     *
     * @return This Camera instance.
     */
	template <typename T>
    Camera& pan (int x, int y, int duration, std::string ease, bool force, void (T::* callback)(Camera&, float, int, int), T* context)
	{
		std::function<void(Camera&, float, int, int)> cb = std::bind(
				callback,
				context,
				std::placeholders::_1,
				std::placeholders::_2,
				std::placeholders::_3,
				std::placeholders::_4);

		return panEffect.start(x, y, duration, ease, force, cb);
	}

    /**
     * This effect will rotate the Camera so that the viewport finishes at the given angle in radians,
     * over the duration and with the ease specified.
     *
     * @since 0.0.0
     *
     * @param radians The destination angle in radians to rotate the Camera viewport to. If the angle is positive then the rotation is clockwise else anticlockwise
     * @param shortestPath If shortest path is set to true the camera will rotate in the quickest direction clockwise or anti-clockwise.
     * @param duration - The duration of the effect in milliseconds.
     * @param ease The ease to use for the rotation.
     * @param force Force the rotation effect to start immediately, even if already running.
     *
     * @return This Camera instance.
     */
    Camera& rotateTo (float radians, bool shortestPath = false, int duration = 1000, std::string ease = "Linear", bool force = false)
    {
        return rotateToEffect.start(radians, shortestPath, duration, ease, force);
    }

    /**
	 * @overload
     *
     * @since 0.0.0
     *
     * @param radians The destination angle in radians to rotate the Camera viewport to. If the angle is positive then the rotation is clockwise else anticlockwise
     * @param shortestPath If shortest path is set to true the camera will rotate in the quickest direction clockwise or anti-clockwise.
     * @param duration - The duration of the effect in milliseconds.
     * @param ease The ease to use for the rotation.
     * @param force Force the rotation effect to start immediately, even if already running.
     * @param callback This callback will be invoked every frame for the duration of the effect.
     * It is sent three arguments: A reference to the camera, a progress amount between 0 and 1 indicating how complete the effect is,
     * the current camera rotation angle in radians.
     * @param context The context in which the callback is invoked.
     *
     * @return This Camera instance.
     */
	template <typename T>
    Camera& rotateTo (float radians, bool shortestPath, int duration, std::string ease, bool force, void (T::* callback)(Camera&, float, float), T* context)
	{
		std::function<void(Camera&, float, float)> cb = std::bind(
				callback,
				context,
				std::placeholders::_1,
				std::placeholders::_2,
				std::placeholders::_3);

        return rotateToEffect.start(radians, shortestPath, duration, ease, force, cb);
	}

    /**
     * This effect will zoom the Camera to the given scale, over the duration and with the ease specified.
     *
     * @since 0.0.0
     *
     * @param zoom The target Camera zoom value.
     * @param duration The duration of the effect in milliseconds.
     * @param ease The ease to use for the pan.
     * @param force Force the pan effect to start immediately, even if already running.
     *
     * @return This Camera instance.
     */
    Camera& zoomTo (float zoom, int duration, std::string ease, bool force)
	{
		return zoomEffect.start(zoom, duration, ease, force);
	}

    /**
     * @overload
     *
     * @since 0.0.0
     *
     * @param zoom The target Camera zoom value.
     * @param duration The duration of the effect in milliseconds.
     * @param ease The ease to use for the pan.
     * @param force Force the pan effect to start immediately, even if already running.
     * @param callback - This callback will be invoked every frame for the duration of the effect.
     * It is sent four arguments: A reference to the camera, a progress amount between 0 and 1 indicating how complete the effect is,
     * the current camera scroll x coordinate and the current camera scroll y coordinate.
     * @param context - The context in which the callback is invoked. Defaults to the Scene to which the Camera belongs.
     *
     * @return This Camera instance.
     */
	template <typename T>
    Camera& zoomTo (float zoom, int duration, std::string ease, bool force, void (T::* callback)(Camera&, float, int, int), T* context)
	{
		std::function<void(Camera&, float, int, int)> cb = std::bind(
				callback,
				context,
				std::placeholders::_1,
				std::placeholders::_2,
				std::placeholders::_3,
				std::placeholders::_4);

		return zoomEffect.start(zoom, duration, ease, force, cb);
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
     * @param x The amount added to the horizontal linear interpolation of the follow target.
     * @param y The amount added to the vertical linear interpolation of the follow target.
     *
     * @return This Camera instance.
     */
    Camera& setLerp (float x = 1.0, float y = 1.0);

    /**
     * Sets the horizontal and vertical offset of the camera from its follow target.
     * The values are subtracted from the targets position during the Cameras update step.
     *
     * @since 0.0.0
     *
     * @param x The horizontal offset from the camera follow target.x position.
     * @param y The vertical offset from the camera follow target.y position.
     *
     * @return This Camera instance.
     */
    Camera& setFollowOffset (int x = 0, int y = 0);

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
     * @param target - The target for the Camera to follow.
     * @param lerpX A value between 0 and 1. This value specifies the amount of linear interpolation to use when horizontally tracking the target. The closer the value to 1, the faster the camera will track.
     * @param lerpY A value between 0 and 1. This value specifies the amount of linear interpolation to use when vertically tracking the target. The closer the value to 1, the faster the camera will track.
     * @param offsetX The horizontal offset from the camera follow target.x position.
     * @param offsetY The vertical offset from the camera follow target.y position.
     *
     * @return This Camera instance.
     */
    Camera& startFollow (GameObjects::GameObject& target, float lerpX = 1.0, float lerpY = 1.0, int offsetX = 0, int offsetY = 0);

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
     * @param time The amount of time since SDL started in ms.
     * @param delta The delta time, in ms, elapsed since the last frame.
     */
    void update (Uint32 time, Uint32 delta);

private:
	/**
	 * Internal follow target reference.
	 *
	 * @since 0.0.0
	 */
	GameObjects::GameObject* follow_ = nullptr;
};
}	// namespace Scene2D
}	// namespace Cameras
}	// namespace Zen

#endif
