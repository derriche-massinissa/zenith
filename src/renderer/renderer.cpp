/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "renderer.h"

#include "../gameobjects/gameobject.h"
#include "../texture/frame.h"
#include "../cameras/2d/camera.h"
#include "../core/game.h"
#include "../window/window.h"
#include "../scene/scene.h"

namespace Zen {

Renderer::Renderer (Game& game_)
	: game (game_), window (game_.window)
{
	backgroundColor = game_.config.backgroundColor;

	game_.scale.on("resize", &Renderer::onResize, this);

	// Get the pixel format of the window
	SDL_Surface *infoSurface_ = nullptr;
	infoSurface_ = SDL_GetWindowSurface(window.window);

	if (!infoSurface_)
	{
		messageError("Failed to create info surface from window: ", SDL_GetError());
		return;
	}

	pixelFormat = *infoSurface_->format;

	SDL_FreeSurface(infoSurface_);

	// Set the mask texture blend mode
	maskBlendMode = SDL_ComposeCustomBlendMode(
			SDL_BLENDFACTOR_ZERO,			// Zero out the mask color
			SDL_BLENDFACTOR_ONE,			// Keep the masked color
			SDL_BLENDOPERATION_ADD,
			SDL_BLENDFACTOR_ZERO,			// Zero out the mask alpha
			SDL_BLENDFACTOR_SRC_ALPHA,		// Multiply masked alpha by mask alpha
			SDL_BLENDOPERATION_ADD
			);

	resize(window.width(), window.height());
}

Renderer::~Renderer ()
{
	if (snapshotState.surface)
		SDL_FreeSurface(snapshotState.surface);

	if (maskBuffer)
		SDL_DestroyTexture(maskBuffer);

	if (cameraBuffer)
		SDL_DestroyTexture(cameraBuffer);

	if (maskTexture)
		SDL_DestroyTexture(maskTexture);
}

void Renderer::onResize (int width_, int height_, int previousWidth_, int previousHeight_)
{
	resize(width_, height_);
}

void Renderer::resize (int width_, int height_)
{
	emit("resize", width_, height_);

	width = width_;
	height = height_;

	// Free the mask buffer
	if (maskBuffer)
		SDL_DestroyTexture(maskBuffer);

	// Reset it to the same size as the renderer
	maskBuffer = SDL_CreateTexture(
			window.renderer,
			pixelFormat.format,
			SDL_TEXTUREACCESS_TARGET,
			width,
			height
			);

	// Free the camera buffer
	if (cameraBuffer)
		SDL_DestroyTexture(cameraBuffer);

	// Reset it to the same size as the renderer
	cameraBuffer = SDL_CreateTexture(
			window.renderer,
			pixelFormat.format,
			SDL_TEXTUREACCESS_TARGET,
			width,
			height
			);

	// Free the mask texture
	if (maskTexture)
		SDL_DestroyTexture(maskTexture);

	// Reset it to the same size as the renderer
	maskTexture = SDL_CreateTexture(
			window.renderer,
			pixelFormat.format,
			SDL_TEXTUREACCESS_TARGET,
			width,
			height
			);
	// Set the mask texture's blend mode
	SDL_SetTextureBlendMode(maskTexture, maskBlendMode);
}

void Renderer::preRender ()
{
	if (game.config.clearBeforeRender)
	{
		SDL_SetRenderDrawColor(
				window.renderer,
				backgroundColor.red(),
				backgroundColor.green(),
				backgroundColor.blue(),
				0xFF);
		SDL_RenderClear(window.renderer);
	}

	drawCount = 0;

	emit("pre-render");
}

void Renderer::render (
		Scene& scene_,
		std::vector<GameObjects::GameObject*> children_,
		Cameras::Scene2D::Camera& camera_)
{
	if (window.isMinimized())
		return;

	emit("render");

	SDL_Rect c_;
	c_.x = camera_.getX();
	c_.y = camera_.getY();
	c_.w = camera_.getWidth();
	c_.h = camera_.getHeight();

	// Set a viewport if the camera isn't the same size as the window
	if (game.scene.customViewports) {
		SDL_RenderSetViewport(window.renderer, &c_);
	}

	if (camera_.mask)
		preRenderMask(nullptr);

	// Camera's background color if not transparent
	if (!camera_.transparent) {
		SDL_SetRenderDrawColor(
				window.renderer,
				camera_.backgroundColor.red(),
				camera_.backgroundColor.green(),
				camera_.backgroundColor.blue(),
				camera_.backgroundColor.alpha());
		SDL_RenderFillRect(window.renderer, &c_);
	}

	drawCount += children_.size();

	// Render the GameObject
	for (auto& child_ : children_)
	{
		camera_.addToRenderList(*child_);

		batchSprite(*child_, *child_->frame, camera_, child_->parentMatrix_);
	}

	camera_.flashEffect.postRender();
	camera_.fadeEffect.postRender();

	camera_.dirty = false;

	if (camera_.mask)
		postRenderMask(camera_.mask, nullptr, &camera_);

	// Remove the viewport if previously set
	if (game.scene.customViewports)
		SDL_RenderSetViewport(window.renderer, nullptr);

	// Update the screen
	SDL_RenderPresent(window.renderer);
}

void Renderer::postRender ()
{
	emit("post-render");

	if (snapshotState.active)
	{
		takeSnapshot();

		snapshotState.active = false;
	}
}

Renderer& Renderer::snapshot (std::string path_, std::function<void(SDL_Surface*)> callback_)
{
	return snapshotArea(0, 0, window.width(), window.height(), path_, callback_);
}

Renderer& Renderer::snapshotArea (
		int x_,
		int y_,
		int width_,
		int height_,
		std::string path_,
		std::function<void(SDL_Surface*)> callback_)
{
	snapshotState.callback = callback_;

	snapshotState.callbackPixel = nullptr;

	snapshotState.path = path_;

	snapshotState.getPixel = false;

	snapshotState.x = Math::clamp(x_, 0, window.width());

	snapshotState.y = Math::clamp(y_, 0, window.height());

	snapshotState.width = Math::clamp(
			width_,
			0,
			window.width() - snapshotState.x);

	snapshotState.height = Math::clamp(
			height_,
			0,
			window.height() - snapshotState.y);

	snapshotState.active = true;

	return *this;
}

Renderer& Renderer::snapshotPixel (int x_, int y_, std::function<void(Display::Color)> callback_)
{
	snapshotArea(x_, y_, 1, 1, nullptr);

	snapshotState.getPixel = true;
	snapshotState.callbackPixel = callback_;

	return *this;
}

void Renderer::takeSnapshot ()
{
	if (!snapshotState.active)
		return;

	if (snapshotState.surface) {
		SDL_FreeSurface(snapshotState.surface);
		snapshotState.surface = nullptr;
	}

	if (snapshotState.getPixel) {
		// Pixel grab
		Uint32 pixel_;
		int pitch_ = 4;
		SDL_Rect rect_;
		rect_.x = snapshotState.x;
		rect_.y = snapshotState.y;
		rect_.w = 1;
		rect_.h = 1;

		if (SDL_RenderReadPixels(
					window.renderer,
					&rect_,
					pixelFormat.format,
					&pixel_,
					pitch_
					))
		{
			messageError("Failed to read the window pixel data: ", SDL_GetError());
			return;
		}

		// Get the color components of the pixel
		Uint8 r_ = 0, g_ = 0, b_ = 0, a_ = 0;
		SDL_GetRGBA(pixel_, &pixelFormat, &r_, &g_, &b_, &a_);

		// Save the color components to the output color object
		Display::Color out_;
		out_.setTo(r_, g_, b_, a_);

		// Call the callback if one was given
		if (snapshotState.callbackPixel)
			snapshotState.callbackPixel(out_);
	}
	else
	{
		// Area/Full grab
		bool areaSnapshot = (
				snapshotState.x != 0 ||
				snapshotState.y != 0 ||
				snapshotState.width != window.width() ||
				snapshotState.height != window.height()
				);

		// Create a blank surface
		if (areaSnapshot) {
			snapshotState.surface = SDL_CreateRGBSurface(
					0,
					snapshotState.width,
					snapshotState.height,
					pixelFormat.BitsPerPixel,
					pixelFormat.Rmask,
					pixelFormat.Gmask,
					pixelFormat.Bmask,
					pixelFormat.Amask
					);
		} else {
			snapshotState.surface = SDL_CreateRGBSurface(
					0,
					width,
					height,
					pixelFormat.BitsPerPixel,
					pixelFormat.Rmask,
					pixelFormat.Gmask,
					pixelFormat.Bmask,
					pixelFormat.Amask
					);
		}
		if (!snapshotState.surface) {
			messageError("Failed to create an RGB surface: ", SDL_GetError());
			return;
		}

		int readFailed_ = 1;

		// Read the window's pixels
		if (areaSnapshot) {
			// Read the rectangle defined in snapshotState
			SDL_Rect rect_;
			rect_.x = snapshotState.x;
			rect_.y = snapshotState.y;
			rect_.w = snapshotState.width;
			rect_.h = snapshotState.height;

			readFailed_ = SDL_RenderReadPixels(
					window.renderer,
					&rect_,
					pixelFormat.format,
					snapshotState.surface->pixels,
					snapshotState.surface->pitch);
		} else {
			// Read the whole window
			readFailed_ = SDL_RenderReadPixels(
					window.renderer,
					nullptr,
					pixelFormat.format,
					snapshotState.surface->pixels,
					snapshotState.surface->pitch);
		}

		// Check if the renderer was read normally
		if (readFailed_) {
			messageError("Failed to read the window pixel data: ", SDL_GetError());
			SDL_FreeSurface(snapshotState.surface);
			return;
		}

		// Save an image file if a path is given
		if (snapshotState.path != "") {
			saveSnapshot();
		}

		// Call the callback if one was given
		if (snapshotState.callback)
			snapshotState.callback(snapshotState.surface);
	}
}

void Renderer::saveSnapshot ()
{
	std::string& path_ = snapshotState.path;
	std::string extension_ = "";

	// Figure out the file type
	for (auto c_ = path_.rbegin(); c_ != path_.rend(); c_++) {
		if (*c_ == '.')
		{
			// We read all the extension's characters
			break;
		}

		extension_.insert(0, 1, *c_);
	}

	if (extension_ == "bmp")
	{
		if (SDL_SaveBMP(snapshotState.surface, path_.c_str()))
			messageError("Failed to save snapshot in a 'BMP' file: ", SDL_GetError());
	}
	else if (extension_ == "png")
	{
		if (IMG_SavePNG(snapshotState.surface, path_.c_str()))
			messageError("Failed to save snapshot in a 'PNG' file: ", IMG_GetError());
	}
	else if (extension_ == "jpg")
	{
		if (IMG_SaveJPG(snapshotState.surface, path_.c_str(), 100))
			messageError("Failed to save snapshot in a 'JPG' file: ", IMG_GetError());
	}
	else if (extension_ == "jpeg")
	{
		if (IMG_SaveJPG(snapshotState.surface, path_.c_str(), 100))
			messageError("Failed to save snapshot in a 'JPEG' file: ", IMG_GetError());
	}
	else
	{
		messageError("File type unsupported! Try something else like 'png' or 'jpg'.");
	}
}

void Renderer::batchSprite (
		GameObjects::GameObject& sprite_,
		Textures::Frame& frame_,
		Cameras::Scene2D::Camera& camera_,
		GameObjects::Components::TransformMatrix *parentTransformMatrix_)
{
	float alpha_ = camera_.getAlpha() * sprite_.getAlpha();

	if (!alpha_)
		// Nothing to see, so abort early
		return;

	auto& camMatrix_ = tempMatrix1;
	auto& spriteMatrix_ = tempMatrix2;

	auto dd_ = frame_.getDrawImageData();

	int frameX_ = dd_.x;
	int frameY_ = dd_.y;
	int frameWidth_ = frame_.cutWidth;
	int frameHeight_ = frame_.cutHeight;
	bool customPivot_ = frame_.customPivot;

	float res_ = frame_.source->resolution;

	int displayOriginX_ = sprite_.getDisplayOriginX();
	int displayOriginY_ = sprite_.getDisplayOriginY();

	int x_ = -displayOriginX_ + frame_.x;
	int y_ = -displayOriginY_ + frame_.y;

	if (sprite_.isCropped) {
		auto& crop_ = sprite_.crop;

		if (crop_.flipX != sprite_.flipX || crop_.flipY != sprite_.flipY)
			frame_.updateCropUVs(crop_, sprite_.flipX, sprite_.flipY);

		frameWidth_ = crop_.cw;
		frameHeight_ = crop_.ch;

		frameX_ = crop_.cx;
		frameY_ = crop_.cy;

		x_ = -displayOriginX_ + crop_.x;
		y_ = -displayOriginY_ + crop_.y;

		if (sprite_.flipX) {
			if (x_ >= 0)
				x_ = -(x_ + frameWidth_);
			else
				x_ = (std::abs(x_) - frameWidth_);
		}

		if (sprite_.flipY) {
			if (y_ >= 0)
				y_ = -(y_ + frameHeight_);
			else
				y_ = (std::abs(y_) - frameHeight_);
		}
	}

	int flipX_ = 1;
	int flipY_ = 1;

	if (sprite_.flipX) {
		if (!customPivot_)
			x_ += (-frame_.getRealWidth() + (displayOriginX_ * 2));

		flipX_ = -1;
	}

	if (sprite_.flipY) {
		if (!customPivot_)
			y_ += (-frame_.getRealHeight() + (displayOriginY_ * 2));

		flipY_ = -1;
	}

	spriteMatrix_.applyITRS(sprite_.x, sprite_.y, sprite_.rotation, sprite_.getScaleX() * flipX_, sprite_.getScaleY() * flipY_);

	camMatrix_.copyFrom(camera_.matrix);

	if (parentTransformMatrix_)
	{
		// Multiply the camera by the parent matrix
		camMatrix_.multiplyWithOffset(
				*parentTransformMatrix_,
				-camera_.scrollX * sprite_.getScrollFactorX(),
				-camera_.scrollY * sprite_.getScrollFactorY()
				);

		// Undo the camera scroll
		spriteMatrix_.setE(sprite_.x);
		spriteMatrix_.setF(sprite_.y);
	}
	else
	{
		spriteMatrix_.setE(
				spriteMatrix_.getE() - (camera_.scrollX * sprite_.getScrollFactorX()));
		spriteMatrix_.setF(
				spriteMatrix_.getF() - (camera_.scrollY * sprite_.getScrollFactorY()));
	}

	// Multiply by the Sprite matrix
	camMatrix_.multiply(spriteMatrix_);

	// Apply the transform matrix
	// TODO FIXME Order of operations should be: translate, rotate, scale
	// But here it is: translate, scale, rotate
	// Should either rotate after rendering, or add a decomposition method
	// that returns the appropriate transform matrix
	GameObjects::Components::DecomposedMatrix dm_ = camMatrix_.decomposeMatrix();

	// TODO
	//setGlobalCompositionOperation(sprite.blendMode);

	if (sprite_.mask)
		preRenderMask(&sprite_);

	// Render the texture
	SDL_Rect source_ {frameX_, frameY_, frameWidth_, frameHeight_};

	// ScaleManager values
	Math::Vector2 sScale_ = game.scale.displayScale;
	Math::Vector2 sOffset_ = game.scale.displayOffset;

	SDL_Rect destination_;
	destination_.x = x_ + dm_.translateX + sOffset_.x;
	destination_.y = y_ + dm_.translateY + sOffset_.y;
	destination_.w = (frameWidth_ / res_) * dm_.scaleX * sScale_.x;
	destination_.h = (frameHeight_ / res_) * dm_.scaleY * sScale_.y;

	double angle_ = dm_.rotation;

	SDL_Point origin_ {displayOriginX_, displayOriginY_};

	SDL_RendererFlip flip_ = SDL_FLIP_NONE;
	if (sprite_.flipX)
		flip_ = (SDL_RendererFlip)SDL_FLIP_HORIZONTAL;
	else if (sprite_.flipY)
		flip_ = (SDL_RendererFlip)SDL_FLIP_VERTICAL;
	else if (sprite_.flipX && sprite_.flipY)
		flip_ = (SDL_RendererFlip)(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL);

	SDL_RenderCopyEx(
			window.renderer,
			frame_.source->sdlTexture,
			&source_,
			&destination_,
			angle_,
			&origin_,
			flip_
			);

	if (sprite_.mask)
		postRenderMask(sprite_.mask, &sprite_, &camera_);
}

void Renderer::preRenderMask (
		GameObjects::GameObject *maskedObject_)
{
	// Is this a camera mask?
	if (!maskedObject_)
	{
		SDL_SetRenderTarget(window.renderer, cameraBuffer);
	}
	else
	{
		SDL_SetRenderTarget(window.renderer, maskBuffer);
	}

	// Clear _AFTER_ setting the target, to clear the buffer and not the screen
	SDL_SetRenderDrawColor(window.renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(window.renderer);
}

void Renderer::postRenderMask (
		GameObjects::GameObject *maskObject_,
		GameObjects::GameObject *maskedObject_,
		Cameras::Scene2D::Camera *camera_)
{
	// Save the target buffer
	SDL_Texture *currentTarget_ = SDL_GetRenderTarget(window.renderer);

	// Set the mask texture as the new render target
	SDL_SetRenderTarget(window.renderer, maskTexture);

	// Clear the mask texture
	SDL_SetRenderDrawColor(window.renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(window.renderer);

	// Draw the mask GameObject
	camera_->addToRenderList(*maskObject_);
	batchSprite(*maskObject_, *maskObject_->frame, *camera_, maskObject_->parentMatrix_);

	// Reset the target to the buffer
	SDL_SetRenderTarget(window.renderer, currentTarget_);

	// Render the mask on the buffer
	SDL_RenderCopy(
			window.renderer,
			maskTexture,
			nullptr,	// Render the whole texture
			nullptr		// Render to the entire target
			);

	// Is this a camera mask?
	if (!maskedObject_)
	{
		// Reset the rendering target
		SDL_SetRenderTarget(window.renderer, nullptr);

		SDL_RenderCopy(
				window.renderer,
				cameraBuffer,
				nullptr,	// Render the whole texture
				nullptr		// Render to the entire target
				);
	}
	else
	{
		// Is a camera mask active? If so, set it back to be the rendering target
		if (camera_->mask)
			SDL_SetRenderTarget(window.renderer, cameraBuffer);
		else
			SDL_SetRenderTarget(window.renderer, nullptr);

		SDL_RenderCopy(
				window.renderer,
				maskBuffer,
				nullptr,	// Render the whole texture
				nullptr		// Render to the entire target
				);
	}
}

}	// namespace Zen
