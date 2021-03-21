/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "camera.h"
#include "camera_manager.h"

#include "../../math/linear.h"
#include "../../math/deg_to_rad.h"

#include "../../gameobjects/gameobject.h"
#include "../../gameobjects/group/group.h"
#include "../../scale/scale_manager.h"

#include "../../gameobjects/components/size.h"

namespace Zen {
namespace Cameras {
namespace Scene2D {

Camera::Camera (int x_, int y_, int width_, int height_)
	: lerp (1.0, 1.0)
	, zoomEffect (*this)
	, rotateToEffect (*this)
	, panEffect (*this)
	, shakeEffect (*this)
	, flashEffect (*this)
	, fadeEffect (*this)
	, x (x_)
	, y (y_)
	, width (width_)
	, height (height_)
	, midPoint (width_ / 2, height_ / 2)
{}

Camera::Camera (const Camera& other_)
	: Camera (other_.x, other_.y, other_.width, other_.height)
{}

Camera::~Camera ()
{
	resetFX();

	emit("destroy");

	removeAllListeners();

	culledObjects.clear();

	if (customViewport)
		// We're turning off a custom viewport for this Camera
		sceneManager->customViewports--;

	renderList.clear();
}

int Camera::getComponentMask ()
{
	return COMPONENT_MASK;
}

void Camera::addToRenderList (GameObjects::GameObject& child_)
{
	renderList.emplace_back(&child_);
}

Camera& Camera::setOrigin (double x_, double y_)
{
	originX = x_;
	originY = y_;

	return *this;
}

Camera& Camera::setOrigin (double x_)
{
	return setOrigin(x_, x_);
}

Math::Vector2 Camera::getScroll (int x_, int y_)
{
	Math::Vector2 out_;

	auto originX_ = width * 0.5;
	auto originY_ = height * 0.5;

	out_.x = x_ - originX_;
	out_.y = y_ - originY_;

	if (useBounds) {
		out_.x = clampX(out_.x);
		out_.y = clampX(out_.y);
	}

	return out_;
}

Camera& Camera::centerOnX (int x_)
{
	double originX_ = width * 0.5;

	midPoint.x = x_;

	setScrollX(x_ - originX_);

	if (useBounds)
		setScrollX( clampX( getScrollX() ) );

	return *this;
}

Camera& Camera::centerOnY (int y_)
{
	double originY_ = height * 0.5;

	midPoint.y = y_;

	setScrollY(y_ - originY_);

	if (useBounds)
		setScrollY( clampY( getScrollY() ) );

	return *this;
}

Camera& Camera::centerOn (int x_, int y_)
{
	centerOnX(x_);
	centerOnY(y_);

	return *this;
}

Camera& Camera::centerToBounds ()
{
	if (useBounds) {
		auto originX_ = width * 0.5;
		auto originY_ = height * 0.5;

		midPoint.set(bounds.centerX(), bounds.centerY());

		setScrollX(bounds.centerX() - originX_);
		setScrollY(bounds.centerY() - originY_);
	}

	return *this;
}

Camera& Camera::centerToSize ()
{
	setScrollX(width * 0.5);
	setScrollY(height * 0.5);

	return *this;
}

std::vector<GameObjects::GameObject*> Camera::cull (
		std::vector<GameObjects::GameObject*> renderableObjects_)
{
	if (disableCull)
		return renderableObjects_;

	auto cameraMatrix_ = matrix.getVector();

	auto mva_ = cameraMatrix_[0];
	auto mvb_ = cameraMatrix_[1];
	auto mvc_ = cameraMatrix_[2];
	auto mvd_ = cameraMatrix_[3];

	// First Invert Matrix
	auto determinant_ = (mva_ * mvd_) - (mvb_ * mvc_);

	if (!determinant_)
		return renderableObjects_;

	auto mve_ = cameraMatrix_[4];
	auto mvf_ = cameraMatrix_[5];

	auto cullTop_ = y;
	auto cullBottom_ = y + height;
	auto cullLeft_ = x;
	auto cullRight_ = x + width;

	determinant_ = 1.f / determinant_;

	culledObjects.clear();

	for (auto& object_ : renderableObjects_)
	{
		//if constexpr (object_->hasComponent(COMPONENT_MASK_SIZE))
		if (true)
		{
			if (object_->parentContainer != nullptr)
			{
				culledObjects.emplace_back(object_);
				continue;
			}

			auto objectW_ = object_->width;
			auto objectH_ = object_->height;
			auto objectX_ = (object_->getX() - (scrollX * object_->getScrollFactorX())) - (objectW_ * object_->getOriginX());
			auto objectY_ = (object_->getY() - (scrollY * object_->getScrollFactorY())) - (objectH_ * object_->getOriginY());
			auto tx_ = (objectX_ * mva_ + objectY_ * mvc_ + mve_);
			auto ty_ = (objectX_ * mvb_ + objectY_ * mvd_ + mvf_);
			auto tw_ = ((objectX_ + objectW_) * mva_ + (objectY_ + objectH_) * mvc_ + mve_);
			auto th_ = ((objectX_ + objectW_) * mvb_ + (objectY_ + objectH_) * mvd_ + mvf_);

			if ((tw_ > cullLeft_ && tx_ < cullRight_) && (th_ > cullTop_ && ty_ < cullBottom_))
			{
				culledObjects.emplace_back(object_);
			}
		}
		else
		{
			culledObjects.emplace_back(object_);
		}
	}

	return culledObjects;
}

Math::Vector2 Camera::getWorldPoint (int x_, int y_)
{
	Math::Vector2 output_;

	auto cameraMatrix_ = matrix.getVector();

	auto mva_ = cameraMatrix_[0];
	auto mvb_ = cameraMatrix_[1];
	auto mvc_ = cameraMatrix_[2];
	auto mvd_ = cameraMatrix_[3];
	auto mve_ = cameraMatrix_[4];
	auto mvf_ = cameraMatrix_[5];

	// Invert Matrix
	double determinant_ = (mva_ * mvd_) - (mvb_ * mvc_);

	if (!determinant_) {
		output_.x = x_;
		output_.y = y_;

		return output_;
	}

	determinant_ = 1 / determinant_;

	auto ima_ = mvd_ * determinant_;
	auto imb_ = -mvb_ * determinant_;
	auto imc_ = -mvc_ * determinant_;
	auto imd_ = mva_ * determinant_;
	auto ime_ = (mvc_ * mvf_ - mvd_ * mve_) * determinant_;
	auto imf_ = (mvb_ * mve_ - mva_ * mvf_) * determinant_;

	double c_ = std::cos(rotation);
	double s_ = std::sin(rotation);

	double sx_ = x_ + ((scrollX * c_ - scrollY * s_) * zoomX);
	double sy_ = y_ + ((scrollX * s_ - scrollY * c_) * zoomY);

	// Apply transform to point
	output_.x = (sx_ * ima_ + sy_ * imc_) + ime_;
	output_.y = (sx_ * imb_ + sy_ * imd_) + imf_;

	return output_;
}

Camera& Camera::ignore (GameObjects::GameObject* entry_)
{
	entry_->cameraFilter |= id;

	return *this;
}

Camera& Camera::ignore (std::vector<GameObjects::GameObject*>& entries_)
{
	for (auto it_ : entries_)
		ignore(it_);

	return *this;
}

Camera& Camera::ignore (std::vector<GameObjects::GameObject*>&& entries_)
{
	return ignore(entries_);
}

Camera& Camera::ignore (GameObjects::Group& entry_)
{
	for (auto& child_ : entry_)
	{
		if (child_->isParent)
			// A group in the group
			ignore(child_);
		else
			child_->cameraFilter |= id;
	}

	return *this;
}

void Camera::preRender ()
{
	renderList.clear();

	int halfWidth_ = width * 0.5;
	int halfHeight_ = height * 0.5;

	int originX_ = width * originX;
	int originY_ = height * originY;

	int sx_ = scrollX;
	int sy_ = scrollY;

	if (deadzone)
	{
		deadzone->centerOn(midPoint.x, midPoint.y);
	}

	bool emitFollowEvent_ = false;

	if (follow && !panEffect.isRunning) {
		int fx_ = follow->getX() - followOffset.x;
		int fy_ = follow->getY() - followOffset.y;

		if (deadzone)
		{
			if (fx_ < deadzone->x)
			{
				sx_ = Math::linear(sx_, sx_ - (deadzone->x - fx_), lerp.x);
			}
			else if (fx_ > deadzone->getRight())
			{
				sx_ = Math::linear(sx_, sx_ + (fx_ - deadzone->getRight()), lerp.x);
			}

			if (fy_ < deadzone->y)
			{
				sy_ = Math::linear(sy_, sy_ - (deadzone->y - fy_), lerp.y);
			}
			else if (fy_ > deadzone->getBottom())
			{
				sy_ = Math::linear(sy_, sy_ + (fy_ - deadzone->getBottom()), lerp.y);
			}
		}
		else
		{
			sx_ = Math::linear(sx_, fx_ - originX_, lerp.x);
			sy_ = Math::linear(sy_, fy_ - originY_, lerp.y);
		}

		emitFollowEvent_ = true;
	}

	if (useBounds) {
		sx_ = clampX(sx_);
		sy_ = clampY(sy_);
	}

	// Values are in pixels and not impacted by zooming the Camera
	setScrollX(sx_);
	setScrollY(sy_);

	int midX_ = sx_ + halfWidth_;
	int midY_ = sy_ + halfHeight_;

	// The center of the camera, in world space, so taking zoom into account
	// Basically the pixel value of what it's looking at in the middle of the cam
	midPoint.set(midX_, midY_);

	int displayWidth_ = width / zoomX;
	int displayHeight_ = height / zoomY;

	worldView.setTo(
			midX_ - (displayWidth_ / 2),
			midY_ - (displayHeight_ / 2),
			displayWidth_,
			displayHeight_
			);

	// No need to take the camera viewport into account as we're using SDL's
	// viewport which will offset and clip everything for us.
	matrix.applyITRS(originX_, originY_, rotation, zoomX, zoomY);
	matrix.translate(-originX_, -originY_);

	shakeEffect.preRender();

	if (emitFollowEvent_)
		emit("follow-update");
}

int Camera::clampX (int x_)
{
	int bx_ = bounds.x + ((getDisplayWidth() - width) / 2);
	int bw_ = std::max(bx_, bx_ + bounds.width - getDisplayWidth());

	if (x_ < bx_)
		x_ = bx_;
	else if (x_ > bw_)
		x_ = bw_;

	return x_;
}

int Camera::clampY (int y_)
{
	int by_ = bounds.y + ((getDisplayHeight() - height) / 2);
	int bh_ = std::max(by_, by_ + bounds.height - getDisplayHeight());

	if (y_ < by_)
		y_ = by_;
	else if (y_ > bh_)
		y_ = bh_;

	return y_;
}

Camera& Camera::removeBounds ()
{
	useBounds = false;

	dirty = true;

	bounds.setEmpty();

	return *this;
}

Camera& Camera::setAngle (double value_)
{
	setRotation(Math::degToRad(value_));

	return *this;
}

Camera& Camera::setBackgroundColor (int color_)
{
	backgroundColor.setFromHex(color_);

	transparent = (backgroundColor.alpha() == 0);

	return *this;
}

Camera& Camera::setBackgroundColor (int r_, int g_, int b_, int a_)
{
	backgroundColor.setTo(r_, g_, b_, a_);

	transparent = (backgroundColor.alpha() == 0);

	return *this;
}

Camera& Camera::setBackgroundColor (Display::Color color_)
{
	backgroundColor = color_;

	transparent = (backgroundColor.alpha() == 0);

	return *this;
}

Camera& Camera::setBounds (int x_, int y_, int width_, int height_, bool centerOn_)
{
	bounds.setTo(x_, y_, width_, height_);

	dirty = true;
	useBounds = true;

	if (centerOn_)
	{
		centerToBounds();
	}
	else
	{
		setScrollX( clampX(scrollX) );
		setScrollY( clampX(scrollY) );
	}

	return *this;
}

Geom::Rectangle Camera::getBounds ()
{
	return bounds;
}

Camera& Camera::setName (std::string value_)
{
	name = value_;

	return *this;
}

Camera& Camera::setPosition (int x_, int y_)
{
	setX(x_);
	setY(y_);

	return *this;
}

Camera& Camera::setPosition (int x_)
{
	return setPosition(x_, x_);
}

Camera& Camera::setRotation (double value_)
{
	rotation = value_;
	dirty = true;

	return *this;
}

Camera& Camera::setScene (Scene* scene_)
{
	if (scene != nullptr && customViewport)
		sceneManager->customViewports--;

	scene = scene_;

	sceneManager = &scene_->game.scene;
	scaleManager = &scene_->scale;
	cameraManager = &scene_->cameras;

	updateSystem();

	return *this;
}

Camera& Camera::setScroll (int x_, int y_)
{
	setScrollX(x_);
	setScrollY(y_);

	return *this;
}

Camera& Camera::setScroll (int x_)
{
	return setScroll(x_, x_);
}

Camera& Camera::setSize (int width_, int height_)
{
	setWidth(width_);
	setHeight(height_);

	return *this;
}

Camera& Camera::setSize (int size_)
{
	return setSize(size_, size_);
}

Camera& Camera::setViewport (int x_, int y_, int width_, int height_)
{
	setX(x_);
	setY(y_);
	setWidth(width_);
	setHeight((height_ < 0) ? width_ : height_);

	return *this;
}

Camera& Camera::setZoom (double x_, double y_)
{
	if (x_ == 0)
		x_ = 0.001;

	if (y_ == 0)
		y_ = 0.001;

	setZoomX(x_);
	setZoomY(y_);

	return *this;
}

Camera& Camera::setZoom (double value_)
{
	return setZoom(value_, value_);
}

Camera& Camera::setMask (GameObjects::GameObject& mask_, bool fixedPosition_)
{
	mask = &mask_;

	maskCamera = (fixedPosition_) ? &cameraManager->def : this;

	return *this;
}

Camera& Camera::clearMask ()
{
	mask = nullptr;
	maskCamera = nullptr;

	return *this;
}

void Camera::updateSystem ()
{
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
}

int Camera::getX ()
{
	return x;
}

Camera& Camera::setX (int x_)
{
	x = x_;
	updateSystem();

	return *this;
}

int Camera::getY ()
{
	return y;
}

Camera& Camera::setY (int y_)
{
	y = y_;
	updateSystem();

	return *this;
}

int Camera::getWidth ()
{
	return width;
}

Camera& Camera::setWidth (int width_)
{
	width = width_;
	updateSystem();

	return *this;
}

int Camera::getHeight ()
{
	return height;
}

Camera& Camera::setHeight (int height_)
{
	height = height_;
	updateSystem();

	return *this;
}

int Camera::getScrollX ()
{
	return scrollX;
}

Camera& Camera::setScrollX (int value_)
{
	scrollX = value_;
	dirty = true;

	return *this;
}

int Camera::getScrollY ()
{
	return scrollY;
}

Camera& Camera::setScrollY (int value_)
{
	scrollY = value_;
	dirty = true;

	return *this;
}

double Camera::getZoom ()
{
	return (zoomX + zoomY) / 2.0;
}

double Camera::getZoomX ()
{
	return zoomX;
}

Camera& Camera::setZoomX (double value_)
{
	zoomX = value_;
	dirty = true;

	return *this;
}

double Camera::getZoomY ()
{
	return zoomY;
}

Camera& Camera::setZoomY (double value_)
{
	zoomY = value_;
	dirty = true;

	return *this;
}

double Camera::getRotation ()
{
	return rotation;
}

double Camera::getCenterX ()
{
	return getX() + (0.5 * getWidth());
}

double Camera::getCenterY ()
{
	return getY() + (0.5 * getHeight());
}

int Camera::getDisplayWidth ()
{
	return getWidth() / getZoomX();
}

int Camera::getDisplayHeight ()
{
	return getHeight() / getZoomY();
}

Camera& Camera::setDeadzone (int width_, int height_)
{
	if (width_ < 0) {
		deadzone.reset();
	} else {
		if (deadzone) {
			deadzone->setSize(width_, height_);
		} else {
			deadzone = std::make_shared<Geom::Rectangle>(0, 0, width_, height_);
		}

		if (follow) {
			int originX_ = width / 2;
			int originY_ = height / 2;

			int fx_ = follow->getX() - followOffset.x;
			int fy_ = follow->getY() - followOffset.y;

			midPoint.set(fx_, fy_);

			setScrollX(fx_ - originX_);
			setScrollY(fy_ - originY_);
		}

		deadzone->centerOn(midPoint.x, midPoint.y);
	}

	return *this;
}

Camera& Camera::fadeIn (
		int duration_, int red_, int green_, int blue_)
{
	return fadeEffect.start(false, duration_, red_, green_, blue_, true);
}

Camera& Camera::fadeOut (int duration_, int red_, int green_, int blue_)
{
	return fadeEffect.start(true, duration_, red_, green_, blue_, true);
}

Camera& Camera::fadeFrom (int duration_, int red_, int green_, int blue_, bool force_)
{
	return fadeEffect.start(false, duration_, red_, green_, blue_, force_);
}

Camera& Camera::fade (int duration_, int red_, int green_, int blue_, bool force_)
{
	return fadeEffect.start(true, duration_, red_, green_, blue_, force_);
}

Camera& Camera::flash (int duration_, int red_, int green_, int blue_, bool force_)
{
	return flashEffect.start(duration_, red_, green_, blue_, force_);
}

Camera& Camera::shake (int duration_, Math::Vector2 intensity_, bool force_)
{
	return shakeEffect.start(duration_, intensity_, force_);
}

Camera& Camera::pan (int x_, int y_, int duration_, std::string ease_, bool force_)
{
	return panEffect.start(x_, y_, duration_, ease_, force_);
}

Camera& Camera::rotateTo (
		double radians_,
		bool shortestPath_,
		int duration_,
		std::string ease_,
		bool force_)
{
	return rotateToEffect.start(radians_, shortestPath_, duration_, ease_, force_);
}

Camera& Camera::zoomTo (double zoom_, int duration_, std::string ease_, bool force_)
{
	return zoomEffect.start(zoom_, duration_, ease_, force_);
}

Camera& Camera::setLerp (double x_, double y_)
{
	lerp.set(x_, y_);

	return *this;
}

Camera& Camera::setFollowOffset (int x_, int y_)
{
	followOffset.set(x_, y_);

	return *this;
}

Camera& Camera::startFollow (
		GameObjects::GameObject& target_,
		double lerpX_,
		double lerpY_,
		int offsetX_,
		int offsetY_)
{
	follow = &target_;

	lerpX_ = Math::clamp(lerpX_, 0.0, 1.0);
	lerpY_ = Math::clamp(lerpY_, 0.0, 1.0);

	lerp.set(lerpX_, lerpY_);

	followOffset.set(offsetX_, offsetY_);

	int originX_ = getWidth() / 2;
	int originY_ = getHeight() / 2;

	int fx_ = target_.getX() - offsetX_;
	int fy_ = target_.getY() - offsetY_;

	midPoint.set(fx_, fy_);

	setScrollX(fx_ - originX_);
	setScrollY(fy_ - originY_);

	if (useBounds) {
		setScrollX( clampX( getScrollX() ) );
		setScrollY( clampY( getScrollY() ) );
	}

	return *this;
}

Camera& Camera::stopFollow ()
{
	follow = nullptr;

	return *this;
}

Camera& Camera::resetFX ()
{
	rotateToEffect.reset();
	panEffect.reset();
	shakeEffect.reset();
	flashEffect.reset();
	fadeEffect.reset();

	return *this;
}

void Camera::update (Uint32 time_, Uint32 delta_)
{
	if (visible) {
		rotateToEffect.update(time_, delta_);
		panEffect.update(time_, delta_);
		zoomEffect.update(time_, delta_);
		shakeEffect.update(time_, delta_);
		flashEffect.update(time_, delta_);
		fadeEffect.update(time_, delta_);
	}
}

}	// namespace Scene2D
}	// namespace Cameras
}	// namespace Zen
