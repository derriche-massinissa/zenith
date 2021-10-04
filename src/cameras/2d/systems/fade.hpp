/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_CAMERAS_SCENE2D_SYSTEMS_FADE_HPP
#define ZEN_CAMERAS_SCENE2D_SYSTEMS_FADE_HPP

#include <functional>
#include <SDL2/SDL_types.h>
#include "../../../ecs/entity.hpp"
#include "../../../scene/scene.fwd.hpp"

namespace Zen {

class MultiPipeline;

/**
 * Fades the Camera to or from the given color over the duration specified.
 *
 * @since 0.0.0
 *
 * @param camera The camera to run the effect on.
 * @param direction The direction of the fade. `true` = fade out (transparent to
 * color), `false` = fade in (color to transparent)
 * @param duration The duration of the effect in milliseconds.
 * @param red The amount to fade the red channel towards. A value between 0 and 255.
 * @param green The amount to fade the green channel towards. A value between 0 and
 * 255.
 * @param blue The amount to fade the blue channel towards. A value between 0 and
 * 255.
 * @param force Force the effect to start immediately, even if already running.
 * @param callback - This callback will be invoked every frame for the duration of
 * the effect.
 * It is sent two arguments: A reference to the camera and a progress amount
 * between 0 and 1 indicating how complete the effect is.
 */
void StartFade (Entity camera, bool direction = true, int duration = 1000,
		int red = 0, int green = 0, int blue = 0, bool force = false,
		std::function<void(Entity, double)> callback = nullptr);

/**
 * The main update loop for this effect. Called automatically by the camera manager.
 *
 * @since 0.0.0
 *
 * @param time The current timestamp since the start of SDL.
 * @param delta The delta time, in ms, elapsed since the last frame.
 */
void UpdateFade (Entity camera, Uint32 time, Uint32 delta);

/**
 * Called internally by the renderer.
 *
 * @since 0.0.0
 *
 * @return `true` if the effect drew to the renderer, otherwise `false`.
 */
bool PostRenderFade (Entity camera, MultiPipeline& pipeline);

/**
 * Called internally when the effect completes.
 *
 * @since 0.0.0
 */
void CompleteFade (Entity camera);

/**
 * Resets this camera effect.
 * If it was previously running, it stops instantly without calling its onComplete
 * callback or emitting an event.
 *
 * @since 0.0.0
 */
void ResetFade (Entity camera);

bool IsFadeRunning (Entity camera);

bool IsFadeComplete (Entity camera);

}	// namespace Zen

#endif
