/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "camera.hpp"

#include <map>
#include "../../../utils/assert.hpp"
#include "../../../event/event_emitter.hpp"
#include "../../../math/linear.hpp"
#include "../../../math/deg_to_rad.hpp"
#include "../../../geom/rectangle.hpp"

#include "../../../components/position.hpp"
#include "../../../components/size.hpp"
#include "../../../components/scroll.hpp"
#include "../../../components/zoom.hpp"
#include "../../../components/rotation.hpp"
#include "../../../components/transform_matrix.hpp"
#include "../../../components/viewport.hpp"
#include "../../../components/input.hpp"
#include "../../../components/follow.hpp"
#include "../../../components/deadzone.hpp"
#include "../../../components/renderable.hpp"
#include "../../../components/actor.hpp"
#include "../../../components/id.hpp"
#include "../../../components/visible.hpp"
#include "../../../components/alpha.hpp"
#include "../../../components/flip.hpp"
#include "../../../components/tint.hpp"
#include "../../../components/world_view.hpp"
#include "../../../components/dirty.hpp"
#include "../../../components/transparent.hpp"
#include "../../../components/cull.hpp"
#include "../../../components/origin.hpp"
#include "../../../components/mask.hpp"
#include "../../../components/background_color.hpp"
#include "../../../components/update.hpp"
#include "../../../components/scroll_factor.hpp"
#include "../../../components/container_item.hpp"
#include "../../../components/bounds.hpp"
#include "../../../components/mid_point.hpp"
#include "../../../systems/scroll.hpp"
#include "../../../systems/bounds.hpp"
#include "../../../systems/transform_matrix.hpp"

namespace Zen {

extern entt::registry g_registry;
extern EventEmitter g_event;

// Temporary vector, to not recreate a new one each culling pass
static std::vector<Entity> culledObjects;

// Map: Camera - Object List
static std::map<Entity, std::vector<Entity>> renderLists;

Entity CreateCamera (int x, int y, int width, int height)
{
	Entity camera = g_registry.create();

	g_registry.emplace<Components::Position>(camera, x, y);
	g_registry.emplace<Components::Size>(camera, width, height);
	g_registry.emplace<Components::Scroll>(camera);
	g_registry.emplace<Components::Zoom>(camera);
	g_registry.emplace<Components::Rotation>(camera);
	g_registry.emplace<Components::TransformMatrix>(camera);
	g_registry.emplace<Components::Viewport>(camera);
	g_registry.emplace<Components::Input>(camera);
	g_registry.emplace<Components::Follow>(camera);
	g_registry.emplace<Components::Deadzone>(camera);
	g_registry.emplace<Components::Renderable>(camera);
	g_registry.emplace<Components::Actor>(camera);
	g_registry.emplace<Components::Id>(camera);
	g_registry.emplace<Components::Visible>(camera);
	g_registry.emplace<Components::Alpha>(camera);
	g_registry.emplace<Components::Flip>(camera);
	g_registry.emplace<Components::Tint>(camera);
	g_registry.emplace<Components::WorldView>(camera);
	g_registry.emplace<Components::Dirty>(camera);
	g_registry.emplace<Components::Transparent>(camera, true);
	g_registry.emplace<Components::Cull>(camera, true);
	g_registry.emplace<Components::Origin>(camera);
	g_registry.emplace<Components::Mask>(camera);
	g_registry.emplace<Components::BackgroundColor>(camera);
	g_registry.emplace<Components::MidPoint>(camera, width / 2., height / 2.);
	g_registry.emplace<Components::Update<Components::Position>>(camera, &UpdateCameraSystem);
	g_registry.emplace<Components::Update<Components::Size>>(camera, &UpdateCameraSystem);
	g_registry.emplace<Components::Update<Components::Actor>>(camera, &UpdateCameraSystem);

	/*
	 * Components:
	 * - Position
	 * - Size
	 * - Scroll
	 * - Zoom
	 * - Rotation
	 * - Bounds (Only added if set by user)
	 * - TransformMatrix
	 * - Viewport
	 * - Input
	 * - Follow
	 * - Deadzone
	 * - Renderable
	 * - Actor
	 * - Id
	 * - Name (Only added if set by user)
	 * - Visible
	 * - Alpha
	 * - Flip
	 * - Tint
	 * - WorldView
	 * - Dirty
	 * - Transparent
	 * - Cull
	 * - Origin
	 * - Mask
	 * - Color (BG)
	 * - Update<Position>
	 * - Update<Size>
	 * - Update<Rotation> (Set dirty)
	 */

	/*
	 * ***
	 *
	 * culledObjects (Temporary): In camera manager:
	 * std::map<Entity, std::vector<Entity>> culledObjects;
	 *
	 * ***
	 *
	 * The Camera that this Camera uses for translation during masking.
	 *
	 * If the mask is fixed in position this will be a reference to
	 * the `CameraManager.def` instance. Otherwise, it'll be a reference
	 * to itself.
	 *
	 * @since 0.0.0
	 * Camera* maskCamera = nullptr;
	 *
	 * ***
	 *
	 * Effects
	 *
	 * ***
	 * renderList: In camera manager:
	 * std::map<Entity, std::vector<Entity>> renderLists;
	 * Map of Camera - GameObjects
	 * ---
	 * This array is populated with all of the Game Objects that this Camera 
	 * has rendered in the previous (or current, depending on when you
	 * inspect it) frame.
	 *
	 * It is cleared at the start of `Camera.preUpdate`, or if the Camera is 
	 * destroyed.
	 *
	 * You should not modify this array as it is used internally by the input 
	 * system, however you can read it as required. Note that Game Objects may 
	 * appear in this list multiple times if they belong to multiple non-
	 * exclusive Containers.
	 *
	 * @since 0.0.0
	 * std::vector<GameObjects::GameObject*> renderList;
	 *
	 * ***
	 *
	 * The mid-point of the Camera in 'world' coordinates.
	 *
	 * Use it to obtain exactly where in the world the center of the camera is 
	 * currently looking.
	 *
	 * This value is updated in the preRender method, after the scroll values 
	 * and follower
	 * have been processed.
	 *
	 * @since 0.0.0
	 * Math::Vector2 midPoint;
	 */
}

void DestroyCamera (Entity entity)
{
	//ResetFX(entity);

	g_event.emit(entity, "destroy");

	g_event.removeAllListeners(entity);

	culledObjects.clear();

	auto it = renderLists.find(entity);
	if (it != renderLists.end())
		renderLists.erase(it);

	g_registry.destroy(entity);
}

void AddToRenderList (Entity entity, Entity child)
{
	renderLists[entity].emplace_back(child);
}

std::vector<Entity> Cull (
		Entity entity,
		std::vector<Entity> renderableEntities)
{
	auto [cull, matrix, position, size, scroll] = g_registry.try_get<
		Components::Cull,
		Components::TransformMatrix,
		Components::Position,
		Components::Size,
		Components::Scroll
		>(entity);

	ZEN_ASSERT(cull && matrix && position && size && scroll, "The entity has no 'Cull', 'TransformMatrix', 'Position', 'Size' or 'Scroll' component.");

	if (!cull || !cull->value)
		return renderableEntities;

	double mva = matrix->a;
	double mvb = matrix->b;
	double mvc = matrix->c;
	double mvd = matrix->d;

	// First Invert Matrix
	double determinant = (mva * mvd) - (mvb * mvc);

	if (!determinant)
		return renderableEntities;

	double mve = matrix->e;
	double mvf = matrix->f;

	double cullTop = position->y;
	double cullBottom = position->y + size->height;
	double cullLeft = position->x;
	double cullRight = position->x + size->width;

	determinant = 1.f / determinant;

	// Clear the temporary matrix
	culledObjects.clear();

	for (auto& object : renderableEntities)
	{
		auto [oSize, oPosition, oScrollFactor, oOrigin, oItem] = g_registry.try_get<
			Components::Size,
			Components::Position,
			Components::ScrollFactor,
			Components::Origin,
			Components::ContainerItem
			>(object);

		if (oSize && oPosition && oScrollFactor && oOrigin && oItem)
		{
			auto objectW = oSize->width;
			auto objectH = oSize->height;
			auto objectX = (oPosition->x - (scroll->x * oScrollFactor->x)) - (objectW * oOrigin->x);
			auto objectY = (oPosition->y - (scroll->y * oScrollFactor->y)) - (objectH * oOrigin->y);
			auto tx = (objectX * mva + objectY * mvc + mve);
			auto ty = (objectX * mvb + objectY * mvd + mvf);
			auto tw = ((objectX + objectW) * mva + (objectY + objectH) * mvc + mve);
			auto th = ((objectX + objectW) * mvb + (objectY + objectH) * mvd + mvf);

			if ((tw > cullLeft && tx < cullRight) && (th > cullTop && ty < cullBottom))
			{
				culledObjects.emplace_back(object);
			}
		}
		else
		{
			culledObjects.emplace_back(object);
		}
	}

	return culledObjects;
}

Math::Vector2 GetWorldPoint (Entity entity, int x, int y)
{
	auto [matrix, rotation, scroll, zoom] = g_registry.try_get<
		Components::TransformMatrix,
		Components::Rotation,
		Components::Scroll,
		Components::Zoom
		>(entity);

	ZEN_ASSERT(matrix && rotation && scroll && zoom, "The entity has no 'TransformMatrix', 'Rotation', 'Scroll' or 'Zoom' component.");

	Math::Vector2 output;

	double mva = matrix->a;
	double mvb = matrix->b;
	double mvc = matrix->c;
	double mvd = matrix->d;
	double mve = matrix->e;
	double mvf = matrix->f;

	// Invert Matrix
	double determinant = (mva * mvd) - (mvb * mvc);

	if (!determinant) {
		output.x = x;
		output.y = y;

		return output;
	}

	determinant = 1. / determinant;

	double ima = mvd * determinant;
	double imb = -mvb * determinant;
	double imc = -mvc * determinant;
	double imd = mva * determinant;
	double ime = (mvc * mvf - mvd * mve) * determinant;
	double imf = (mvb * mve - mva * mvf) * determinant;

	double c = std::cos(rotation->value);
	double s = std::sin(rotation->value);

	double sx = x + ((scroll->x * c - scroll->y * s) * zoom->x);
	double sy = y + ((scroll->x * s - scroll->y * c) * zoom->y);

	// Apply transform to point
	output.x = (sx * ima + sy * imc) + ime;
	output.y = (sx * imb + sy * imd) + imf;

	return output;
}

void Ignore (Entity camera, Entity entry)
{
	auto cameraId = g_registry.try_get<Components::Id>(camera);
	auto entityRenderable = g_registry.try_get<Components::Renderable>(entry);

	if (cameraId && entityRenderable)
		entityRenderable->filter |= cameraId->value;

	// TODO Group ignore
	//for (auto& child_ : entry_)
	//{
	//	if (child_->isParent)
	//		// A group in the group
	//		ignore(child_);
	//	else
	//		child_->cameraFilter |= id;
	//}
}

void Ignore (Entity camera, std::vector<Entity>& entries)
{
	for (auto it : entries)
		Ignore(camera, it);
}

void Ignore (Entity camera, std::vector<Entity>&& entries)
{
	Ignore(camera, entries);
}

void PreRender (Entity entity)
{
	auto [size, origin, scroll, deadzone, follow, bounds, position, rotation, zoom, matrix, midPoint, worldView] = g_registry.try_get<
		Components::Size,
		Components::Origin,
		Components::Scroll,
		Components::Deadzone,
		Components::Follow,
		Components::Bounds,
		Components::Position,
		Components::Rotation,
		Components::Zoom,
		Components::TransformMatrix,
		Components::MidPoint,
		Components::WorldView
		>(entity);

	// TODO ZEN_ASSERT(matrix && rotation && scroll && zoom, "The entity has no 'TransformMatrix', 'Rotation', 'Scroll' or 'Zoom' component.");

	renderLists[entity].clear();

	int halfWidth = size->width * 0.5;
	int halfHeight = size->height * 0.5;

	int originX = size->width * origin->x;
	int originY = size->height * origin->y;

	int sx = scroll->x;
	int sy = scroll->y;

	if (deadzone)
	{
		CenterOn(&deadzone->zone, midPoint->x, midPoint->y);
	}

	////bool emitFollowEvent = false;

	if (follow) // TODO && !panEffect.isRunning) {
	{
		auto& tPosition = g_registry.get<Components::Position>(follow->target);
		int fx = tPosition.x - follow->offset.x;
		int fy = tPosition.y - follow->offset.y;

		if (deadzone)
		{
			auto& z = deadzone->zone;

			if (fx < z.x)
			{
				sx = Math::Linear(sx, sx - (z.x - fx), follow->lerp.x);
			}
			else if (fx > (z.x + z.width))
			{
				sx = Math::Linear(sx, sx + (fx - (z.x + z.width)), follow->lerp.x);
			}

			if (fy < z.y)
			{
				sy = Math::Linear(sy, sy - (z.y - fy), follow->lerp.y);
			}
			else if (fy > (z.y + z.height))
			{
				sy = Math::Linear(sy, sy + (fy - (z.y + z.height)), follow->lerp.y);
			}
		}
		else
		{
			sx = Math::Linear(sx, fx - origin->x, follow->lerp.x);
			sy = Math::Linear(sy, fy - origin->y, follow->lerp.y);
		}

		////emitFollowEvent = true;
	}

	if (bounds)
	{
		sx = ClampX(entity, sx);
		sy = ClampY(entity, sy);
	}

	// Values are in pixels and not impacted by zooming the Camera
	SetScrollX(entity, sx);
	SetScrollY(entity, sy);

	int midX = sx + halfWidth;
	int midY = sy + halfHeight;

	// The center of the camera, in world space, so taking zoom into account
	// Basically the pixel value of what it's looking at in the middle of the cam
	midPoint->x = midX;
	midPoint->y = midY;

	int displayWidth = size->width / zoom->x;
	int displayHeight = size->height / zoom->y;

	SetTo(&worldView->worldView,
		midX - (displayWidth / 2.),
		midY - (displayHeight / 2.),
		displayWidth,
		displayHeight
		);

	ApplyITRS(matrix, position->x + originX, position->y + originY, rotation->value, zoom->x, zoom->y);
	Translate(matrix, -originX, -originY);

	// TODO
	//shakeEffect.preRender();

	////if (emitFollowEvent)
	////	emit("follow-update");
}

////void FadeIn (
////		Entity entity,
////		int duration_, int red_, int green_, int blue_)
////{
////	return fadeEffect.start(false, duration_, red_, green_, blue_, true);
////}
////
////void FadeOut (Entity entity, int duration_, int red_, int green_, int blue_)
////{
////	return fadeEffect.start(true, duration_, red_, green_, blue_, true);
////}
////
////void FadeFrom (Entity entity, int duration_, int red_, int green_, int blue_, bool force_)
////{
////	return fadeEffect.start(false, duration_, red_, green_, blue_, force_);
////}
////
////void Fade (Entity entity, int duration_, int red_, int green_, int blue_, bool force_)
////{
////	return fadeEffect.start(true, duration_, red_, green_, blue_, force_);
////}
////
////void Flash (Entity entity, int duration_, int red_, int green_, int blue_, bool force_)
////{
////	return flashEffect.start(duration_, red_, green_, blue_, force_);
////}
////
////void Shake (Entity entity, int duration_, Math::Vector2 intensity_, bool force_)
////{
////	return shakeEffect.start(duration_, intensity_, force_);
////}
////
////void Pan (Entity entity, int x_, int y_, int duration_, std::string ease_, bool force_)
////{
////	return panEffect.start(x_, y_, duration_, ease_, force_);
////}
////
////void Camera::rotateTo (
////		double radians_,
////		bool shortestPath_,
////		int duration_,
////		std::string ease_,
////		bool force_)
////{
////	return rotateToEffect.start(radians_, shortestPath_, duration_, ease_, force_);
////}
////
////void ZoomTo (Entity entity, double zoom_, int duration_, std::string ease_, bool force_)
////{
////	return zoomEffect.start(zoom_, duration_, ease_, force_);
////}
////
////void ResetFX (Entity entity)
////{
////	rotateToEffect.reset();
////	panEffect.reset();
////	shakeEffect.reset();
////	flashEffect.reset();
////	fadeEffect.reset();
////
////	return *this;
////}

void UpdateCameraSystem (Entity entity)
{
	/*
	if (scaleManager == nullptr)
		return;

	bool custom_ = (x != 0 || y != 0 || scaleManager->getWidth() != width || scaleManager->getHeight() != height);

	if (custom_ && !customViewport)
		// We need a custom viewport for this camera
		sceneManager->customViewports++;
	else if (!custom_ && customViewport)
		// We're turning off a custom viewport for this Camera
		sceneManager->customViewports--;

	dirty = true;
	customViewport = custom_;
	*/
}

void UpdateCamera (Entity entity, Uint32 time, Uint32 delta)
{
	////if (visible) {
	////	rotateToEffect.update(time_, delta_);
	////	panEffect.update(time_, delta_);
	////	zoomEffect.update(time_, delta_);
	////	shakeEffect.update(time_, delta_);
	////	flashEffect.update(time_, delta_);
	////	fadeEffect.update(time_, delta_);
	////}
}

}	// namespace Zen
