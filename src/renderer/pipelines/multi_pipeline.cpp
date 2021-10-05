/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "multi_pipeline.hpp"
#include "../../ecs/entity.hpp"
#include "../renderer.hpp"
#include "../utility.hpp"
#include "../../utils/file/file_to_string.hpp"
#include "../../texture/components/frame.hpp"
#include "../../texture/components/source.hpp"
#include "../../texture/systems/frame.hpp"
#include "../../systems/origin.hpp"
#include "../../systems/textured.hpp"
#include "../../systems/flip.hpp"
#include "../../systems/position.hpp"
#include "../../systems/rotation.hpp"
#include "../../systems/scale.hpp"
#include "../../systems/transform_matrix.hpp"
#include "../../systems/alpha.hpp"
#include "../../systems/tint.hpp"
#include "../../systems/scroll.hpp"
#include "../../systems/scroll_factor.hpp"
#include "../../cameras/2d/systems/camera.hpp"
#include "../../math/deg_to_rad.hpp"

#include "../shaders/multi_vert.hpp"
#include "../shaders/multi_frag.hpp"

namespace Zen {

extern entt::registry g_registry;
extern Renderer g_renderer;

MultiPipeline::MultiPipeline (PipelineConfig config)
	: Pipeline(prepareConfig(config))
{}

PipelineConfig MultiPipeline::prepareConfig (PipelineConfig config)
{
	// Load default shaders
	if (config.fragShader.empty())
		config.fragShader = Shaders::MULTI_FRAG;

	config.fragShader = ParseFragmentShaderMaxTextures(config.fragShader,
			g_renderer.maxTextures);

	if (config.vertShader.empty())
		config.vertShader = Shaders::MULTI_VERT;

	if (config.attributes.empty()) {
		config.attributes = {
			{
				.name = "aPosition",
				.size = 2
			},
			{
				.name = "aTexCoord",
				.size = 2
			},
			{
				.name = "aTexId"
			},
			{
				.name = "aTintEffect"
			},
			{
				.name = "aTint",
				.size = 4,
				.type = GL_UNSIGNED_BYTE,
				.normalized = true
			}
		};
	}

	return config;
}

void MultiPipeline::boot ()
{
	Pipeline::boot();
	currentShader->set("uMainSampler", g_renderer.textureIndexes);
}

void MultiPipeline::batchSprite (Entity gameObject, Entity camera,
		Components::TransformMatrix *parentTransformMatrix)
{
	double alpha = GetAlpha(camera) * GetAlpha(gameObject);
	if (!alpha)
		// Nothing to see, so abort early
		return;

	auto &camMatrix = tempMatrix1;
	auto &spriteMatrix = tempMatrix2;
	auto &calcMatrix = tempMatrix3;

	auto *frame = g_registry.try_get<Components::Frame>(GetFrame(gameObject));
	auto *source = g_registry.try_get<Components::TextureSource>(frame->source);
	GL_texture texture = source->glTexture;

	double u0 = frame->u0;
	double v0 = frame->v0;
	double u1 = frame->u1;
	double v1 = frame->v1;
	int frameX = frame->cutX;//frame->x;
	int frameY = frame->cutY;//frame->y;
	// TODO test with cut.height instead of frame.height
	int frameWidth = (frame->rotated) ? frame->cutHeight : frame->cutWidth;
	int frameHeight = (frame->rotated) ? frame->cutWidth : frame->cutHeight;
	bool customPivot = frame->customPivot;

	double displayOriginX = GetDisplayOriginX(gameObject);
	double displayOriginY = GetDisplayOriginY(gameObject);

	// Position
	double x, y;
	//double x = -displayOriginX + frameX;
	//double y = -displayOriginY + frameY;

	// Rotation
	double rot = GetRotation(gameObject);

	// Is the frame rotated in the texture atlas?
	if (!frame->rotated) {
		x = -displayOriginX + frame->data.spriteSourceSize.x;
		y = -displayOriginY + frame->data.spriteSourceSize.y;
	}
	else {
		// Restore frame to upright orientation
		rot += Math::DegToRad(-90);

		// Invert `x` and `y` axis for origin and invert y axis (No minus for x_)
		x = displayOriginY;
		y = -displayOriginX;

		// A trimmed frame has the following metrics:
		// - Width of frame
		// - Height of frame
		// - X position in atlas
		// - Y position in atlas
		// - Real dimensions of image before trimming
		//		- Real width
		// 		- Real height
		// - Trimmed empty space
		//		- Padding Left
		// 		- Padding Up
		// 		- Padding Right
		// 		- Padding Bottom

		// Frame is rotated -90deg on atlas (clockwise)
		// First we restore it upward by rotating it by -90deg
		// By doing this, the following happens:
		// - The originaly trimX or left padding becomes y's padding or top padding
		// - The originaly paddingV or bottom padding becomes x's padding or
		//		left padding

		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// |                                  |                 ^
		// |                                  |               trimX
		// |                                  |                 v
		// |           +--------------+       |              +------+
		// |           |              |       |              |  R   |
		// | < trimX > |   Original   |       |              |  o   |
		// |           |              |       | < paddingB > |  t   |
		// |           +--------------+       |              |  a   |
		// |                  ^               |              |  t   |
		// |               paddingB           |              +------+
		// |                  v               |
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		double bottomPadding =
			frame->data.sourceSize.height -				// Original height
			frame->data.spriteSourceSize.height -		// Non empty height
			frame->data.spriteSourceSize.y;				// Top trimmed padding

		x += bottomPadding - frame->data.sourceSize.height;
		y += frame->data.spriteSourceSize.x;
	}

	if (IsCropped(gameObject)) {
		auto crop = GetCrop(gameObject);
		bool flipX = GetFlipX(gameObject);
		bool flipY = GetFlipY(gameObject);

		if (crop.flipX != flipX || crop.flipY != flipY)
			UpdateFrameCropUVs(GetFrame(gameObject), &crop, flipX, flipY);

		u0 = crop.u0;
		v0 = crop.v0;
		u1 = crop.u1;
		v1 = crop.v1;

		frameWidth = crop.width;
		frameHeight = crop.height;

		frameX = crop.x;
		frameY = crop.y;

		x = -displayOriginX + frameX;
		y = -displayOriginY + frameY;
	}

	double flipX = 1;
	double flipY = 1;

	if (GetFlipX(gameObject)) {
		if (!customPivot)
			x += (-frame->data.sourceSize.width + (displayOriginX*2));

		flipX = -1;
	}

	// No need to invert the Y axis for OpenGL, assets should be inverted already
	if (GetFlipY(gameObject)) {
		if (!customPivot)
			y += (-frame->data.sourceSize.height + (displayOriginY*2));

		flipY = -1;
	}

	ApplyITRS(&spriteMatrix,
		GetX(gameObject), GetY(gameObject),
		GetRotation(gameObject),
		GetScaleX(gameObject) * flipX, GetScaleY(gameObject) * flipY
	);

	camMatrix = GetTransformMatrix(camera);

	if (parentTransformMatrix) {
		// Multiply the camera by the parent matrix
		MultiplyWithOffset(&camMatrix, *parentTransformMatrix,
				-GetScrollX(camera) * GetScrollFactorX(gameObject),
				-GetScrollY(camera) * GetScrollFactorY(gameObject)
				);

		// Undo the camera scroll
		spriteMatrix.e = GetX(gameObject);
		spriteMatrix.f = GetY(gameObject);
	}
	else {
		spriteMatrix.e -= GetScrollX(camera) * GetScrollFactorX(gameObject);
		spriteMatrix.f -= GetScrollY(camera) * GetScrollFactorY(gameObject);
	}

	// Multiply by the sprite matrix, store result in calcMatrix
	calcMatrix = camMatrix;
	Multiply(&calcMatrix, spriteMatrix);

	double xw = x + frameWidth;
	double yh = y + frameHeight;

	bool roundPixels = GetRoundPixels(camera);

	double tx0 = GetXRound(calcMatrix, x, y, roundPixels);
	double ty0 = GetYRound(calcMatrix, x, y, roundPixels);

	double tx1 = GetXRound(calcMatrix, x, yh, roundPixels);
	double ty1 = GetYRound(calcMatrix, x, yh, roundPixels);

	double tx2 = GetXRound(calcMatrix, xw, yh, roundPixels);
	double ty2 = GetYRound(calcMatrix, xw, yh, roundPixels);

	double tx3 = GetXRound(calcMatrix, xw, y, roundPixels);
	double ty3 = GetYRound(calcMatrix, xw, y, roundPixels);

	double cameraAlpha = GetAlpha(camera);
	int tintTL, tintTR, tintBL, tintBR;
	GetTint(gameObject, &tintTL, &tintTR, &tintBL, &tintBR);
	double atl, atr, abl, abr;
	GetAlpha(gameObject, &atl, &atr, &abl, &abr);

	tintTL = GetTintAppendFloatAlpha(tintTL, cameraAlpha * atl);
	tintTR = GetTintAppendFloatAlpha(tintTR, cameraAlpha * atr);
	tintBL = GetTintAppendFloatAlpha(tintBL, cameraAlpha * abl);
	tintBR = GetTintAppendFloatAlpha(tintBR, cameraAlpha * abr);

	bool tintFill = IsTintFilled(gameObject);

	if (shouldFlush(6))
		flush();

	int unit = setGameObject(gameObject);

	g_renderer.pipelines.preBatch(gameObject);

	batchQuad(gameObject, tx0, ty0, tx1, ty1, tx2, ty2, tx3, ty3, u0, v0, u1, v1,
			tintTL, tintTR, tintBL, tintBR, tintFill, texture, unit);

	g_renderer.pipelines.postBatch(gameObject);
}

void MultiPipeline::batchTexture (
	Entity gameObject,
	GL_texture texture,
	double textureWidth, double textureHeight,
	double srcX, double srcY,
	double srcWidth, double srcHeight,
	double scaleX, double scaleY,
	double rotation,
	bool flipX, bool flipY,
	double scrollFactorX, double scrollFactorY,
	double displayOriginX, double displayOriginY,
	double frameX, double frameY, double frameWidth, double frameHeight,
	int tintTL, int tintTR, int tintBL, int tintBR, int tintEffect,
	double uOffset, double vOffset,
	Entity camera,
	Components::TransformMatrix *parentTransformMatrix,
	bool skipFlip,
	int textureUnit
	)
{
	auto &camMatrix = tempMatrix1;
	auto &spriteMatrix = tempMatrix2;
	auto &calcMatrix = tempMatrix3;

	double u0 = (frameX / textureWidth) + uOffset;
	double v0 = (frameY / textureHeight) + vOffset;
	double u1 = (frameX + frameWidth) / textureWidth + uOffset;
	double v1 = (frameY + frameHeight) / textureHeight + vOffset;

	double width = srcWidth;
	double height = srcHeight;

	double x = -displayOriginX;
	double y = -displayOriginY;

	if (IsCropped(gameObject)) {
		auto crop = GetCrop(gameObject);

		width = crop.width;
		height = crop.height;

		srcWidth = crop.width;
		srcHeight = crop.height;

		frameX = crop.x;
		frameY = crop.y;

		double ox = frameX;
		double oy = frameY;

		if (flipX)
			ox = (frameWidth - crop.x - crop.width);

		if (flipY)
			oy = (frameHeight - crop.y - crop.height);

		u0 = (ox / textureWidth) + uOffset;
		v0 = (oy / textureHeight) + vOffset;
		u1 = (ox + crop.width) / textureWidth + uOffset;
		v1 = (oy + crop.height) / textureHeight + vOffset;

		x = -displayOriginX + frameX;
		y = -displayOriginY + frameY;
	}

	// Invert the flipY if this is a RenderTexture
	flipY = flipY ^ (!skipFlip && g_renderer.textureInfo[texture].isRenderTexture
			? 1 : 0);

	if (flipX) {
		width *= -1;
		x += srcWidth;
	}

	if (flipY) {
		height *= -1;
		y += srcHeight;
	}

	double xw = x + width;
	double yh = y + height;

	ApplyITRS(&spriteMatrix,
		srcX, srcY,
		rotation,
		scaleX, scaleY
	);

	camMatrix = GetTransformMatrix(camera);

	if (parentTransformMatrix) {
		// Multiply the camera by the parent matrix
		MultiplyWithOffset(&camMatrix, *parentTransformMatrix,
			-GetScrollX(camera) * scrollFactorX,
			-GetScrollY(camera) * scrollFactorY
		);
		// Undo the camera scroll
		spriteMatrix.e = srcX;
		spriteMatrix.f = srcY;
	}
	else {
		spriteMatrix.e -= GetScrollX(camera) * scrollFactorX;
		spriteMatrix.f -= GetScrollY(camera) * scrollFactorY;
	}

	// Multiply by the sprite matrix, store result in calcMatrix
	calcMatrix = camMatrix;
	Multiply(&calcMatrix, spriteMatrix);

	bool roundPixels = GetRoundPixels(camera);

	double tx0 = GetXRound(calcMatrix, x, y, roundPixels);
	double ty0 = GetYRound(calcMatrix, x, y, roundPixels);

	double tx1 = GetXRound(calcMatrix, x, yh, roundPixels);
	double ty1 = GetYRound(calcMatrix, x, yh, roundPixels);

	double tx2 = GetXRound(calcMatrix, xw, yh, roundPixels);
	double ty2 = GetYRound(calcMatrix, xw, yh, roundPixels);

	double tx3 = GetXRound(calcMatrix, xw, y, roundPixels);
	double ty3 = GetYRound(calcMatrix, xw, y, roundPixels);

	if (textureUnit < 0)
		textureUnit = g_renderer.setTexture2D(texture);

	if (gameObject != entt::null)
		g_renderer.pipelines.preBatch(gameObject);

	batchQuad(gameObject, tx0, ty0, tx1, ty1, tx2, ty2, tx3, ty3, u0, v0, u1, v1,
			tintTL, tintTR, tintBL, tintBR, tintEffect, texture, textureUnit);

	if (gameObject != entt::null)
		g_renderer.pipelines.postBatch(gameObject);
}

void MultiPipeline::batchTextureFrame (
	Entity frame,
	double x, double y,
	int tint, double alpha,
	Components::TransformMatrix transformMatrix,
	Components::TransformMatrix *parentTransformMatrix
	)
{
	auto *fr = g_registry.try_get<Components::Frame>(frame);

	g_renderer.pipelines.set(name);

	auto &spriteMatrix = tempMatrix1;
	auto &calcMatrix = tempMatrix2;

	spriteMatrix = transformMatrix;

	double xw = x + fr->width;
	double yh = y + fr->height;

	if (parentTransformMatrix) {
		calcMatrix = spriteMatrix;
		Multiply(&calcMatrix, *parentTransformMatrix);
	}
	else {
		calcMatrix = spriteMatrix;
	}

	double tx0 = GetX(calcMatrix, x, y);
	double ty0 = GetY(calcMatrix, x, y);

	double tx1 = GetX(calcMatrix, x, yh);
	double ty1 = GetY(calcMatrix, x, yh);

	double tx2 = GetX(calcMatrix, xw, yh);
	double ty2 = GetY(calcMatrix, xw, yh);

	double tx3 = GetX(calcMatrix, xw, y);
	double ty3 = GetY(calcMatrix, xw, y);

	int unit = g_renderer.setTextureSource(fr->source);

	tint = GetTintAppendFloatAlpha(tint, alpha);

	auto *src = g_registry.try_get<Components::TextureSource>(fr->source);

	batchQuad(entt::null, tx0, ty0, tx1, ty1, tx2, ty2, tx3, ty3, fr->u0, fr->v0,
			fr->u1, fr->v1, tint, tint, tint, tint, 0, src->glTexture, unit);
}

}	// namespace Zen
