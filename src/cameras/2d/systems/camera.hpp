/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_CAMERAS_SCENE2D_CAMERA_HPP
#define ZEN_CAMERAS_SCENE2D_CAMERA_HPP

#include <SDL2/SDL_types.h>
#include "../../../ecs/entity.hpp"
#include "../../../math/types/vector2.hpp"

namespace Zen {

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
Entity CreateCamera (double x, double y, double width, double height);

/**
 * @since 0.0.0
 */
void DestroyCamera (Entity entity);

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
void AddToRenderList (Entity entity, Entity child);

std::vector<Entity>* GetRenderList (Entity camera);

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
std::vector<Entity> Cull (Entity entity, std::vector<Entity> renderableEntities);

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
Math::Vector2 GetWorldPoint (Entity entity, int x, int y);

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
void Ignore (Entity camera, Entity entry);

/**
 * @overload
 * @since 0.0.0
 *
 * @param entries_ A vector of GameObjects, to be ignored by this Camera.
 *
 * @return This Camera instance.
 */
void Ignore (Entity camera, std::vector<Entity>& entries);

/**
 * @overload
 * @since 0.0.0
 *
 * @param entries_ A vector of GameObjects, to be ignored by this Camera.
 *
 * @return This Camera instance.
 */
void Ignore (Entity camera, std::vector<Entity>&& entries);

/**
 * Internal preRender step.
 *
 * @since 0.0.0
 */
void PreRender (Entity entity);

/**
 * Fades the Camera in from the given color over the duration specified.
 *
 * @since 0.0.0
 *
 * @param camera The camera to apply the effect on.
 * @param duration The duration of the effect in milliseconds.
 * @param red The amount to fade the red channel towards. A value between 0 and 255.
 * @param green The amount to fade the green channel towards. A value between 0 and
 * 255.
 * @param blue The amount to fade the blue channel towards. A value between 0 and
 * 255.
 * @param callback This callback will be invoked every frame for the duration of
 * the effect.
 * It is sent two arguments: A reference to the camera and a progress amount
 * between 0 and 1 indicating how complete the effect is.
 */
void FadeIn (Entity camera, int duration = 1000, int red = 0, int green = 0,
		int blue = 0, std::function<void(Entity, double)> callback = nullptr);

/**
 * Fades the Camera out to the given color over the duration specified.
 * This is an alias for Camera.fade that forces the fade to start, regardless of existing fades.
 *
 * @since 0.0.0
 *
 * @param camera The camera to apply the effect on.
 * @param duration The duration of the effect in milliseconds.
 * @param red The amount to fade the red channel towards. A value between 0 and 255.
 * @param green The amount to fade the green channel towards. A value between 0 and
 * 255.
 * @param blue The amount to fade the blue channel towards. A value between 0 and
 * 255.
 * @param callback This callback will be invoked every frame for the duration of
 * the effect.
 * It is sent two arguments: A reference to the camera and a progress amount
 * between 0 and 1 indicating how complete the effect is.
 */
void FadeOut (Entity camera, int duration = 1000, int red = 0, int green = 0,
		int blue = 0, std::function<void(Entity, double)> callback = nullptr);

/**
 * Fades the Camera from the given color to transparent over the duration specified.
 *
 * @since 0.0.0
 *
 * @param camera The camera to apply the effect on.
 * @param duration The duration of the effect in milliseconds.
 * @param red The amount to fade the red channel towards. A value between 0 and 255.
 * @param green The amount to fade the green channel towards. A value between 0 and
 * 255.
 * @param blue The amount to fade the blue channel towards. A value between 0 and
 * 255.
 * @param force Force the effect to start immediately, even if already running.
 * @param callback This callback will be invoked every frame for the duration of
 * the effect.
 * It is sent two arguments: A reference to the camera and a progress amount
 * between 0 and 1 indicating how complete the effect is.
 */
void FadeFrom (Entity camera, int duration = 1000, int red = 0, int green = 0,
		int blue = 0, bool force = false,
		std::function<void(Entity, double)> callback = nullptr);

/**
 * Fades the Camera from transparent to the given color over the duration specified.
 *
 * @since 0.0.0
 *
 * @param duration The duration of the effect in milliseconds.
 * @param red The amount to fade the red channel towards. A value between
 * 0 and 255.
 * @param green The amount to fade the green channel towards. A value between
 * 0 and 255.
 * @param blue The amount to fade the blue channel towards. A value between
 * 0 and 255.
 * @param force Force the effect to start immediately, even if already running.
 * @param callback This callback will be invoked every frame for the duration of
 * the effect.
 * It is sent two arguments: A reference to the camera and a progress amount
 * between 0 and 1 indicating how complete the effect is.
 */
void Fade (Entity camera, int duration = 1000, int red = 0, int green = 0,
		int blue = 0, bool force = false,
		std::function<void(Entity, double)> callback = nullptr);

//////**
///// * Flashes the Camera by setting it to the given color immediately and then fading it away again quickly over the duration specified.
///// *
///// * @since 0.0.0
///// *
///// * @param duration_ The duration of the effect in milliseconds.
///// * @param red_ The amount to fade the red channel towards. A value between 0 and 255.
///// * @param green_ The amount to fade the green channel towards. A value between 0 and 255.
///// * @param blue_ The amount to fade the blue channel towards. A value between 0 and 255.
///// * @param force_ Force the effect to start immediately, even if already running.
///// *
///// * @return This Camera instance.
///// */
/////Camera& flash (int duration_ = 250, int red_ = 0, int green_ = 0, int blue_ = 0, bool force_ = false);
/////
//////**
///// * @overload
///// *
///// * @since 0.0.0
///// *
///// * @param duration_ The duration of the effect in milliseconds.
///// * @param red_ The amount to fade the red channel towards. A value between 0 and 255.
///// * @param green_ The amount to fade the green channel towards. A value between 0 and 255.
///// * @param blue_ The amount to fade the blue channel towards. A value between 0 and 255.
///// * @param force_ Force the effect to start immediately, even if already running.
///// * @param callback_ This callback will be invoked every frame for the duration of the effect.
///// * It is sent two arguments: A reference to the camera and a progress amount between 0 and 1 indicating how complete the effect is.
///// * @param context_ The context in which the callback is invoked.
///// *
///// * @return This Camera instance.
///// */
/////template <typename T>
/////	Camera& flash (int duration_, int red_, int green_, int blue_, bool force_, void (T::* callback_)(Camera, double), T* context_)
/////	{
/////		std::function<void(Camera, double)> cb_ = std::bind(
/////				callback_,
/////				context_,
/////				std::placeholders::_1,
/////				std::placeholders::_2);
/////
/////		return flashEffect.start(duration_, red_, green_, blue_, force_, cb_);
/////	}
/////
//////**
///// * Shakes the Camera by the given intensity over the duration specified.
///// *
///// * @since 0.0.0
///// *
///// * @param duration_ The duration of the effect in milliseconds.
///// * @param intensity_ The intensity of the shake.
///// * @param force_ Force the shake effect to start immediately, even if already running.
///// *
///// * @return This Camera instance.
///// */
/////Camera& shake (int duration_ = 100, Math::Vector2 intensity_ = Math::Vector2 (0.05), bool force_ = false);
/////
//////**
///// * @overload
///// *
///// * @since 0.0.0
///// *
///// * @param duration_ The duration of the effect in milliseconds.
///// * @param intensity_ The intensity of the shake.
///// * @param force_ Force the effect to start immediately, even if already running.
///// * @param callback_ This callback will be invoked every frame for the duration of the effect.
///// * It is sent two arguments: A reference to the camera and a progress amount between 0 and 1 indicating how complete the effect is.
///// * @param context_ The context in which the callback is invoked.
///// *
///// * @return This Camera instance.
///// */
/////template <typename T>
/////	Camera& shake (int duration_, Math::Vector2 intensity_, bool force_, void (T::* callback_)(Camera, double), T* context_)
/////	{
/////		std::function<void(Camera, double)> cb_ = std::bind(
/////				callback_,
/////				context_,
/////				std::placeholders::_1,
/////				std::placeholders::_2);
/////
/////		return shakeEffect.start(duration_, intensity_, force_, cb_);
/////	}
/////
//////**
///// * This effect will scroll the Camera so that the center of its viewport finishes at the given destination,
///// * over the duration and with the ease specified.
///// *
///// * @since 0.0.0
///// *
///// * @param x_ The destination x coordinate to scroll the center of the Camera viewport to.
///// * @param y_ The destination y coordinate to scroll the center of the Camera viewport to.
///// * @param duration_ The duration of the effect in milliseconds.
///// * @param ease_ The ease to use for the pan.
///// * @param force_ Force the pan effect to start immediately, even if already running.
///// *
///// * @return This Camera instance.
///// */
/////Camera& pan (int x_, int y_, int duration_ = 1000, std::string ease_ = "Linear", bool force_ = false);
/////
//////**
///// * @overload
///// *
///// * @since 0.0.0
///// *
///// * @param x_ The destination x coordinate to scroll the center of the Camera viewport to.
///// * @param y_ The destination y coordinate to scroll the center of the Camera viewport to.
///// * @param duration_ The duration of the effect in milliseconds.
///// * @param ease_ The ease to use for the pan.
///// * @param force_ Force the pan effect to start immediately, even if already running.
///// * @param callback_ This callback will be invoked every frame for the duration of the effect.
///// * It is sent four arguments: A reference to the camera, a progress amount between 0 and 1 indicating how complete the effect is,
///// * the current camera scroll x coordinate and the current camera scroll y coordinate.
///// * @param context_ The context in which the callback is invoked.
///// *
///// * @return This Camera instance.
///// */
/////template <typename T>
/////	Camera& pan (int x_, int y_, int duration_, std::string ease_, bool force_, void (T::* callback_)(Camera, double, int, int), T* context_)
/////	{
/////		std::function<void(Camera, double, int, int)> cb_ = std::bind(
/////				callback_,
/////				context_,
/////				std::placeholders::_1,
/////				std::placeholders::_2,
/////				std::placeholders::_3,
/////				std::placeholders::_4);
/////
/////		return panEffect.start(x_, y_, duration_, ease_, force_, cb_);
/////	}
/////
//////**
///// * This effect will rotate the Camera so that the viewport finishes at the given angle in radians,
///// * over the duration and with the ease specified.
///// *
///// * @since 0.0.0
///// *
///// * @param radians_ The destination angle in radians to rotate the Camera viewport to. If the angle is positive then the rotation is clockwise else anticlockwise
///// * @param shortestPath_ If shortest path is set to true the camera will rotate in the quickest direction clockwise or anti-clockwise.
///// * @param duration_ - The duration of the effect in milliseconds.
///// * @param ease_ The ease to use for the rotation.
///// * @param force_ Force the rotation effect to start immediately, even if already running.
///// *
///// * @return This Camera instance.
///// */
/////Camera& rotateTo (
/////		double radians_,
/////		bool shortestPath_ = false,
/////		int duration_ = 1000,
/////		std::string ease_ = "Linear",
/////		bool force_ = false);
/////
//////**
///// * @overload
///// *
///// * @since 0.0.0
///// *
///// * @param radians_ The destination angle in radians to rotate the Camera viewport to. If the angle is positive then the rotation is clockwise else anticlockwise
///// * @param shortestPath_ If shortest path is set to true the camera will rotate in the quickest direction clockwise or anti-clockwise.
///// * @param duration_ - The duration of the effect in milliseconds.
///// * @param ease_ The ease to use for the rotation.
///// * @param force_ Force the rotation effect to start immediately, even if already running.
///// * @param callback_ This callback will be invoked every frame for the duration of the effect.
///// * It is sent three arguments: A reference to the camera, a progress amount between 0 and 1 indicating how complete the effect is,
///// * the current camera rotation angle in radians.
///// * @param context_ The context in which the callback is invoked.
///// *
///// * @return This Camera instance.
///// */
/////template <typename T>
/////	Camera& rotateTo (double radians_, bool shortestPath_, int duration_, std::string ease_, bool force_, void (T::* callback_)(Camera, double, double), T* context_)
/////	{
/////		std::function<void(Camera, double, double)> cb_ = std::bind(
/////				callback_,
/////				context_,
/////				std::placeholders::_1,
/////				std::placeholders::_2,
/////				std::placeholders::_3);
/////
/////		return rotateToEffect.start(radians_, shortestPath_, duration_, ease_, force_, cb_);
/////	}
/////
//////**
///// * This effect will zoom the Camera to the given scale, over the duration and with the ease specified.
///// *
///// * @since 0.0.0
///// *
///// * @param zoom_ The target Camera zoom value.
///// * @param duration_ The duration of the effect in milliseconds.
///// * @param ease_ The ease to use for the pan.
///// * @param force_ Force the pan effect to start immediately, even if already running.
///// *
///// * @return This Camera instance.
///// */
/////Camera& zoomTo (double zoom_, int duration_, std::string ease_, bool force_);
/////
//////**
///// * @overload
///// *
///// * @since 0.0.0
///// *
///// * @param zoom_ The target Camera zoom value.
///// * @param duration_ The duration of the effect in milliseconds.
///// * @param ease_ The ease to use for the pan.
///// * @param force_ Force the pan effect to start immediately, even if already running.
///// * @param callback_ - This callback will be invoked every frame for the duration of the effect.
///// * It is sent four arguments: A reference to the camera, a progress amount between 0 and 1 indicating how complete the effect is,
///// * the current camera scroll x coordinate and the current camera scroll y coordinate.
///// * @param context_ - The context in which the callback is invoked. Defaults to the Scene to which the Camera belongs.
///// *
///// * @return This Camera instance.
///// */
/////template <typename T>
/////	Camera& zoomTo (double zoom_, int duration_, std::string ease_, bool force_, void (T::* callback_)(Camera, double, int, int), T* context_)
/////	{
/////		std::function<void(Camera, double, double)> cb_ = std::bind(
/////				callback_,
/////				context_,
/////				std::placeholders::_1,
/////				std::placeholders::_2,
/////				std::placeholders::_3,
/////				std::placeholders::_4);
/////
/////		return zoomEffect.start(zoom_, duration_, ease_, force_, cb_);
/////	}

/**
 * Resets any active FX, such as a fade, flash or shake. Useful to call after a
 * fade in order to remove the fade.
 *
 * @since 0.0.0
 */
void resetFX (Entity camera);

/**
 * Internal method called automatically when the viewport changes.
 *
 * @since 0.0.0
 */
void UpdateCameraSystem (Entity entity);

/**
 * Internal method called automatically by the Camera Manager.
 *
 * @since 0.0.0
 *
 * @param camera The camera to update
 * @param time The amount of time since SDL started in ms.
 * @param delta The delta time, in ms, elapsed since the last frame.
 */
void UpdateCamera (Entity camera, Uint32 time, Uint32 delta);

}	// namespace Zen

#endif
