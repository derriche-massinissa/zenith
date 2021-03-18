/**
 * @file		bitmap_mask.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "bitmap_mask.h"

namespace Zen {
namespace Masks {

BitmapMask::BitmapMask (GameObjects:GameObject& renderable)
{
	maskObject_ = &renderable;
}

void BitmapMask::preRender (
		Renderer *renderer,
		GameObjects::GameObject *maskedObject,
		Camera *camera)
{
	// Is this a camera mask?
	if (!maskedObject)
	{
		SDL_SetRenderTarget(renderer->window.renderer_, renderer->cameraBuffer_);
	}
	else
	{
		SDL_SetRenderTarget(renderer->window.renderer_, renderer->maskBuffer_);
	}

	// Clear _AFTER_ setting the target, to clear the buffer and not the screen
	SDL_SetRenderDrawColor(renderer->window.renderer_, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer->window.renderer_);
}

void BitmapMask::postRender (
		Renderer *renderer,
		GameObjects::GameObject *maskedObject,
		Camera *camera)
{
	// Save the target buffer
	SDL_Texture *currentTarget = SDL_GetRenderTarget(renderer->window.renderer_;)

	// Set the mask texture as the new render target
	SDL_SetRenderTarget(renderer->window.renderer_, renderer->maskTexture_);

	// Clear the mask texture
	SDL_SetRenderDrawColor(renderer->window.renderer_, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer->window.renderer_);

	// Draw the mask GameObject
	maskObject_->render(*renderer, *maskObject_, *camera);

	// Reset the target to the buffer
	SDL_SetRenderTarget(renderer->window.renderer_, currentTarget);

	// Render the mask on the buffer
	SDL_RenderCopy(
			renderer->window.renderer_,
			renderer->maskTexture_,
			nullptr,	// Render the whole texture
			nullptr		// Render to the entire target
			);

	// Is this a camera mask?
	if (!maskedObject)
	{
		// Reset the rendering target
		SDL_SetRenderTarget(renderer->window.renderer_, nullptr);

		SDL_RenderCopy(
				renderer->window.renderer_,
				renderer->cameraBuffer_,
				nullptr,	// Render the whole texture
				nullptr		// Render to the entire target
				);
	}
	else
	{
		// Is a camera mask active? If so, set it back to be the rendering target
		if (camera->mask)
			SDL_SetRenderTarget(renderer->window.renderer_, renderer->cameraBuffer_);
		else
			SDL_SetRenderTarget(renderer->window.renderer_, nullptr);

		SDL_RenderCopy(
				renderer->window.renderer_,
				renderer->maskBuffer_,
				nullptr,	// Render the whole texture
				nullptr		// Render to the entire target
				);
	}
}

}	// namespace Masks
}	// namespace Zen
