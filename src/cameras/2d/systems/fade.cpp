/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "fade.hpp"
#include "../components/fade.hpp"
#include "../../../components/actor.hpp"
#include "../../../components/position.hpp"
#include "../../../components/size.hpp"
#include "../../../utils/messages.hpp"
#include "../../../scene/scene.hpp"
#include "../../../math/clamp.hpp"
#include "../../../window/window.hpp"
#include "../events/events.hpp"

namespace Zen {

extern entt::registry g_registry;
extern Window g_window;

void StartFade (Entity camera, bool direction, int duration,
		int red, int green, int blue, bool force,
		std::function<void(Entity, double)> callback)
{
	auto [fade, actor] = g_registry.try_get<Components::EffectFade,
		 Components::Actor>(camera);
	if (!fade) {
		MessageWarning("The entity has no 'EffectFade' component");
		return;
	}

	if (!force && fade->isRunning)
		return;

	fade->isRunning = true;
	fade->isComplete = false;
	fade->duration = duration;
	fade->direction = direction;
	fade->progress = 0;

	fade->red = red;
	fade->green = green;
	fade->blue = blue;
	fade->alpha = (direction) ? 0 : 255;

	fade->elapsed = 0;

	fade->onUpdate = callback;

	if (actor) {
		std::string eventName = (direction)
			? ZEN_EVENTS_CAMERAS_FADE_OUT_START
			: ZEN_EVENTS_CAMERAS_FADE_IN_START;

		actor->scene->events.emit(camera, eventName, camera, duration, red,
				green, blue);
	}
}

void UpdateFade (Entity camera, [[maybe_unused]] Uint32 time, Uint32 delta)
{
	auto fade = g_registry.try_get<Components::EffectFade>(camera);
	if (!fade) {
		MessageWarning("The entity has no 'EffectFade' component");
		return;
	}

	if (!fade->isRunning)
		return;

	fade->elapsed += delta;

	fade->progress = Math::Clamp(fade->elapsed / fade->duration, 0, 1);

	if (fade->onUpdate)
		fade->onUpdate(camera, fade->progress);

	if (fade->elapsed < fade->duration) {
		fade->alpha = (fade->direction)
			? (fade->progress) * 255
			: (1 - fade->progress) * 255;
	}
	else {
		fade->alpha = (fade->direction) ? 255 : 0;
		CompleteFade(camera);
	}
}

bool PostRenderFade (Entity camera)
{
	auto [fade, position, size] = g_registry.try_get<Components::EffectFade,
		 Components::Position, Components::Size>(camera);
	if (!fade) {
		MessageWarning("The entity has no 'EffectFade' component");
		return false;
	}

	if (!fade->isRunning && !fade->isComplete)
		return false;

	SDL_SetRenderDrawColor(g_window.renderer, fade->red, fade->green,
			fade->blue, fade->alpha);

	SDL_FRect rect {
		(float) position->x,
		(float) position->y,
		(float) size->width,
		(float) size->height
	};

	SDL_SetRenderDrawBlendMode(g_window.renderer,
			SDL_BlendMode::SDL_BLENDMODE_BLEND);
	SDL_RenderFillRectF(g_window.renderer, &rect);

	return true;
}

void CompleteFade (Entity camera)
{
	auto [fade, actor] = g_registry.try_get<Components::EffectFade,
		 Components::Actor>(camera);
	if (!fade) {
		MessageWarning("The entity has no 'EffectFade' component");
		return;
	}

	fade->onUpdate = nullptr;
	fade->isRunning = false;
	fade->isComplete = true;

	if (actor) {
		std::string eventName = (fade->direction)
			? ZEN_EVENTS_CAMERAS_FADE_OUT_COMPLETE
			: ZEN_EVENTS_CAMERAS_FADE_IN_COMPLETE;

		actor->scene->events.emit(camera, eventName, camera);
	}
}

void ResetFade (Entity camera)
{
	auto fade = g_registry.try_get<Components::EffectFade>(camera);
	if (!fade) {
		MessageWarning("The entity has no 'EffectFade' component");
		return;
	}

	fade->onUpdate = nullptr;
	fade->isRunning = false;
	fade->isComplete = false;
}

}	// namespace Zen
