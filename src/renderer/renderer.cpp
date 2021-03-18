/**
 * @file		renderer.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "renderer.h"
#include <algorithm>
#include <cmath>

namespace Zen {

Renderer::Renderer (Game& game_)
	: game(game_), window(game_.window)
{
	backgroundColor = game.config.backgroundColor;

	game.scale.on("SYS_RESIZE", &Renderer::onResize, this);

	resize(window.width(), window.height());
}

Renderer::~Renderer ()
{
	if (snapshotState.surface)
		SDL_FreeSurface(snapshotState.surface);
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
	emit("SYS_RESIZE", {{width, height}});
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
		camera.mask_->preRender(*this, nullptr, camera.maskCamera_);

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

	drawCount += children.size();

	matrix.copyFrom(camera.matrix_);

	for (int i = 0; i < children.size(); i++) {
		if (children[i]->mask_)
			children[i]->mask_->preRender(*this, *children[i], camera);

		children[i]->render(*this, *children[i], camera);

		if (children[i]->mask_)
			children[i]->mask_->postRender(*this, *children[i], camera);
	}

	camera.flashEffect.postRender();
	camera.fadeEffect.postRender();

	camera.dirty_ = false;

	if (camera.mask_)
		camera.mask_->postRender(*this);

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

		// Get the window's surface
		SDL_Surface *infoSurface = nullptr;
		infoSurface = SDL_GetWindowSurface(window.window);
		if (!infoSurface) {
			messageError("Failed to create info surface from window: ", SDL_GetError());
			return;
		}

		if (SDL_RenderReadPixels(
					window.renderer,
					&rect,
					infoSurface->format->format,
					&pixel,
					pitch
					)) {
			messageError("Failed to read the window pixel data: ", SDL_GetError());
			SDL_FreeSurface(infoSurface);
			return;
		}

		// Get the color components of the pixel
		Uint8 r = 0, g = 0, b = 0, a = 0;
		SDL_GetRGBA(pixel, infoSurface->format, &r, &g, &b, &a);

		SDL_FreeSurface(infoSurface);

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

		SDL_Surface infoSurface = nullptr;
		infoSurface = SDL_GetWindowSurface(window.window);
		if (!infoSurface) {
			messageError("Failed to create info surface from window: ", SDL_GetError());
			return;
		}

		// Create a blank surface
		if (areaSnapshot) {
			snapshotState.surface = SDL_CreateRGBSurface(
					0,
					snapshotState.width,
					snapshotState.height,
					infoSurface->format->BitsPerPixel,
					infoSurface->format->Rmask,
					infoSurface->format->Gmask,
					infoSurface->format->Bmask,
					infoSurface->format->Amask
					);
		} else {
			snapshotState.surface = SDL_CreateRGBSurface(
					0,
					infoSurface->w,
					infoSurface->h,
					infoSurface->format->BitsPerPixel,
					infoSurface->format->Rmask,
					infoSurface->format->Gmask,
					infoSurface->format->Bmask,
					infoSurface->format->Amask
					);
		}
		if (!snapshotState.surface) {
			messageError("Failed to create an RGB surface: ", SDL_GetError());
			SDL_FreeSurface(infoSurface);
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
					infoSurface->format->format,
					snapshotState.surface->pixels,
					snapshotState.surface->pitch);
		} else {
			// Read the whole window
			readFailed = SDL_RenderReadPixels(
					window.renderer,
					nullptr,
					infoSurface->format->format,
					snapshotState.surface->pixels,
					snapshotState.surface->pitch);
		}

		// Check if the renderer was read normally
		if (readFailed) {
			messageError("Failed to read the window pixel data: ", SDL_GetError());
			SDL_FreeSurface(snapshotState.surface);
			SDL_FreeSurface(infoSurface);
			return;
		}

		SDL_FreeSurface(infoSurface);

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
		sprite.mask.preRender(*this, sprite, camera);

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
		sprite.mask.postRender(*this, sprite, camera);
}

}	// namespace Zen
