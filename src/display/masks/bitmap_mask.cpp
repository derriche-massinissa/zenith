/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include <SDL2/SDL.h>
#include "bitmap_mask.h"

namespace Zen {
namespace Display {
namespace Masks {

BitmapMask::BitmapMask (GameObjects::GameObject& renderable_)
{
	maskObject = &renderable_;
}

void BitmapMask::preRender (
		Renderer *renderer_,
		GameObjects::GameObject *maskedObject_,
		Cameras::Scene2D::Camera *camera_)
{
	// Is this a camera mask?
	if (!maskedObject_)
	{
		SDL_SetRenderTarget(renderer_->window.renderer, renderer_->cameraBuffer);
	}
	else
	{
		SDL_SetRenderTarget(renderer_->window.renderer, renderer_->maskBuffer);
	}

	// Clear _AFTER_ setting the target, to clear the buffer and not the screen
	SDL_SetRenderDrawColor(renderer_->window.renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer_->window.renderer);
}

void BitmapMask::postRender (
		Renderer *renderer_,
		GameObjects::GameObject *maskedObject_,
		Cameras::Scene2D::Camera *camera_)
{
	// Save the target buffer
	SDL_Texture *currentTarget_ = SDL_GetRenderTarget(renderer_->window.renderer;)

	// Set the mask texture as the new render target
	SDL_SetRenderTarget(renderer_->window.renderer, renderer_->maskTexture);

	// Clear the mask texture
	SDL_SetRenderDrawColor(renderer_->window.renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer_->window.renderer);

	// Draw the mask GameObject
	maskObject->render(*renderer_, *maskObject, *camera_);

	// Reset the target to the buffer
	SDL_SetRenderTarget(renderer_->window.renderer, currentTarget_);

	// Render the mask on the buffer
	SDL_RenderCopy(
			renderer_->window.renderer,
			renderer_->maskTexture,
			nullptr,	// Render the whole texture
			nullptr		// Render to the entire target
			);

	// Is this a camera mask?
	if (!maskedObject_)
	{
		// Reset the rendering target
		SDL_SetRenderTarget(renderer_->window.renderer, nullptr);

		SDL_RenderCopy(
				renderer_->window.renderer,
				renderer_->cameraBuffer,
				nullptr,	// Render the whole texture
				nullptr		// Render to the entire target
				);
	}
	else
	{
		// Is a camera mask active? If so, set it back to be the rendering target
		if (camera_->mask)
			SDL_SetRenderTarget(renderer_->window.renderer, renderer_->cameraBuffer);
		else
			SDL_SetRenderTarget(renderer_->window.renderer, nullptr);

		SDL_RenderCopy(
				renderer_->window.renderer,
				renderer_->maskBuffer,
				nullptr,	// Render the whole texture
				nullptr		// Render to the entire target
				);
	}
}

}	// namespace Masks
}	// namespace Display
}	// namespace Zen
