/**
 * @file		renderer.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "renderer.h"

namespace Zen {

Renderer::Renderer (Game& game_)
	: game(game_), window(game_.window)
{
	backgroundColor = game.config.backgroundColor;

	game.scale.on("SYS_RESIZE", &Renderer::onResize, this);

	// Get the pixel format of the window
	// Get the window's surface
	SDL_Surface *infoSurface = nullptr;
	infoSurface = SDL_GetWindowSurface(window.window_);
	if (!infoSurface) {
		messageError("Failed to create info surface from window: ", SDL_GetError());
		return;
	}

	pixelFormat_ = infoSurface->format;

	SDL_FreeSurface(infoSurface);

	// Set the mask texture blend mode
	maskBlendMode_ = SDL_ComposeCustomBlendMode(
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

	if (maskBuffer_)
		SDL_FreeSurface(maskBuffer_);

	if (cameraBuffer_)
		SDL_FreeSurface(cameraBuffer_);
}

void Renderer::onResize (Data data)
{
	int gameSizeWidth = data.i.get(0);
	int gameSizeHeight = data.i.get(1);
	int baseSizeWidth = data.i.get(2);
	int baseSizeHeight = data.i.get(3);

	resize(baseSizeWidth, baseSizeHeight);
}

void Renderer::resize (int width, int height)
{
	emit("SYS_RESIZE", width, height);

	width_ = width;
	height_ = height;

	// Free the mask buffer
	if (maskBuffer_)
		SDL_FreeSurface(maskBuffer_);

	// Reset it to the same size as the renderer
	maskBuffer_ = SDL_CreateTexture(
			window.renderer_,
			pixelFormat_->format,
			SDL_TEXTUREACCESS_TARGET,
			width,
			height
			);

	// Free the camera buffer
	if (cameraBuffer_)
		SDL_FreeSurface(cameraBuffer_);

	// Reset it to the same size as the renderer
	cameraBuffer_ = SDL_CreateTexture(
			window.renderer_,
			pixelFormat_->format,
			SDL_TEXTUREACCESS_TARGET,
			width,
			height
			);

	// Free the mask texture
	if (maskTexture_)
		SDL_FreeSurface(maskTexture_);

	// Reset it to the same size as the renderer
	maskTexture_ = SDL_CreateTexture(
			window.renderer_,
			pixelFormat_->format,
			SDL_TEXTUREACCESS_TARGET,
			width,
			height
			);
	// Set the mask texture's blend mode
	SDL_SetTextureBlendMode(maskTexture_, maskBlendMode_);
}

void Renderer::preRender ()
{
	if (game.config.clearBeforeRender) {
		SDL_SetRenderDrawColor(
				window.renderer_,
				backgroundColor_.red(),
				backgroundColor_.green(),
				backgroundColor_.blue(),
				0xFF);
		SDL_RenderClear(window.renderer_);
	}

	drawCount_ = 0;

	emit("SYS_PRE_RENDER");
}

void Renderer::render (Scene& scene, std::vector<GameObjects::GameObject*> children, Cameras::Scene2D::Camera& camera)
{
	emit("SYS_RENDER");

	SDL_Rect c;
	c.x = camera.x_;
	c.y = camera.y_;
	c.w = camera.width_;
	c.h = camera.height_;

	// Set a viewport if the camera isn't the same size as the window
	if (game.scene.customViewports_) {
		SDL_RenderSetViewport(window.renderer_, &c);
	}

	if (camera.mask_)
		camera.mask_->preRender(this, nullptr, &camera.maskCamera_);

	// Camera's background color if not transparent
	if (!camera.transparent_) {
		SDL_SetRenderDrawColor(
				window.renderer_,
				camera.backgroundColor_.red(),
				camera.backgroundColor_.green(),
				camera.backgroundColor_.blue(),
				camera.backgroundColor_.alpha());
		SDL_RenderFillRect(window.renderer_, &c);
	}

	drawCount_ += children.size();

	// Render the GameObject
	for (int i = 0; i < children.size(); i++) 
		children[i]->render(*this, *children[i], camera);

	camera.flashEffect.postRender();
	camera.fadeEffect.postRender();

	camera.dirty_ = false;

	if (camera.mask_)
		camera.mask_->postRender(this);

	// Remove the viewport if previously set
	if (game.scene.customViewports_)
		SDL_RenderSetViewport(window.renderer_, nullptr);

	// Update the screen
	SDL_RenderPresent(window.renderer_);
}

void Renderer::postRender ()
{
	emit("SYS_PORT_RENDER");

	if (snapshotState.active) {
		takeSnapshot();

		snapshotState.active = false;
	}
}

Renderer& Renderer::snapshot (std::function<void(SDL_Surface*)> callback, std::string path)
{
	return snapshotArea(0, 0, window.width(), window.height(), callback, path);
}

Renderer& Renderer::snapshotArea (int x, int y, int width, int height, std::function<void(SDL_Surface*)> callback, std::string path)
{
	snapshotState.callback = callback;

	snapshotState.callbackPixel = nullptr;

	snapshotState.path = path;

	snapshotState.getPixel = false;

	snapshotState.x = Math::clamp(x, 0, window.width());

	snapshotState.y = Math::clamp(y, 0, window.height());

	snapshotState.width = Math::clamp(
			width,
			0,
			window.width() - snapshotState.x);

	snapshotState.height = Math::clamp(
			height,
			0,
			window.height() - snapshotState.y);

	snapshotState.active = true;

	return *this;
}

Renderer& Renderer::snapshotPixel (int x, int y, std::function<void(Display::Color)> callback)
{
	snapshotArea(x, y, 1, 1, nullptr);

	snapshotState.getPixel = true;
	snapshotState.callbackPixel = callback;

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
		Uint32 pixel;
		int pitch = 4;
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		rect.w = 1;
		rect.h = 1;

		if (SDL_RenderReadPixels(
					window.renderer,
					&rect,
					pixelFormat_->format,
					&pixel,
					pitch
					)) {
			messageError("Failed to read the window pixel data: ", SDL_GetError());
			return;
		}

		// Get the color components of the pixel
		Uint8 r = 0, g = 0, b = 0, a = 0;
		SDL_GetRGBA(pixel, pixelFormat_, &r, &g, &b, &a);

		// Save the color components to the output color object
		Display::Color out;
		out.setTo(r, g, b, a);

		// Call the callback if one was given
		if (snapshotState.callbackPixel)
			snapshotState.callbackPixel(out);
	} else {
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
					pixelFormat_->BitsPerPixel,
					pixelFormat_->Rmask,
					pixelFormat_->Gmask,
					pixelFormat_->Bmask,
					pixelFormat_->Amask
					);
		} else {
			snapshotState.surface = SDL_CreateRGBSurface(
					0,
					width_,
					height_,
					pixelFormat_->BitsPerPixel,
					pixelFormat_->Rmask,
					pixelFormat_->Gmask,
					pixelFormat_->Bmask,
					pixelFormat_->Amask
					);
		}
		if (!snapshotState.surface) {
			messageError("Failed to create an RGB surface: ", SDL_GetError());
			return;
		}

		int readFailed = 1;

		// Read the window's pixels
		if (areaSnapshot) {
			// Read the rectangle defined in snapshotState
			SDL_Rect rect;
			rect.x = snapshotState.x;
			rect.y = snapshotState.y;
			rect.w = snapshotState.width;
			rect.h = snapshotState.height;

			readFailed = SDL_RenderReadPixels(
					window.renderer,
					&rect,
					pixelFormat_->format,
					snapshotState.surface->pixels,
					snapshotState.surface->pitch);
		} else {
			// Read the whole window
			readFailed = SDL_RenderReadPixels(
					window.renderer,
					nullptr,
					pixelFormat_->format,
					snapshotState.surface->pixels,
					snapshotState.surface->pitch);
		}

		// Check if the renderer was read normally
		if (readFailed) {
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
	std::string& path = snapshotState.path;
	std::string extension = "";

	// Figure out the file type
	for (auto c = path.rbegin(), c != path.rend(); c++) {
		if (*c == '.') {
			// We read all the extension's characters
			break;
		}

		extension.insert(0, *c);
	}

	if (extension == "bmp") {
		if (SDL_SaveBMP(snapshotState.surface, path))
			messageError("Failed to save snapshot in a 'BMP' file: ", SDL_GetError());
	} else (extension == "png") {
		if (IMG_SavePNG(snapshotState.surface, path))
			messageError("Failed to save snapshot in a 'PNG' file: ", IMG_GetError());
	} else (extension = "jpg") {
		if (IMG_SaveJPG(snapshotState.surface, path))
			messageError("Failed to save snapshot in a 'JPG' file: ", IMG_GetError());
	} else (extension == "jpeg") {
		if (IMG_SaveJPG(snapshotState.surface, path))
			messageError("Failed to save snapshot in a 'JPEG' file: ", IMG_GetError());
	} else {
		messageError("Couldn't deduce the image file type");
	}
}

void Renderer::batchSprite (
		GameObject::GameObject& sprite,
		Textures::Frame& frame,
		Cameras::Scene2D::Camera& camera,
		GameObjects::Components::TransformMatrix *parentTransformMatrix)
{
	float alpha = camera.getAlpha() * sprite.getAlpha();

	if (!alpha)
		// Nothing to see, so abort early
		return;

	auto& camMatrix = tempMatrix1_;
	auto& spriteMatrix = tempMatrix2_;

	int dd = getDrawImageData();

	int frameX = dd.x;
	int frameY = dd.y;
	int frameWidth = frame.cutWidth_;
	int frameHeight = frame.cutHeight_;
	bool customPivot = frame.customPivot_;

	float res = frame.source_.resolution_;

	int displayOriginX = sprite.getDisplayOriginX();
	int displayOriginY = sprite.getDisplayOriginY();

	int x = -displayOriginX + frame.x_;
	int y = -displayOriginY + frame.y_;

	if (sprite.isCropped) {
		auto& crop = sprite.crop_;

		if (crop.flipX != sprite.flipX_ || crop.flipY != sprite.flipY_)
			frame.updateCropUVs(crop, sprite.flipX_, sprite.flipY_);

		frameWidth = crop.cw;
		frameHeight = crop.ch;

		frameX = crop.cx;
		frameY = crop.cy;

		x = -displayOriginX + crop.x;
		y = -displayOriginY + crop.y;

		if (sprite.flipX) {
			if (x >= 0)
				x = -(x + frameWidth);
			else
				x = (std::abs(x) - frameWidth);
		}

		if (sprite.flipY) {
			if (y >= 0)
				y = -(y + frameHeight);
			else
				y = (std::abs(y) - frameHeight);
		}
	}

	int flipX = 1;
	int flipY = 1;

	if (sprite.flipX) {
		if (!customPivot)
			x += (-frame.realWidth + (displayOriginX * 2));

		flipX = -1;
	}

	if (sprite.flipY) {
		if (!customPivot)
			x += (-frame.realHeight + (displayOriginY * 2));

		flipY = -1;
	}

	spriteMatrix.applyITRS(sprite.x, sprite.y, sprite.rotation, sprite.scaleX * flipX, sprite.scaleY * flipY);

	camMatrix.copyFrom(camera.matrix);

	if (parentTransformMatrix) {
		// Multiply the camera by the parent matrix
		camMatrix.multiplyWithOffset(
				*parentTransformMatrix,
				-camera.scrollX * sprite.scrollFactorX,
				-camera.scrollY * sprite.scrollFactorY
				);

		// Undo the camera scroll
		spriteMatrix.setE(sprite.x);
		spriteMatrix.setF(sprite.y);
	} else {
		spriteMatrix.setE(
				spriteMatrix.getE() - (camera.scrollX * sprite.scrollFactorX));
		spriteMatrix.setF(
				spriteMatrix.getF() - (camera.scrollY * sprite.scrollFactorY));
	}

	// Multiply by the Sprite matrix
	camMatrix.multiply(spriteMatrix);

	// Apply the transform matrix
	// FIXME Order of operations should be: translate, rotate, scale
	// But here it is: translate, scale, rotate
	// Should either rotate after rendering, or add a decomposition method
	// that returns the appropriate transform matrix
	DecomposedMatrix dm = camMatrix.decomposeMatrix();

	setGlobalCompositionOperation(sprite.blendMode);

	if (sprite.mask)
		sprite.mask->preRender(this, &sprite, &camera, dm);

	// Render the texture
	SDL_Rect source {frameX, frameY, frameWidth, frameHeight};

	SDL_Rect destination;
	destination.x = x + dm.translateX;
	destination.y = y + dm.translateY;
	destination.w = (frameWidth / res) * dm.scaleX;
	destination.h = (frameHeight / res) * dm.scaleY;

	double angle = dm.rotation;

	SDL_Point origin {displayOriginX, displayOriginY};

	SDL_RendererFlip flip = SDL_FLIP_NONE;
	flip |= (sprite.flipX) ? SDL_FLIP_HORIZONTAL : 0;
	flip |= (sprite.flipY) ? SDL_FLIP_VERTICAL : 0;

	SDL_RenderCopyEx(
			window.renderer,
			frame.source.sdlTexture,
			&source,
			&destination,
			angle,
			&origin,
			flip
			);

	if (sprite.mask)
		sprite.mask->postRender(this, &sprite, &camera);
}

}	// namespace Zen
