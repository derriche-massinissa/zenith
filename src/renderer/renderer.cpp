/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "renderer.hpp"

#include "../window/window.hpp"
#include "../scale/scale_manager.hpp"
#include "../scene/scene.hpp"
#include "../scene/scene_manager.hpp"

#include "../structs/types/size.hpp"
#include "../math/rad_to_deg.hpp"
#include "../math/deg_to_rad.hpp"
#include "../math/clamp.hpp"
#include "../math/pow2/is_size_power_of_two.hpp"
#include "utility.hpp"
#include "../utils/messages.hpp"
#include "../utils/vector/remove.hpp"
#include "../utils/map/emplace.hpp"
#include "../core/config.hpp"
#include "events/events.hpp"

#include "../cameras/2d/systems/fade.hpp"

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
#include "../systems/text.hpp"
#include "../systems/mask.hpp"
#include "../systems/type.hpp"
#include "../systems/renderable.hpp"
#include "../display/color.hpp"
#include "../texture/systems/frame.hpp"
#include "../texture/components/frame.hpp"
#include "../texture/components/source.hpp"
#include "../components/mask.hpp"
#include "../components/masked.hpp"
#include "../cameras/2d/systems/camera.hpp"
#include "../text/text_manager.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Zen {

extern entt::registry g_registry;
extern Window g_window;
extern ScaleManager g_scale;
extern TextManager g_text;
extern SceneManager g_scene;

//void Renderer::start (GameConfig *cfg_)
//{
//	config = cfg_;
//
//	backgroundColor = config->backgroundColor;
//
//	g_scale.on("resize", &Renderer::onResize, this);
//
//	// Set the mask texture blend mode
//	maskBlendMode = SDL_ComposeCustomBlendMode(
//			SDL_BLENDFACTOR_ZERO,			// Zero out the mask color
//			SDL_BLENDFACTOR_ONE,			// Keep the masked color
//			SDL_BLENDOPERATION_ADD,
//			SDL_BLENDFACTOR_ZERO,			// Zero out the mask alpha
//			SDL_BLENDFACTOR_SRC_ALPHA,		// Multiply masked alpha by mask alpha
//			SDL_BLENDOPERATION_ADD
//			);
//
//	// Create the supported blend modes
//	createBlendModes();
//
//	resize(g_window.width(), g_window.height());
//}

//void Renderer::render (
//		std::vector<Entity> children_,
//		Entity camera_)
//{
//	emit("render");
//
//	int offsetX_ = g_scale.displayOffset.x;
//	int offsetY_ = g_scale.displayOffset.y;
//	double scaleX_ = g_scale.displayScale.x;
//	double scaleY_ = g_scale.displayScale.y;
//
//	SDL_Rect c_;
//	c_.x = GetX(camera_);
//	c_.y = GetY(camera_);
//	c_.w = GetWidth(camera_);
//	c_.h = GetHeight(camera_);
//
//	if (GetViewport(camera_) || g_scale.scaleMode != SCALE_MODE::RESIZE)
//	{
//		// Skip rendering this camera if its viewport is outside the window
//		if (c_.x > width || c_.y > height || c_.x < -c_.w || c_.y < -c_.h)
//			return;
//
//		// Clip the viewport if it goes outside the left side of the window
//		if (c_.x < 0)
//		{
//			c_.w += c_.x;
//			c_.x = 0;
//		}
//
//		// Clip the viewport if it goes outside the right side of the window
//		if ((c_.x + c_.w) > width)
//		{
//			c_.w = width - c_.x;
//		}
//
//		// Clip the viewport if it goes outside the top side of the window
//		if (c_.y < 0)
//		{
//			c_.h += c_.y;
//			c_.y = 0;
//		}
//
//		// Clip the viewport if it goes outside the bottom side of the window
//		if ((c_.y + c_.h) > height)
//		{
//			c_.h = height - c_.y;
//		}
//	}
//
//	c_.x *= scaleX_;
//	c_.y *= scaleY_;
//	c_.w *= scaleX_;
//	c_.h *= scaleY_;
//	c_.x += offsetX_;
//	c_.y += offsetY_;
//
//	// Clip the renderer
//	if (GetViewport(camera_) || g_scale.scaleMode != SCALE_MODE::RESIZE)
//	{
//		SDL_RenderSetClipRect(g_window.renderer, &c_);
//	}
//
//	if (GetMask(camera_) != entt::null)
//		preRenderMask(entt::null);
//
//	// Camera's background color if not transparent
//	if (!IsTransparent(camera_)) {
//		SDL_SetRenderDrawBlendMode(g_window.renderer, SDL_BLENDMODE_BLEND);
//
//		auto bgc = GetBackgroundColor(camera_);
//
//		SDL_SetRenderDrawColor(
//			g_window.renderer,
//			bgc.red,
//			bgc.green,
//			bgc.blue,
//			bgc.alpha * GetAlpha(camera_)
//		);
//
//		SDL_RenderFillRect(g_window.renderer, &c_);
//
//		// Reset draw blend mode
//		SDL_SetRenderDrawBlendMode(g_window.renderer, SDL_BLENDMODE_NONE);
//	}
//
//	drawCount += children_.size();
//
//	// Render the GameObject
//	for (auto& child_ : children_)
//	{
//		AddToRenderList(camera_, child_);
//
//		// Text object
//		if (IsText(child_)) {
//			g_text.render(child_, camera_);
//			continue;
//		}
//
//		// Textured object (image)
//		if (child_ != entt::null && GetFrame(child_) != entt::null)
//			batchSprite(child_, GetFrame(child_), camera_, GetParentTransformMatrix(child_));
//	}
//
//	//camera_.flashEffect.postRender();
//	PostRenderFade(camera_);
//
//	SetDirty(camera_, false);
//
//	if (GetMask(camera_) != entt::null)
//		postRenderMask(GetMask(camera_), entt::null, camera_);
//
//	// Remove the viewport if previously set
//	if (GetViewport(camera_))
//	{
//		SDL_RenderSetClipRect(g_window.renderer, nullptr);
//	}
//}

//void Renderer::batchSprite (
//		Entity sprite_,
//		Entity frame_,
//		Entity camera_,
//		Components::TransformMatrix *parentTransformMatrix_)
//{
//	double alpha_ = GetAlpha(camera_) * GetAlpha(sprite_);
//
//	if (!alpha_)
//		// Nothing to see, so abort early
//		return;
//
//	auto& camMatrix_ = tempMatrix1;
//	auto& spriteMatrix_ = tempMatrix2;
//
//	Rectangle dd_ = GetFrameDrawImageData(frame_);
//
//	auto cut = GetFrameCut(frame_);
//
//	// FIXME Stop cheating and implement correct components to frames
//	auto frameCheat___ = g_registry.get<Components::Frame>(frame_);
//
//	double frameX_ = dd_.x;
//	double frameY_ = dd_.y;
//	double frameWidth_;
//	double frameHeight_;
//
//	if (frameCheat___.rotated)
//	{
//		frameWidth_ = cut.height;
//		frameHeight_ = cut.width;
//	}
//	else
//	{
//		frameWidth_ = cut.width;
//		frameHeight_ = cut.height;
//	}
//
//	// FIXME AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
//	double res_ = 1.0; // frame___.source->resolution;
//
//	double displayOriginX_ = GetDisplayOriginX(sprite_);
//	double displayOriginY_ = GetDisplayOriginY(sprite_);
//
//	// Position
//	double x_, y_;
//
//	// Sprite rotation
//	double rot_ = GetRotation(sprite_);
//
//	// Is the frame rotated in the texture atlas?
//	if (!frameCheat___.rotated) {
//		x_ = -displayOriginX_ + frameCheat___.data.spriteSourceSize.x;
//		y_ = -displayOriginY_ + frameCheat___.data.spriteSourceSize.y;
//	} else {
//		// Restore frame to upright position
//		rot_ += Math::DegToRad(-90);
//
//		// Invert `x` and `y` axis for origin and invert y axis (No minus for x_)
//		x_ = displayOriginY_;
//		y_ = -displayOriginX_;
//
//		// A trimmed frame has the following metrics:
//		// - Width of frame
//		// - Height of frame
//		// - X position in atlas
//		// - Y position in atlas
//		// - Real dimensions of image before trimming
//		//		- Real width
//		// 		- Real height
//		// - Trimmed empty space
//		//		- Padding Left
//		// 		- Padding Up
//		// 		- Padding Right
//		// 		- Padding Bottom
//
//		// Frame is rotated -90deg on atlas (clockwise)
//		// First we restore it upward by rotating it by -90deg
//		// By doing this, the following happens:
//		// - The originaly trimX or left padding becomes y's padding or top padding
//		// - The originaly paddingV or bottom padding becomes x's padding or
//		//		left padding
//
//		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//		// |                                  |                 ^
//		// |                                  |               trimX
//		// |                                  |                 v
//		// |           +--------------+       |              +------+
//		// |           |              |       |              |  R   |
//		// | < trimX > |   Original   |       |              |  o   |
//		// |           |              |       | < paddingB > |  t   |
//		// |           +--------------+       |              |  a   |
//		// |                  ^               |              |  t   |
//		// |               paddingB           |              +------+
//		// |                  v               |
//		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//		double bottomPadding =
//			frameCheat___.data.sourceSize.height -				// Original height
//			frameCheat___.data.spriteSourceSize.height -		// Non empty height
//			frameCheat___.data.spriteSourceSize.y;				// Top trimmed padding
//
//		x_ += bottomPadding - frameCheat___.data.sourceSize.height;
//		y_ += frameCheat___.data.spriteSourceSize.x;
//	}
//
//	if (IsCropped(sprite_))
//	{
//		auto crop_ = GetCrop(sprite_);
//
//		if (crop_.flipX != GetFlipX(sprite_) || crop_.flipY != GetFlipY(sprite_))
//			UpdateFrameCropUVs(frame_, &crop_, GetFlipX(sprite_), GetFlipY(sprite_));
//
//		frameWidth_ = crop_.cw;
//		frameHeight_ = crop_.ch;
//
//		frameX_ = crop_.cx;
//		frameY_ = crop_.cy;
//
//		x_ = -displayOriginX_ + crop_.x;
//		y_ = -displayOriginY_ + crop_.y;
//
//		if (GetFlipX(sprite_)) {
//			if (x_ >= 0)
//				x_ = -(x_ + frameWidth_);
//			else
//				x_ = (std::abs(x_) - frameWidth_);
//		}
//
//		if (GetFlipY(sprite_)) {
//			if (y_ >= 0)
//				y_ = -(y_ + frameHeight_);
//			else
//				y_ = (std::abs(y_) - frameHeight_);
//		}
//	}
//
//	bool flipX_ = GetFlipX(sprite_);
//	bool flipY_ = GetFlipY(sprite_);
//
//	// Convert transformations to matrix
//	ApplyITRS(&spriteMatrix_,
//		GetX(sprite_), GetY(sprite_),
//		rot_,
//		GetScaleX(sprite_), GetScaleY(sprite_)
//	);
//
//	// Translate to take origin and trimming into account
//	Translate(&spriteMatrix_, x_, y_);
//
//	// Get the camera transform matrix
//	camMatrix_ = GetTransformMatrix(camera_);
//
//	// Is the game object a member of a container
//	if (parentTransformMatrix_)
//	{
//		// Multiply the camera by the parent matrix
//		MultiplyWithOffset(
//				&camMatrix_,
//				*parentTransformMatrix_,
//				-GetScrollX(camera_) * GetScrollFactorX(sprite_),
//				-GetScrollY(camera_) * GetScrollFactorY(sprite_)
//				);
//
//		// Undo the camera scroll
//		spriteMatrix_.e = GetX(sprite_);
//		spriteMatrix_.f = GetY(sprite_);
//	}
//	else
//	{
//		spriteMatrix_.e -= GetScrollX(camera_) * GetScrollFactorX(sprite_);
//		spriteMatrix_.f -= GetScrollY(camera_) * GetScrollFactorY(sprite_);
//	}
//
//	// Multiply by the Sprite matrix
//	Multiply(&camMatrix_, spriteMatrix_);
//
//	// Decompose the transform matrix
//	DecomposedMatrix dm_ = DecomposeMatrix(camMatrix_);
//
//	if (GetMask(sprite_) != entt::null)
//		preRenderMask(sprite_);
//
//	// Render the texture
//	SDL_Rect source_;
//	source_.x = frameX_;
//	source_.y = frameY_;
//	source_.w = frameWidth_;
//	source_.h = frameHeight_;
//
//	// ScaleManager values
//	Math::Vector2 sScale_ = g_scale.displayScale;
//	Math::Vector2 sOffset_ = g_scale.displayOffset;
//
//	SDL_FRect destination_;
//
//	// Position
//	destination_.x = dm_.translateX * sScale_.x + sOffset_.x;
//	destination_.y = dm_.translateY * sScale_.y + sOffset_.y;
//
//	// Scale
//	destination_.w = (frameWidth_ / res_) * dm_.scaleX * sScale_.x;
//	destination_.h = (frameHeight_ / res_) * dm_.scaleY * sScale_.y;
//
//	// Rotation
//	double angle_ = Math::RadToDeg(dm_.rotation);
//
//	// Origin (Already taken care of with the transform matrices)
//	// Set explicitly to '0' as the default is the middle of the sprite.
//	SDL_FPoint origin_ {0.f, 0.f};
//
//	// Flip
//	SDL_RendererFlip flip_ = SDL_FLIP_NONE;
//	if (flipX_ && flipY_)
//	{
//		flip_ = (SDL_RendererFlip)(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL);
//	}
//	else if (flipX_)
//	{
//		flip_ = (SDL_RendererFlip)SDL_FLIP_HORIZONTAL;
//	}
//	else if (flipY_)
//	{
//		flip_ = (SDL_RendererFlip)SDL_FLIP_VERTICAL;
//	}
//
//	// FIXME AAAAAAAAAAAAAAAAAAAAAa
//	auto source___ = g_registry.get<Components::TextureSource>(frameCheat___.source);
//
//	// Tint (Color Modulation)
//	if (IsTinted(sprite_))
//	{
//		Color tint_ =  GetTint(sprite_);
//
//		SDL_SetTextureColorMod(
//			source___.sdlTexture,
//			tint_.red,
//			tint_.green,
//			tint_.blue
//			);
//	}
//
//	// Alpha (Transparency)
//	if (alpha_ < 1.0)
//	{
//		SDL_SetTextureBlendMode(
//			source___.sdlTexture,
//			SDL_BLENDMODE_BLEND
//			);
//		SDL_SetTextureAlphaMod(
//			source___.sdlTexture,
//			alpha_ * 255
//			);
//	}
//
//	// Blending
//	if (GetBlendMode(sprite_) != BLEND_MODE::NORMAL)
//	{
//		SDL_SetTextureBlendMode(
//			source___.sdlTexture,
//			blendModes[GetBlendMode(sprite_)]
//			);
//	}
//
//	SDL_RenderCopyExF(
//			g_window.renderer,
//			source___.sdlTexture,
//			&source_,
//			&destination_,
//			angle_,
//			&origin_,
//			flip_
//			);
//
//	// Clear blending
//	if (GetBlendMode(sprite_) != BLEND_MODE::NORMAL)
//	{
//		SDL_SetTextureBlendMode(
//			source___.sdlTexture,
//			blendModes[BLEND_MODE::NORMAL]
//			);
//	}
//
//	// Clear alpha from texture
//	if (alpha_ < 1.0)
//	{
//		SDL_SetTextureAlphaMod(
//			source___.sdlTexture,
//			255
//			);
//	}
//
//	// Clear tint from texture
//	if (IsTinted(sprite_))
//	{
//		SDL_SetTextureColorMod(
//			source___.sdlTexture,
//			0xff,
//			0xff,
//			0xff
//			);
//	}
//
//	if (GetMask(sprite_) != entt::null)
//		postRenderMask(GetMask(sprite_), sprite_, camera_);
//}
//
//void Renderer::preRenderMask (Entity maskedObject_)
//{
//	// Is this a camera mask?
//	if (maskedObject_ != entt::null)
//	{
//		SDL_SetRenderTarget(g_window.renderer, cameraBuffer);
//	}
//	else
//	{
//		SDL_SetRenderTarget(g_window.renderer, maskBuffer);
//	}
//
//	// Clear _AFTER_ setting the target, to clear the buffer and not the screen
//	SDL_SetRenderDrawColor(g_window.renderer, 0x00, 0x00, 0x00, 0x00);
//	SDL_RenderClear(g_window.renderer);
//}
//
//void Renderer::postRenderMask (
//		Entity maskObject_,
//		Entity maskedObject_,
//		Entity camera_)
//{
//	// Save the target buffer
//	SDL_Texture *currentTarget_ = SDL_GetRenderTarget(g_window.renderer);
//
//	// Set the mask texture as the new render target
//	SDL_SetRenderTarget(g_window.renderer, maskTexture);
//
//	// Clear the mask texture
//	SDL_SetRenderDrawColor(g_window.renderer, 0x00, 0x00, 0x00, 0x00);
//	SDL_RenderClear(g_window.renderer);
//
//	// Draw the mask GameObject
//	AddToRenderList(camera_, maskObject_);
//	batchSprite(maskObject_, GetFrame(maskObject_), camera_, GetParentTransformMatrix(maskObject_));
//
//	// Reset the target to the buffer
//	SDL_SetRenderTarget(g_window.renderer, currentTarget_);
//
//	// Render the mask on the buffer
//	SDL_RenderCopy(
//			g_window.renderer,
//			maskTexture,
//			nullptr,	// Render the whole texture
//			nullptr		// Render to the entire target
//			);
//
//	// Is this a camera mask?
//	if (maskedObject_ != entt::null)
//	{
//		// Reset the rendering target
//		SDL_SetRenderTarget(g_window.renderer, nullptr);
//
//		SDL_RenderCopy(
//				g_window.renderer,
//				cameraBuffer,
//				nullptr,	// Render the whole texture
//				nullptr		// Render to the entire target
//				);
//	}
//	else
//	{
//		// Is a camera mask active? If so, set it back to be the rendering target
//		if (GetMask(camera_) != entt::null)
//			SDL_SetRenderTarget(g_window.renderer, cameraBuffer);
//		else
//			SDL_SetRenderTarget(g_window.renderer, nullptr);
//
//		SDL_RenderCopy(
//				g_window.renderer,
//				maskBuffer,
//				nullptr,	// Render the whole texture
//				nullptr		// Render to the entire target
//				);
//	}
//}








/*
void Renderer::init (GameConfig* config)
{
	backgroundColor = config->backgroundColor;

	// Create the supported blend modes
	createBlendModes();

	glClearColor(backgroundColor.gl[0], backgroundColor.gl[1],
			backgroundColor.gl[2], backgroundColor.gl[3]);

	// Mipmaps
	mipmapFilter = config.mipmapFilter;
}
*/

Renderer::~Renderer ()
{
	if (snapshotState.surface)
		SDL_FreeSurface(snapshotState.surface);
}

void Renderer::boot ()
{
	width = g_scale.displaySize.width;
	height = g_scale.displaySize.height;

	isBooted = true;

	renderTarget.resize(width, height);
	renderTarget.setAutoResize(true);

	// Setup pipelines
	pipelines.boot();
}

void Renderer::onResize (Size, Size displaySize_, int, int)
{
	// Has the window size changed?
	if (displaySize_.width != width || displaySize_.height != height) {
		resize(displaySize_.width, displaySize_.height);
	}
}

void Renderer::beginCapture (double width_, double height_)
{
	if (width_ < 0)
		width_ = width;
	if (height_ < 0)
		height_ = height;

	renderTarget.bind(true, width_, height_);

	setProjectionMatrix(width_, height_);

	resetTextures();
}

RenderTarget* Renderer::endCapture ()
{
	renderTarget.unbind(true);

	resetProjectionMatrix();

	return &renderTarget;
}

void Renderer::resize (double width_, double height_)
{
	width = width_;
	height = height_;

	setProjectionMatrix(width, height);

	glViewport(0, 0, width, height);

	glScissor(0, 0, width, height);

	defaultScissor[2] = width;
	defaultScissor[3] = height;

	emit("resize", width, height);
}

double Renderer::getAspectRatio ()
{
	return ( (double) width ) / height;
}

void Renderer::setProjectionMatrix (double width_, double height_)
{
	if (width_ != projectionWidth || height_ != projectionHeight) {
		projectionWidth = width_;
		projectionHeight = height_;

		projectionMatrix = glm::ortho(0., width_, height_, 0.);
	}
}

void Renderer::resetProjectionMatrix ()
{
	projectionWidth = width;
	projectionHeight = height;

	projectionMatrix = glm::ortho(0., width, height, 0.);
}

void Renderer::flush ()
{
	pipelines.flush();
}

std::array<int, 4> Renderer::pushScissor (int x_, int y_, int width_, int height_)
{
	std::array<int, 4> scissor {x_, y_, width_, height_};

	scissorStack.push_back(scissor);

	setScissor(x_, y_, width_, height_);

	currentScissor = scissor;

	return scissor;
}

void Renderer::setScissor (int x_, int y_, int width_, int height_)
{
	bool set_ = (width_ > 0 && height_ > 0);
	bool current_ = (currentScissor[2] > 0);

	if (set_ && current_) {
		int cx_ = currentScissor[0];
		int cy_ = currentScissor[1];
		int cw_ = currentScissor[2];
		int ch_ = currentScissor[3];

		set_ = (cx_ != x_ || cy_ != y_ || cw_ != width_ || ch_ != height_);
	}

	if (set_) {
		flush();

		glScissor(x_, height_ - y_, width_, height_);
	}
}

void Renderer::resetScissor ()
{
	glEnable(GL_SCISSOR_TEST);

	if (currentScissor[2] > 0) {
		int cx_ = currentScissor[0];
		int cy_ = currentScissor[1];
		int cw_ = currentScissor[2];
		int ch_ = currentScissor[3];

		if (cw_ > 0 && ch_ > 0)
			glScissor(cx_, ch_ - cy_, cw_, ch_);
	}
}

void Renderer::popScissor ()
{
	// Remove the current scissor
	scissorStack.pop_back();

	if (!scissorStack.empty()) {
		// Reset the previous scissor
		auto scissor = scissorStack.back();

		setScissor(scissor[0], scissor[1], scissor[2], scissor[3]);

		currentScissor = scissor;
	}
	else {
		currentScissor = {-1, -1, -1, -1};
	}
}

bool Renderer::hasActiveStencilMask ()
{
	//TODO

	// Entities
	Entity mask = currentMask.mask;
	Entity camMask = currentCameraMask.mask;

	// No mask set?
	if ((mask == entt::null) && (camMask == entt::null))
		return false;

	// Get components
	auto m = g_registry.try_get<Components::Mask>(mask);
	auto c = g_registry.try_get<Components::Mask>(camMask);
	if (!m && !c)
		return false;

	return (m->isStencil || c->isStencil);
}

void Renderer::resetViewport ()
{
	glViewport(0, 0, width, height);
}

bool Renderer::setBlendMode (int modeId_, bool force_)
{
	BlendMode blendMode = blendModes[modeId_];

	if (force_ || (modeId_ != -1 && currentBlendMode != modeId_)) {
		flush();

		glEnable(GL_BLEND);
		if (blendMode.equation.size() == 1)
			glBlendEquation(blendMode.equation[0]);
		else
			glBlendEquationSeparate(blendMode.equation[0], blendMode.equation[1]);

		if (blendMode.func.size() == 4) {
			glBlendFuncSeparate(blendMode.func[0], blendMode.func[1],
					blendMode.func[2], blendMode.func[3]);
		}
		else {
			glBlendFunc(blendMode.func[0], blendMode.func[1]);
		}

		currentBlendMode = modeId_;

		return true;
	}

	return false;
}

bool Renderer::setBlendMode (BLEND_MODE mode_, bool force_)
{
	return setBlendMode(static_cast<int>(mode_), force_);
}

int Renderer::addBlendMode (std::vector<GLenum> func_,
		std::vector<GLenum> equation_)
{
	int index = blendModes.size();

	blendModes.emplace_back(BlendMode {
		.equation = equation_,
		.func = func_
	});

	return index;
}

void Renderer::updateBlendMode (int index, std::vector<GLenum> func,
		std::vector<GLenum> equation)
{
	size_t i = (index > 0) ? index : 0;

	if (i < blendModes.size()) {
		blendModes[i].func = func;

		if (!equation.empty())
			blendModes[i].equation = equation;
	}
}

void Renderer::removeBlendMode (int index)
{
	size_t i = (index > 0) ? index : 0;

	if (i > 17 && i < blendModes.size())
		blendModes.erase(blendModes.begin() + i);
}

void Renderer::setBlankTexture ()
{
	setTexture2D(blankTexture);
}

int Renderer::setTextureSource (Entity textureSource_)
{
	auto source_ = g_registry.try_get<Components::TextureSource>(textureSource_);
	ZEN_ASSERT(source_, "The entity is not a Texture Source");

	if (pipelines.forceZero()) {
		setTextureZero(source_->glTexture, true);

		return 0;
	}

	if (source_->glIndexCounter < startActiveTexture) {
		source_->glIndexCounter = startActiveTexture;

		if (currentActiveTexture < maxTextures) {
			source_->glIndex = currentActiveTexture;

			glActiveTexture(GL_TEXTURE0 + currentActiveTexture);
			glBindTexture(GL_TEXTURE_2D, source_->glTexture);

			currentActiveTexture++;
		}
		else {
			// We're out of textures, so flush the batch and reset back to 0
			flush();

			startActiveTexture++;
			textureFlush++;

			source_->glIndexCounter = startActiveTexture;
			source_->glIndex = 1;

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, source_->glTexture);

			currentActiveTexture = 2;
		}
	}

	isTextureClean = false;

	return source_->glIndex;
}

bool Renderer::isNewNormalMap (GL_texture texture_, GL_texture normalMap_)
{
	return (textureZero != texture_ || normalTexture != normalMap_);
}

void Renderer::setTextureZero (GL_texture texture_, bool flush_)
{
	if (textureZero != texture_) {
		if (flush_)
			flush();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_);

		textureZero = texture_;
	}
}

void Renderer::clearTextureZero ()
{
	textureZero = 0;
}

void Renderer::setNormalMap (GL_texture texture_)
{
	if (normalTexture != texture_) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_);

		normalTexture = texture_;

		if (currentActiveTexture == 1)
			currentActiveTexture = 2;
	}
}

void Renderer::clearNormalMap ()
{
	normalTexture = 0;
	startActiveTexture++;
	currentActiveTexture = 1;

	textureFlush++;
}

void Renderer::unbindTextures ()
{
	for (size_t i = 0; i < tempTextures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	normalTexture = 0;
	textureZero = 0;

	currentActiveTexture = 1;
	startActiveTexture++;

	textureFlush++;
}

void Renderer::resetTextures (bool all)
{
	if (isTextureClean)
		// No need to do this if the textures are already clean
		return;

	flush();

	if (all) {
		for (size_t i = 0; i < tempTextures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, tempTextures[i]);
		}

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tempTextures[1]);

		isTextureClean = true;
	}
	else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tempTextures[0]);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tempTextures[1]);
	}

	normalTexture = 0;
	textureZero = 0;

	currentActiveTexture = 1;
	startActiveTexture++;

	textureFlush++;
}

int Renderer::setTexture2D (GL_texture texture_)
{
	if (pipelines.forceZero()) {
		setTextureZero(texture_, true);
		return 0;
	}

	auto &info_ = textureInfo[texture_];

	if (info_.glIndexCounter < startActiveTexture) {
		info_.glIndexCounter = startActiveTexture;

		if (currentActiveTexture < maxTextures) {
			info_.glIndex = currentActiveTexture;

			glActiveTexture(GL_TEXTURE0 + currentActiveTexture);
			glBindTexture(GL_TEXTURE_2D, texture_);

			currentActiveTexture++;
		}
		else {
			// We're out of textures, so flush the batch and reset back to 1
			// (0 is reserved for fbos)
			flush();

			startActiveTexture++;

			textureFlush++;

			info_.glIndexCounter = startActiveTexture;
			info_.glIndex = 1;

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture_);

			currentActiveTexture = 2;
		}
	}

	isTextureClean = false;

	return info_.glIndex;
}

void Renderer::pushFramebuffer (GL_fbo framebuffer, bool updateScissor,
		bool resetTextures, bool setViewport)
{
	if (framebuffer == currentFramebuffer)
		return;

	fboStack.emplace_back(framebuffer);

	setFramebuffer(framebuffer, updateScissor, resetTextures, setViewport);
}

void Renderer::setFramebuffer (GL_fbo framebuffer_, bool updateScissor_,
		bool resetTextures_, bool setViewport_)
{
	if (framebuffer_ == currentFramebuffer)
		return;

	double width_ = width;
	double height_ = height;

	if (framebuffer_ && framebufferInfo[framebuffer_].colorBuffer && setViewport_) {
		width_ = textureInfo[framebufferInfo[framebuffer_].colorBuffer].width;
		height_ = textureInfo[framebufferInfo[framebuffer_].colorBuffer].height;
	}
	else {
		flush();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

	if (setViewport_)
		glViewport(0, 0, width_, height_);

	if (updateScissor_) {
		if (framebuffer_)
			pushScissor(0, 0, width_, height_);
		else
			popScissor();
	}

	currentFramebuffer = framebuffer_;

	if (resetTextures_)
		resetTextures();
}

GL_fbo Renderer::popFramebuffer (bool updateScissor, bool resetTextures,
		bool setViewport)
{
	// Remove the current fbo
	fboStack.pop_back();

	// Reset the previous frambuffer
	GL_fbo framebuffer = 0;

	if (!fboStack.empty())
		framebuffer = fboStack.back();

	setFramebuffer(framebuffer, updateScissor, resetTextures, setViewport);

	return framebuffer;
}

bool Renderer::setProgram (GL_program program_)
{
	if (program_ != currentProgram) {
		flush();

		glUseProgram(program_);

		currentProgram = program_;

		return true;
	}

	return false;
}

void Renderer::resetProgram ()
{
	glUseProgram(currentProgram);
}

GL_texture Renderer::createTextureFromSource (Entity source_, double width_,
		double height_, GLenum scaleMode_)
{
	auto src_ = g_registry.try_get<Components::TextureSource>(source_);
	ZEN_ASSERT(src_, "The entity is not a Texture Source");

	GLenum minFilter_ = GL_NEAREST;
	GLenum magFilter_ = GL_NEAREST;
	GLenum wrap_ = GL_CLAMP_TO_EDGE;

	width_ = (source_ != entt::null) ? src_->width : width_;
	height_ = (source_ != entt::null) ? src_->width : height_;

	bool pow_ = Math::IsSizePowerOfTwo(width_, height_);

	if (pow_)
		wrap_ = GL_REPEAT;

	if (scaleMode_ == GL_LINEAR && config.antialias) {
		minFilter_ = (pow_) ? mipmapFilter : GL_LINEAR;
		magFilter_ = GL_LINEAR;
	}

	GL_texture texture_ = 0;
	if (source_ == entt::null && width_ > 0 && height_ > 0) {
		texture_ = createTexture2D(0, minFilter_, magFilter_, wrap_, wrap_,
				GL_RGBA, nullptr, width_, height_);
	}
	else if (source_ != entt::null) {
		// Create the texture
		texture_ = createTexture2D(0, minFilter_, magFilter_, wrap_, wrap_,
				GL_RGBA, src_->tmp);
	}

	return texture_;
}

GL_texture Renderer::createTexture2D (int mipLevel_, GLenum minFilter_,
		GLenum magFilter_, GLenum wrapT_, GLenum wrapS_, GLenum format_,
		SDL_Surface* surface_, int width_, int height_, bool forceSize_)
{
	GL_texture texture_;
	glGenTextures(1, &texture_);

	glActiveTexture(GL_TEXTURE0);

	// Keep current texture to reset it when done
	GL_texture currentTexture_ = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&currentTexture_);

	glBindTexture(GL_TEXTURE_2D, texture_);

	// Set wrapping options on the currently bound texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT_);

	// Set filtering options on the currently bound texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter_);

	// Vertical flip
	// TODO
	// Do not flip, just flip the textures when building the atlas maps using
	// crunch

	if (!surface_) {
		glTexImage2D(GL_TEXTURE_2D, mipLevel_, format_, width_, height_, 0,
				format_, GL_UNSIGNED_BYTE, nullptr);
	}
	else {
		if (!forceSize_) {
			width_ = surface_->w;
			height_ = surface_->h;
		}

		glTexImage2D(GL_TEXTURE_2D, mipLevel_, format_, width_, height_, 0,
				format_, GL_UNSIGNED_BYTE, surface_->pixels);
	}

	if (Math::IsSizePowerOfTwo(width_, height_))
		glGenerateMipmap(GL_TEXTURE_2D);

	if (currentTexture_)
		glBindTexture(GL_TEXTURE_2D, currentTexture_);

	textureInfo[texture_] = {
		.isRenderTexture = false,
		.width = width_,
		.height = height_,
		.glIndex = 0,
		.glIndexCounter = -1
	};

	return texture_;
}

GL_fbo Renderer::createFramebuffer (int width_, int height_,
		GL_texture renderTexture_, bool addDepthStencilBuffer_)
{
	// Framebuffer
	GL_fbo framebuffer_;
	glCreateFramebuffers(1, &framebuffer_);
	setFramebuffer(framebuffer_);

	// Depth and stencil buffer
	GLuint renderBuffer_ = 0;
	if (addDepthStencilBuffer_) {
		glCreateRenderbuffers(1, &renderBuffer_);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer_);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_,
				height_);
		// Attach to fbo
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
				GL_RENDERBUFFER, renderBuffer_);
	}

	// Color buffer
	textureInfo[renderTexture_].isRenderTexture = true;
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
			renderTexture_, 0);

	// Save info about framebuffer
	Emplace(&framebufferInfo, framebuffer_, renderTexture_, renderBuffer_);

	// Check if fbo is complete
	GLenum complete_ = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (complete_ != GL_FRAMEBUFFER_COMPLETE) {
		std::string error_;

		switch (complete_) {
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				error_ = "Incomplete Attachment";
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				error_ = "Missing Attachment";
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				error_ = "Framebuffer Unsupported";
				break;
			default:
				break;
		}

		MessageError("Framebuffer Incomplete: ", error_);
	}

	setFramebuffer();

	resetTextures();

	return framebuffer_;
}

GL_vao Renderer::createVertexArray ()
{
	GL_vao vertexArray;

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	return vertexArray;
}

GL_vbo Renderer::createVertexBuffer (size_t initialSize, GLenum bufferUsage)
{
	GL_vbo vertexBuffer;

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, initialSize, nullptr, bufferUsage);

	return vertexBuffer;
}

GL_vbo Renderer::createVertexBuffer (std::vector<std::uint8_t> initialData,
		GLenum bufferUsage)
{
	GL_vbo vertexBuffer;

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(std::uint8_t) * initialData.size(),
			initialData.data(), bufferUsage);

	return vertexBuffer;
}

GL_ibo Renderer::createIndexBuffer (size_t initialSize, GLenum bufferUsage)
{
	GL_vbo indexBuffer;

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, initialSize, nullptr, bufferUsage);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return indexBuffer;
}

GL_ibo Renderer::createIndexBuffer (std::vector<std::uint8_t> initialData,
		GLenum bufferUsage)
{
	GL_vbo indexBuffer;

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(std::uint8_t) *
			initialData.size(), initialData.data(), bufferUsage);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return indexBuffer;
}

void Renderer::deleteTexture (GL_texture texture_, bool reset_)
{
	if (reset_)
		resetTextures(true);

	if (texture_)
		glDeleteTextures(1, &texture_);

	auto it_ = textureInfo.find(texture_);
	if (it_ != textureInfo.end())
		textureInfo.erase(it_);
}

void Renderer::deleteFramebuffer (GL_fbo framebuffer_)
{
	if (!framebuffer_)
		return;

	// Remove from the stack
	bool removed = false;
	for (size_t i = 0; i < fboStack.size(); i++) {
		GL_fbo fbo_ = fboStack[i];

		if (fbo_ == framebuffer_) {
			fboStack.erase(fboStack.begin() + i);
			removed = true;
			break;
		}
	}

	// Framebuffer not found
	if (!removed)
		return;

	// Delete buffers (Color buffer texture needs to be deleted manually)
	glDeleteFramebuffers(1, &framebuffer_);
	glDeleteRenderbuffers(1, &framebufferInfo[framebuffer_].renderBuffer);

	if (currentFramebuffer == framebuffer_)
		currentFramebuffer = 0;

	// Delete info about framebuffer
	framebufferInfo.erase(framebufferInfo.find(framebuffer_));
}

void Renderer::deleteProgram (GL_program program)
{
	if (program)
		glDeleteProgram(program);
}

void Renderer::deleteBuffer (GLuint buffer)
{
	glDeleteBuffers(1, &buffer);
}

void Renderer::preRenderCamera (Entity camera)
{
	double cx = GetX(camera);
	double cy = GetY(camera);
	double cw = GetWidth(camera);
	double ch = GetHeight(camera);

	Color color = GetBackgroundColor(camera);

	pipelines.preBatchCamera(camera);

	pushScissor(cx, cy, cw, ch);

	auto *masked = g_registry.try_get<Components::Masked>(camera);
	if (masked) {
		currentCameraMask.mask = masked->mask;
		currentCameraMask.camera = masked->camera;

		PreRenderMask(camera, masked->camera);
	}

	if (color.gl[3] > 0) {
		auto &pipeline = pipelines.setMulti();

		pipeline.drawFillRect(
			cx, cy, cw, ch,
			GetTintFromFloats(color.gl[2], color.gl[1], color.gl[0], 1.),
			color.gl[3]
		);
	}
}

Mask_ Renderer::getCurrentStencilMask ()
{
	Mask_ tmp;
	Mask_ *prev = nullptr;

	if (!maskStack.empty())
		prev = &maskStack.back();
	else if (currentCameraMask.mask != entt::null &&
			IsMaskStencil(currentCameraMask.mask))
		prev = &currentCameraMask;
	else
		prev = &tmp;

	return *prev;
}

void Renderer::postRenderCamera (Entity camera) {
	//if (flashEffect.isRunning || (fadeEffect.isRunning || fadeEffect.isComplete)) {
	if (IsFadeRunning(camera) || IsFadeComplete(camera)) {
		auto &pipeline = pipelines.setMulti();

		//flashEffect.postRender(pipeline, GetHexF);
		PostRenderFade(camera, pipeline);
	}

	SetDirty(camera, false);

	popScissor();

	auto mask = g_registry.try_get<Components::Masked>(camera);
	if (mask) {
		currentCameraMask.mask = entt::null;
		PostRenderMask(mask->mask, mask->camera);
	}
	
	pipelines.postBatchCamera(camera);
}

void Renderer::preRender ()
{
	// Make sure we are bound to the main framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (config.clearBeforeRender) {
		Color clearColor = config.backgroundColor;

		glClearColor(clearColor.gl[0], clearColor.gl[1], clearColor.gl[2], clearColor.gl[3]);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	glEnable(GL_SCISSOR_TEST);

	for (int i = 0; i < 4; i++)
		currentScissor[i] = defaultScissor[i];

	scissorStack.clear();
	scissorStack.push_back(currentScissor);

	if (g_scene.customViewports)
		glScissor(0, 0, width, height);

	currentMask.mask = entt::null;
	currentCameraMask.mask = entt::null;
	maskStack.clear();

	textureFlush = 0;

	emit("pre-render");
}

void Renderer::render (std::vector<Entity> children_, Entity camera_)
{
	emit("render", camera_);

	// Apply scissor for cam region + render background color, if not transparent
	preRenderCamera(camera_);

	// If nothing to render, bail out
	if (children_.empty()) {
		setBlendMode(BLEND_MODE::NORMAL);

		// Applies camera effects and pops the scissor, is set
		postRenderCamera(camera_);

		return;
	}

	// Reset the current type
	currentType = "";

	for (size_t i = 0; i < children_.size(); i++) {
		finalType = (i == (children_.size() - 1));

		auto &child_ = children_[i];

		auto m_ = g_registry.try_get<Components::Masked>(child_);
		Entity mask_ = (m_) ? m_->mask : entt::null;

		if (currentMask.mask != entt::null && currentMask.mask != mask_) {
			// Render out the previously set mask
			PostRenderMask(currentMask.mask, currentMask.camera);
		}

		if (mask_ != entt::null && currentMask.mask != mask_)
			PreRenderMask(child_, camera_);

		int bm_ = GetBlendMode(child_);
		if (bm_ != currentBlendMode)
			setBlendMode(bm_);

		if (GetType(child_) != currentType) {
			newType = true;
			currentType = GetType(child_);
		}

		if (!finalType)
			nextTypeMatch = (GetType(children_[i+1]) == currentType);
		else
			nextTypeMatch = false;

		Render(child_, camera_);

		newType = false;
	}

	if (currentMask.mask != entt::null) {
		// Render out the previously set mask, if it was the last item in the
		// display list
		PostRenderMask(currentMask.mask, currentMask.camera);
	}

	setBlendMode(BLEND_MODE::NORMAL);

	// Applies camera effects and pops the scissor, if set
	postRenderCamera(camera_);
}

void Renderer::postRender ()
{
	flush();

	// Update screen
	SDL_GL_SwapWindow(g_window.window);

	emit("post-render");

	if (snapshotState.active) {
		snapshot();
		snapshotState.active = false;
		snapshotState.callback = nullptr;
	}

	if (textureFlush > 0) {
		startActiveTexture++;
		currentActiveTexture = 1;
	}
}

void Renderer::snapshot (std::string path_,
		std::function<void(SDL_Surface*)> callback_)
{
	return snapshotArea(0, 0, g_window.width(), g_window.height(), path_, callback_);
}

void Renderer::snapshotArea (int x_, int y_, int width_, int height_,
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
}

void Renderer::snapshotPixel (int x_, int y_, std::function<void(Color)>& callback_)
{
	snapshotArea(x_, y_, 1, 1, "", nullptr);

	snapshotState.getPixel = true;
	snapshotState.callbackPixel = callback_;
}

void Renderer::snapshotPixel (int x_, int y_, std::function<void(Color)>&& callback_)
{
	return snapshotPixel(x_, y_, callback_);
}

void Renderer::snapshotFramebuffer (GL_fbo framebuffer, int bufferWidth,
		int bufferHeight, std::function<void(SDL_Surface*)>&& callback,
		bool getPixel, int x, int y, int width, int height, std::string path)
{
	snapshotArea(x, y, width, height, path, callback);

	snapshotState.getPixel = getPixel;

	snapshotState.isFramebuffer = true;
	snapshotState.bufferWidth = bufferWidth;
	snapshotState.bufferHeight = bufferHeight;

	setFramebuffer(framebuffer);

	takeSnapshot();

	setFramebuffer(currentFramebuffer);

	snapshotState.callback = nullptr;
	snapshotState.isFramebuffer = false;
	snapshotState.active = false;
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
		std::uint8_t pixel_[4];

		glReadPixels(snapshotState.x, snapshotState.y, 1, 1, GL_RGBA,
				GL_UNSIGNED_BYTE, &pixel_);

		// Save the color components to the output color object
		Color out_;
		SetTo(&out_, pixel_[0], pixel_[1], pixel_[2], pixel_[3]);

		// Call the callback if one was given
		if (snapshotState.callbackPixel)
			snapshotState.callbackPixel(out_);
	}
	else
	{
		// Create a blank surface
		if (!(snapshotState.surface = SDL_CreateRGBSurface(
				0,
				snapshotState.width,
				snapshotState.height,
				pixelFormat_.BitsPerPixel,
				pixelFormat_.Rmask,
				pixelFormat_.Gmask,
				pixelFormat_.Bmask,
				pixelFormat_.Amask
				))) {
			MessageError("Failed to create an RGB surface: ", SDL_GetError());
			return;
		}

		std::vector<std::uint8_t> pixels_;
		pixels_.resize(snapshotState.surface->pitch * snapshotState.surface->h);

		// Read the window's pixels
		glReadPixels(snapshotState.x, snapshotState.y, snapshotState.width,
				snapshotState.height, GL_RGBA, GL_UNSIGNED_BYTE, pixels_.data());

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
			MessageError("Failed to save snapshot in a 'BMP' file: ",
					SDL_GetError());
	}
	else if (extension_ == "png")
	{
		if (IMG_SavePNG(snapshotState.surface, path_.c_str()))
			MessageError("Failed to save snapshot in a 'PNG' file: ",
					IMG_GetError());
	}
	else if (extension_ == "jpg")
	{
		if (IMG_SaveJPG(snapshotState.surface, path_.c_str(), 100))
			MessageError("Failed to save snapshot in a 'JPG' file: ",
					IMG_GetError());
	}
	else if (extension_ == "jpeg")
	{
		if (IMG_SaveJPG(snapshotState.surface, path_.c_str(), 100))
			MessageError("Failed to save snapshot in a 'JPEG' file: ",
					IMG_GetError());
	}
	else
	{
		MessageError("File type unsupported! Try something else like 'png'"
				"or 'jpg'.");
	}
}

}	// namespace Zen
