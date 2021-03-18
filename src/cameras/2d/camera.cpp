/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "camera.h"

namespace Zen {
namespace Cameras {
namespace Scene2D {

Camera::Camera (int x_, int y_, int width_, int height_)
	: BaseCamera(x_, y_, width_, height_)
	, lerp (1.0, 1.0)
	, zoomEffect (*this)
	, rotateToEffect (*this)
	, panEffect (*this)
	, shakeEffect (*this)
	, flashEffect (*this)
	, fadeEffect (*this)
{}

Camera::~Camera ()
{
	resetFX();
}

int Camera::getComponentMask ()
{
	return COMPONENT_MASK;
}

Camera& Camera::setDeadzone (int width_ = -1, int height_ = -1)
{
	if (width_ < 0) {
		deadzone.reset();
	} else {
		if (deadzone) {
			deadzone->setWidth(width_);
			deadzone->setHeight(height_);
		} else {
			deadzone = std::make_shared<Geom::Rectangle>(0, 0, width_, height_);
		}

		if (follow) {
			int originX_ = width / 2;
			int originY_ = height / 2;

			int fx_ = follow->x - followOffset.x;
			int fy_ = follow->y - followOffset.y;

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

void Camera::preRender ()
{
	int halfWidth_ = getWidth() * 0.5;
	int halfHeight_ = getHeight() * 0.5;

	int originX_ = getWidth() * originX;
	int originY_ = getHeight() * originY;

	int sx_ = getScrollX();
	int sy_ = getScrollY();

	if (deadzone)
		deadzone->centerOn(midPoint.x, midPoint.y);

	bool emitFollowEvent_ = false;

	if (follow && !panEffect.isRunning) {
		int fx_ = follow->x - followOffset.x;
		int fy_ = follow->y - followOffset.y;

		if (deadzone) {
			if (fx_ < deadzone->getX())
				sx_ = Math::linear(sx_, sx_ - (deadzone->getX() - fx_), lerp.x);
			else if (fx > deadzone_->getRight())
				sx_ = Math::linear(sx_, sx_ + (fx_ - deadzone->getRight()), lerp.x)

					if (fy_ < deadzone->getY())
						sy_ = Math::linear(sy_, sy_ - (deadzone->getY() - fy_), lerp.y);
					else if (fy > deadzone_->getBottom())
						sy_ = Math::linear(sy_, sy_ + (fy_ - deadzone->getBottom()), lerp.y)
		} else {
			sx_ = Math::linear(sx_, fx_ - originX_, lerp.x);
			sy_ = Math::linear(sy_, fy_ - originY_, lerp.y);
		}

		emitFollowEvent_ = true;
	}

	if (useBounds) {
		sx_ = clampX(sx_);
		sy_ = clampY(sy_);
	}

	//  Values are in pixels and not impacted by zooming the Camera
	setScrollX(sx_);
	setScrollY(sy_);

	int midX_ = sx_ + halfWidth_;
	int midY_ = sy_ + halfHeight_;

	//  The center of the camera, in world space, so taking zoom into account
	//  Basically the pixel value of what it's looking at in the middle of the cam
	midPoint.set(midX_, midY_);

	int displayWidth_ = width / getZoom();
	int displayHeight_ = height / getZoom();

	int vwx_ = midX_ - (displayWidth_ / 2);
	int vwy_ = midY_ - (displayHeight_ / 2);

	worldView.setTo(vwx_, vwy_, displayWidth_, displayHeight_);

	matrix.applyITRS(getX(), + originX_, getY() + originY_, getRotation(), getZoom(), getZoom());
	matrix.translate(-originX_, -originY_);

	shakeEffect.preRender();

	if (emitFollowEvent_)
		emit("follow-update");
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

	int fx_ = target_.x - offsetX_;
	int fy_ = target_.y - offsetY_;

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

}	// Scene2D
}	// Cameras
}	// Zen
