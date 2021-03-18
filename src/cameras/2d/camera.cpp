/**
 * @file		camera.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include <memory>
namespace Zen {
namespace Cameras {
namespace Scene2D {
Camera::Camera (int x, int y, int width, int height)
	: BaseCamera(x, y, width, height)
{}

Camera::~Camera ()
{
	resetFX();
}

int Camera::getComponentMask ()
{
	return COMPONENT_MASK;
}

Camera& Camera::setDeadzone (int width = -1, int height = -1)
{
	if (width < 0) {
		deadzone_.reset();
	} else {
		if (deadzone_) {
			deadzone_->setWidth(width);
			deadzone_->setHeight(height);
		} else {
			deadzone_ = std::make_shared<Geom::Rectangle>(0, 0, width, height);
		}

		if (follow_) {
			int originX = width_ / 2;
			int originY = height_ / 2;

			int fx = follow_->x - followOffset_.x;
			int fy = follow_->y - followOffset_.y;

			midPoint_.set(fx, fy);

			setScrollX(fx - originX);
			setScrollY(fy - originY);
		}

		deadzone_->centerOn(midPoint_.x, midPoint_.y);
	}

	return *this;
}

void Camera::preRender ()
{
	int halfWidth = getWidth() * 0.5;
	int halfHeight = getHeight() * 0.5;

	int originX = getWidth() * originX_;
	int originY = getHeight() * originY_;

	int sx = getScrollX();
	int sy = getScrollY();

	if (deadzone_)
		deadzone_->centerOn(midPoint_.x, midPoint_.y);

	bool emitFollowEvent = false;

	if (follow_ && !panEffect.isRunning) {
		int fx = follow_->x - followOffset_.x;
		int fy = follow_->y - followOffset_.y;

		if (deadzone_) {
			if (fx < deadzone_->getX())
				sx = Math::linear(sx, sx - (deadzone_->getX() - fx), lerp_.x);
			else if (fx > deadzone_->getRight())
				sx = Math::linear(sx, sx + (fx - deadzone_->getRight()), lerp_.x)

			if (fy < deadzone_->getY())
				sy = Math::linear(sy, sy - (deadzone_->getY() - fy), lerp_.y);
			else if (fy > deadzone_->getBottom())
				sy = Math::linear(sy, sy + (fy - deadzone_->getBottom()), lerp_.y)
		} else {
			sx = Math::linear(sx, fx - originX, lerp_.x);
			sy = Math::linear(sy, fy - originY, lerp_.y);
		}

		emitFollowEvent = true;
	}

	if (useBounds_) {
		sx = clampX(sx);
		sy = clampY(sy);
	}

	//  Values are in pixels and not impacted by zooming the Camera
	setScrollX(sx);
	setScrollY(sy);

	int midX = sx + halfWidth;
	int midY = sy + halfHeight;

	//  The center of the camera, in world space, so taking zoom into account
	//  Basically the pixel value of what it's looking at in the middle of the cam
	midPoint_.set(midX, midY);

	int displayWidth = width_ / getZoom();
	int displayHeight = height_ / getZoom();

	int vwx = midX  (displayWidth / 2);
	int vwy = midY  (displayHeight / 2);

	worldView_.setTo(vwx, vwy, displayWidth, displayHeight);

	matrix_.applyITRS(getX(), + originX, getY() + originY, getRotation(), getZoom(), getZoom());
	matrix_.translate(-originX, -originY);

	shakeEffect.preRender();

	if (emitFollowEvent)
		emit("SYS_FOLLOW_UPDATE");
}

Camera& Camera::setLerp (float x, float y)
{
	lerp_.set(x, y);

	return *this;
}

Camera& Camera::setFollowOffset (int x, int y)
{
	followOffset_.set(x, y);

	return *this;
}

Camera& Camera::startFollow (GameObjects::GameObject& target, float lerpX, float lerpY, int offsetX, int offsetY)
{
	follow_ = &target;

	lerpX = Math::clamp(lerpX, 0.0, 1.0);
	lerpY = Math::clamp(lerpY, 0.0, 1.0);

	lerp_.set(lerpX, lerpY);

	followOffset_.set(offsetX, offsetY);

	int originX = getWidth() / 2;
	int originY = getHeight() / 2;

	int fx = target.x - offsetX;
	int fy = target.y - offsetY;

	midPoint_.set(fx, fy);

	setScrollX(fx - originX);
	setScrollY(fy - originY);

	if (useBounds_) {
		setScrollX(clampX(getScrollX()));
		setScrollY(clampY(getScrollY()));
	}

	return *this;
}

Camera& Camera::stopFollow ()
{
	follow_ = nullptr;

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

void Camera::update (Uint32 time, Uint32 delta)
{
	if (visible) {
		rotateToEffect.update(time, delta);
		panEffect.update(time, delta);
		zoomEffect.update(time, delta);
		shakeEffect.update(time, delta);
		flashEffect.update(time, delta);
		fadeEffect.update(time, delta);
	}
}
}	// Scene2D
}	// Cameras
}	// Zen
