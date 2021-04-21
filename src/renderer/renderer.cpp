/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "renderer.hpp"

#include "../window/window.hpp"
#include "../scale/scale_manager.hpp"
#include "../scene/scene.h"

#include "../structs/types/size.hpp"
#include "../math/rad_to_deg.hpp"
#include "../math/deg_to_rad.hpp"
#include "../math/clamp.hpp"
#include "../utils/messages.hpp"
#include "../core/config.hpp"

#include "../systems/position.hpp"
#include "../systems/size.hpp"
#include "../systems/viewport.hpp"
#include "../systems/mask.hpp"
#include "../systems/transparent.hpp"
#include "../systems/background_color.hpp"
#include "../systems/alpha.hpp"
#include "../systems/dirty.hpp"
#include "../systems/origin.hpp"
#include "../systems/textured.hpp"
#include "../systems/flip.hpp"
#include "../systems/transform_matrix.hpp"
#include "../systems/scale.hpp"
#include "../systems/rotation.hpp"
#include "../systems/scroll_factor.hpp"
#include "../systems/scroll.hpp"
#include "../systems/tint.hpp"
#include "../systems/blend_mode.hpp"
#include "../display/color.hpp"
#include "../texture/systems/frame.hpp"
#include "../texture/components/frame.hpp"
#include "../texture/components/source.hpp"
#include "../cameras/2d/systems/camera.hpp"

namespace Zen {

extern entt::registry g_registry;
extern Window g_window;
extern ScaleManager g_scale;

Renderer::~Renderer ()
{
	/*
	 * Destroying the renderer is enough to clean all textures
	if (snapshotState.surface)
		SDL_FreeSurface(snapshotState.surface);

	if (maskBuffer)
		SDL_DestroyTexture(maskBuffer);

	if (cameraBuffer)
		SDL_DestroyTexture(cameraBuffer);

	if (maskTexture)
		SDL_DestroyTexture(maskTexture);
	*/
}

void Renderer::start (GameConfig *cfg_)
{
	config = cfg_;

	backgroundColor = config->backgroundColor;

	g_scale.on("resize", &Renderer::onResize, this);

	// Set the mask texture blend mode
	maskBlendMode = SDL_ComposeCustomBlendMode(
			SDL_BLENDFACTOR_ZERO,			// Zero out the mask color
			SDL_BLENDFACTOR_ONE,			// Keep the masked color
			SDL_BLENDOPERATION_ADD,
			SDL_BLENDFACTOR_ZERO,			// Zero out the mask alpha
			SDL_BLENDFACTOR_SRC_ALPHA,		// Multiply masked alpha by mask alpha
			SDL_BLENDOPERATION_ADD
			);

	// Create the supported blend modes
	createBlendModes();

	resize(g_window.width(), g_window.height());
}

void Renderer::onResize (Size gameSize_, Size displaySize_, int previousWidth_, int previousHeight_)
{
	resize(gameSize_.width, gameSize_.height);
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
			g_window.renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			width,
			height
			);

	// Enable transparency
	SDL_SetTextureBlendMode(maskBuffer, SDL_BLENDMODE_BLEND);

	// Free the camera buffer
	if (cameraBuffer)
		SDL_DestroyTexture(cameraBuffer);

	// Reset it to the same size as the renderer
	cameraBuffer = SDL_CreateTexture(
			g_window.renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			width,
			height
			);

	// Enable transparency
	SDL_SetTextureBlendMode(maskBuffer, SDL_BLENDMODE_BLEND);

	// Free the mask texture
	if (maskTexture)
		SDL_DestroyTexture(maskTexture);

	// Reset it to the same size as the renderer
	maskTexture = SDL_CreateTexture(
			g_window.renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			width,
			height
			);

	// Set the mask texture's blend mode
	SDL_SetTextureBlendMode(maskTexture, maskBlendMode);
}

void Renderer::preRender ()
{
	if (config->clearBeforeRender)
	{
		SDL_SetRenderDrawColor(
				g_window.renderer,
				backgroundColor.red,
				backgroundColor.green,
				backgroundColor.blue,
				0xff);
		SDL_RenderClear(g_window.renderer);
	}

	drawCount = 0;

	emit("pre-render");
}

void Renderer::render (
		Scene& scene_,
		std::vector<Entity> children_,
		Entity camera_)
{
	emit("render");

	int offsetX_ = g_scale.displayOffset.x;
	int offsetY_ = g_scale.displayOffset.y;
	double scaleX_ = g_scale.displayScale.x;
	double scaleY_ = g_scale.displayScale.y;

	SDL_Rect c_;
	c_.x = GetX(camera_);
	c_.y = GetY(camera_);
	c_.w = GetWidth(camera_);
	c_.h = GetHeight(camera_);

	if (GetViewport(camera_) || g_scale.scaleMode != SCALE_MODE::RESIZE)
	{
		// Skip rendering this camera if its viewport is outside the window
		if (c_.x > width || c_.y > height || c_.x < -c_.w || c_.y < -c_.h)
			return;

		// Clip the viewport if it goes outside the left side of the window
		if (c_.x < 0)
		{
			c_.w += c_.x;
			c_.x = 0;
		}

		// Clip the viewport if it goes outside the right side of the window
		if ((c_.x + c_.w) > width)
		{
			c_.w = width - c_.x;
		}

		// Clip the viewport if it goes outside the top side of the window
		if (c_.y < 0)
		{
			c_.h += c_.y;
			c_.y = 0;
		}

		// Clip the viewport if it goes outside the bottom side of the window
		if ((c_.y + c_.h) > height)
		{
			c_.h = height - c_.y;
		}
	}

	c_.x *= scaleX_;
	c_.y *= scaleY_;
	c_.w *= scaleX_;
	c_.h *= scaleY_;
	c_.x += offsetX_;
	c_.y += offsetY_;

	// Clip the renderer
	if (GetViewport(camera_) || g_scale.scaleMode != SCALE_MODE::RESIZE)
	{
		SDL_RenderSetClipRect(g_window.renderer, &c_);
	}

	if (GetMask(camera_) != entt::null)
		preRenderMask(entt::null);

	// Camera's background color if not transparent
	if (!IsTransparent(camera_)) {
		SDL_SetRenderDrawBlendMode(g_window.renderer, SDL_BLENDMODE_BLEND);

		auto bgc = GetBackgroundColor(camera_);

		SDL_SetRenderDrawColor(
			g_window.renderer,
			bgc.red,
			bgc.green,
			bgc.blue,
			bgc.alpha * GetAlpha(camera_)
		);

		SDL_RenderFillRect(g_window.renderer, &c_);

		// Reset draw blend mode
		SDL_SetRenderDrawBlendMode(g_window.renderer, SDL_BLENDMODE_NONE);
	}

	drawCount += children_.size();

	// Render the GameObject
	for (auto& child_ : children_)
	{
		AddToRenderList(camera_, child_);

		if (child_ != entt::null && GetFrame(child_) != entt::null)
			batchSprite(child_, GetFrame(child_), camera_, GetParentTransformMatrix(child_));
	}

	//camera_.flashEffect.postRender();
	//camera_.fadeEffect.postRender();

	SetDirty(camera_, false);

	if (GetMask(camera_) != entt::null)
		postRenderMask(GetMask(camera_), entt::null, camera_);

	// Remove the viewport if previously set
	if (GetViewport(camera_))
	{
		SDL_RenderSetClipRect(g_window.renderer, nullptr);
	}
}

void Renderer::postRender ()
{
	// Update the screen once every element has been rendered
	// This will also trigger a delay as the renderer is Vsynced
	SDL_RenderPresent(g_window.renderer);

	emit("post-render");

	if (snapshotState.active)
	{
		takeSnapshot();

		snapshotState.active = false;
	}
}

Renderer& Renderer::snapshot (std::string path_, std::function<void(SDL_Surface*)> callback_)
{
	return snapshotArea(0, 0, g_window.width(), g_window.height(), path_, callback_);
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

	snapshotState.x = Math::Clamp(x_, 0, g_window.width());

	snapshotState.y = Math::Clamp(y_, 0, g_window.height());

	snapshotState.width = Math::Clamp(
			width_,
			0,
			g_window.width() - snapshotState.x);

	snapshotState.height = Math::Clamp(
			height_,
			0,
			g_window.height() - snapshotState.y);

	snapshotState.active = true;

	return *this;
}

Renderer& Renderer::snapshotPixel (int x_, int y_, std::function<void(Color)>& callback_)
{
	snapshotArea(x_, y_, 1, 1, "", nullptr);

	snapshotState.getPixel = true;
	snapshotState.callbackPixel = callback_;

	return *this;
}

Renderer& Renderer::snapshotPixel (int x_, int y_, std::function<void(Color)>&& callback_)
{
	return snapshotPixel(x_, y_, callback_);
}

void Renderer::takeSnapshot ()
{
	if (!snapshotState.active)
		return;

	if (snapshotState.surface) {
		SDL_FreeSurface(snapshotState.surface);
		snapshotState.surface = nullptr;
	}

	// Get the pixel format of the window
	SDL_Surface *infoSurface_ = nullptr;

	infoSurface_ = SDL_GetWindowSurface(g_window.window);

	if (!infoSurface_) {
		MessageError("Failed to create info surface from window: ", SDL_GetError());
		return;
	}

	SDL_PixelFormat pixelFormat_ = *infoSurface_->format;

	SDL_FreeSurface(infoSurface_);


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
					g_window.renderer,
					&rect_,
					pixelFormat_.format,
					&pixel_,
					pitch_
					))
		{
			MessageError("Failed to read the window pixel data: ", SDL_GetError());
			return;
		}

		// Get the color components of the pixel
		Uint8 r_ = 0, g_ = 0, b_ = 0, a_ = 0;
		SDL_GetRGBA(pixel_, &pixelFormat_, &r_, &g_, &b_, &a_);

		// Save the color components to the output color object
		Color out_;
		SetTo(&out_, r_, g_, b_, a_);

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
				snapshotState.width != g_window.width() ||
				snapshotState.height != g_window.height()
				);

		// Create a blank surface
		if (areaSnapshot) {
			snapshotState.surface = SDL_CreateRGBSurface(
					0,
					snapshotState.width,
					snapshotState.height,
					pixelFormat_.BitsPerPixel,
					pixelFormat_.Rmask,
					pixelFormat_.Gmask,
					pixelFormat_.Bmask,
					pixelFormat_.Amask
					);
		} else {
			snapshotState.surface = SDL_CreateRGBSurface(
					0,
					g_window.width(),
					g_window.height(),
					pixelFormat_.BitsPerPixel,
					pixelFormat_.Rmask,
					pixelFormat_.Gmask,
					pixelFormat_.Bmask,
					pixelFormat_.Amask
					);
		}
		if (!snapshotState.surface) {
			MessageError("Failed to create an RGB surface: ", SDL_GetError());
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

			MessageNote("area");

			readFailed_ = SDL_RenderReadPixels(
					g_window.renderer,
					&rect_,
					pixelFormat_.format,
					snapshotState.surface->pixels,
					snapshotState.surface->pitch);
		} else {
			// Read the whole window
			readFailed_ = SDL_RenderReadPixels(
					g_window.renderer,
					nullptr,
					pixelFormat_.format,
					snapshotState.surface->pixels,
					snapshotState.surface->pitch);
		}

		// Check if the renderer was read normally
		if (readFailed_) {
			MessageError("Failed to read the window pixel data: ", SDL_GetError());
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
			MessageError("Failed to save snapshot in a 'BMP' file: ", SDL_GetError());
	}
	else if (extension_ == "png")
	{
		if (IMG_SavePNG(snapshotState.surface, path_.c_str()))
			MessageError("Failed to save snapshot in a 'PNG' file: ", IMG_GetError());
	}
	else if (extension_ == "jpg")
	{
		if (IMG_SaveJPG(snapshotState.surface, path_.c_str(), 100))
			MessageError("Failed to save snapshot in a 'JPG' file: ", IMG_GetError());
	}
	else if (extension_ == "jpeg")
	{
		if (IMG_SaveJPG(snapshotState.surface, path_.c_str(), 100))
			MessageError("Failed to save snapshot in a 'JPEG' file: ", IMG_GetError());
	}
	else
	{
		MessageError("File type unsupported! Try something else like 'png' or 'jpg'.");
	}
}

void Renderer::batchSprite (
		Entity sprite_,
		Entity frame_,
		Entity camera_,
		Components::TransformMatrix *parentTransformMatrix_)
{
	double alpha_ = GetAlpha(camera_) * GetAlpha(sprite_);

	if (!alpha_)
		// Nothing to see, so abort early
		return;

	auto& camMatrix_ = tempMatrix1;
	auto& spriteMatrix_ = tempMatrix2;

	Rectangle dd_ = GetFrameDrawImageData(frame_);

	auto cut = GetFrameCut(frame_);

	// FIXME Stop cheating and implement correct components to frames
	auto frameCheat___ = g_registry.get<Components::Frame>(frame_);

	double frameX_ = dd_.x;
	double frameY_ = dd_.y;
	double frameWidth_;
	double frameHeight_;

	if (frameCheat___.rotated)
	{
		frameWidth_ = cut.height;
		frameHeight_ = cut.width;
	}
	else
	{
		frameWidth_ = cut.width;
		frameHeight_ = cut.height;
	}

	// FIXME AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	double res_ = 1.0; // frame___.source->resolution;

	double displayOriginX_ = GetDisplayOriginX(sprite_);
	double displayOriginY_ = GetDisplayOriginY(sprite_);

	double x_ = (-1. * displayOriginX_) + frameCheat___.data.spriteSourceSize.x;
	double y_ = (-1. * displayOriginY_) + frameCheat___.data.spriteSourceSize.y;

	if (IsCropped(sprite_))
	{
		auto crop_ = GetCrop(sprite_);

		if (crop_.flipX != GetFlipX(sprite_) || crop_.flipY != GetFlipY(sprite_))
			UpdateFrameCropUVs(frame_, &crop_, GetFlipX(sprite_), GetFlipY(sprite_));

		frameWidth_ = crop_.cw;
		frameHeight_ = crop_.ch;

		frameX_ = crop_.cx;
		frameY_ = crop_.cy;

		x_ = -displayOriginX_ + crop_.x;
		y_ = -displayOriginY_ + crop_.y;

		if (GetFlipX(sprite_)) {
			if (x_ >= 0)
				x_ = -(x_ + frameWidth_);
			else
				x_ = (std::abs(x_) - frameWidth_);
		}

		if (GetFlipY(sprite_)) {
			if (y_ >= 0)
				y_ = -(y_ + frameHeight_);
			else
				y_ = (std::abs(y_) - frameHeight_);
		}
	}

	bool flipX_ = GetFlipX(sprite_);
	bool flipY_ = GetFlipY(sprite_);

	if (!frameCheat___.rotated) {
		ApplyITRS(&spriteMatrix_,
			GetX(sprite_) + x_, GetY(sprite_) + y_,
			GetRotation(sprite_),
			GetScaleX(sprite_), GetScaleY(sprite_)
		);
	}
	else {
		ApplyITRS(&spriteMatrix_,
			GetX(sprite_) + x_, GetY(sprite_) + y_ + frameCheat___.height,
			GetRotation(sprite_) + Math::DegToRad(-90),
			GetScaleX(sprite_), GetScaleY(sprite_)
		);
	}

	/*
	LoadIdentity(&spriteMatrix_);
	Translate(&spriteMatrix_, GetX(sprite_) + x_, GetY(sprite_) + y_);

	if (frameCheat___.rotated) {
		Translate(&spriteMatrix_, 0, frameCheat___.height);
		Rotate(&spriteMatrix_, Math::DegToRad(-90));
	}

	Rotate(&spriteMatrix_, GetRotation(sprite_));
	Scale(&spriteMatrix_, GetScaleX(sprite_), GetScaleY(sprite_));
	*/

	camMatrix_ = GetTransformMatrix(camera_);

	if (parentTransformMatrix_)
	{
		// Multiply the camera by the parent matrix
		MultiplyWithOffset(
				&camMatrix_,
				*parentTransformMatrix_,
				-GetScrollX(camera_) * GetScrollFactorX(sprite_),
				-GetScrollY(camera_) * GetScrollFactorY(sprite_)
				);

		// Undo the camera scroll
		spriteMatrix_.e = GetX(sprite_);
		spriteMatrix_.f = GetY(sprite_);
	}
	else
	{
		spriteMatrix_.e -= GetScrollX(camera_) * GetScrollFactorX(sprite_);
		spriteMatrix_.f -= GetScrollY(camera_) * GetScrollFactorY(sprite_);
	}

	// Multiply by the Sprite matrix
	Multiply(&camMatrix_, spriteMatrix_);

	// Decompose the transform matrix
	DecomposedMatrix dm_ = DecomposeMatrix(camMatrix_);

	if (GetMask(sprite_) != entt::null)
		preRenderMask(sprite_);

	// Render the texture
	SDL_Rect source_;
	source_.x = frameX_;
	source_.y = frameY_;
	source_.w = frameWidth_;
	source_.h = frameHeight_;

	// ScaleManager values
	Math::Vector2 sScale_ = g_scale.displayScale;
	Math::Vector2 sOffset_ = g_scale.displayOffset;

	SDL_FRect destination_;

	// Position
	destination_.x = dm_.translateX * sScale_.x + sOffset_.x;
	destination_.y = dm_.translateY * sScale_.y + sOffset_.y;

	// Scale
	destination_.w = (frameWidth_ / res_) * dm_.scaleX * sScale_.x;
	destination_.h = (frameHeight_ / res_) * dm_.scaleY * sScale_.y;

	// Rotation
	double angle_ = Math::RadToDeg(dm_.rotation);

	// Origin (Already taken care of with the transform matrices)
	// Set explicitly to '0' as the default is the middle of the sprite.
	SDL_FPoint origin_ {0.f, 0.f};

	// Flip
	SDL_RendererFlip flip_ = SDL_FLIP_NONE;
	if (flipX_ && flipY_)
	{
		flip_ = (SDL_RendererFlip)(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL);
	}
	else if (flipX_)
	{
		flip_ = (SDL_RendererFlip)SDL_FLIP_HORIZONTAL;
	}
	else if (flipY_)
	{
		flip_ = (SDL_RendererFlip)SDL_FLIP_VERTICAL;
	}

	// FIXME AAAAAAAAAAAAAAAAAAAAAa
	auto source___ = g_registry.get<Components::TextureSource>(frameCheat___.source);

	// Tint (Color Modulation)
	if (IsTinted(sprite_))
	{
		Color tint_ =  GetTint(sprite_);

		SDL_SetTextureColorMod(
			source___.sdlTexture,
			tint_.red,
			tint_.green,
			tint_.blue
			);
	}

	// Alpha (Transparency)
	if (alpha_ < 1.0)
	{
		SDL_SetTextureBlendMode(
			source___.sdlTexture,
			SDL_BLENDMODE_BLEND
			);
		SDL_SetTextureAlphaMod(
			source___.sdlTexture,
			alpha_ * 255
			);
	}

	// Blending
	if (GetBlendMode(sprite_) != BLEND_MODE::NORMAL)
	{
		SDL_SetTextureBlendMode(
			source___.sdlTexture,
			blendModes[GetBlendMode(sprite_)]
			);
	}

	SDL_RenderCopyExF(
			g_window.renderer,
			source___.sdlTexture,
			&source_,
			&destination_,
			angle_,
			&origin_,
			flip_
			);

	// Clear blending
	if (GetBlendMode(sprite_) != BLEND_MODE::NORMAL)
	{
		SDL_SetTextureBlendMode(
			source___.sdlTexture,
			blendModes[BLEND_MODE::NORMAL]
			);
	}

	// Clear alpha from texture
	if (alpha_ < 1.0)
	{
		SDL_SetTextureAlphaMod(
			source___.sdlTexture,
			255
			);
	}

	// Clear tint from texture
	if (IsTinted(sprite_))
	{
		SDL_SetTextureColorMod(
			source___.sdlTexture,
			0xff,
			0xff,
			0xff
			);
	}

	if (GetMask(sprite_) != entt::null)
		postRenderMask(GetMask(sprite_), sprite_, camera_);
}

void Renderer::preRenderMask (Entity maskedObject_)
{
	// Is this a camera mask?
	if (maskedObject_ != entt::null)
	{
		SDL_SetRenderTarget(g_window.renderer, cameraBuffer);
	}
	else
	{
		SDL_SetRenderTarget(g_window.renderer, maskBuffer);
	}

	// Clear _AFTER_ setting the target, to clear the buffer and not the screen
	SDL_SetRenderDrawColor(g_window.renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(g_window.renderer);
}

void Renderer::postRenderMask (
		Entity maskObject_,
		Entity maskedObject_,
		Entity camera_)
{
	// Save the target buffer
	SDL_Texture *currentTarget_ = SDL_GetRenderTarget(g_window.renderer);

	// Set the mask texture as the new render target
	SDL_SetRenderTarget(g_window.renderer, maskTexture);

	// Clear the mask texture
	SDL_SetRenderDrawColor(g_window.renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(g_window.renderer);

	// Draw the mask GameObject
	AddToRenderList(camera_, maskObject_);
	batchSprite(maskObject_, GetFrame(maskObject_), camera_, GetParentTransformMatrix(maskObject_));

	// Reset the target to the buffer
	SDL_SetRenderTarget(g_window.renderer, currentTarget_);

	// Render the mask on the buffer
	SDL_RenderCopy(
			g_window.renderer,
			maskTexture,
			nullptr,	// Render the whole texture
			nullptr		// Render to the entire target
			);

	// Is this a camera mask?
	if (maskedObject_ != entt::null)
	{
		// Reset the rendering target
		SDL_SetRenderTarget(g_window.renderer, nullptr);

		SDL_RenderCopy(
				g_window.renderer,
				cameraBuffer,
				nullptr,	// Render the whole texture
				nullptr		// Render to the entire target
				);
	}
	else
	{
		// Is a camera mask active? If so, set it back to be the rendering target
		if (GetMask(camera_) != entt::null)
			SDL_SetRenderTarget(g_window.renderer, cameraBuffer);
		else
			SDL_SetRenderTarget(g_window.renderer, nullptr);

		SDL_RenderCopy(
				g_window.renderer,
				maskBuffer,
				nullptr,	// Render the whole texture
				nullptr		// Render to the entire target
				);
	}
}

}	// namespace Zen
