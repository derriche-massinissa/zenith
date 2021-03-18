/**
 * @file		base_camera.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "base_camera.h"
#include <memory>

namespace Zen {
namespace Cameras {
namespace Scene2D {
BaseCamera::BaseCamera (int x, int y, int width, int height)
	: x_ (x)
	, y_ (y)
	, width_ (width)
	, height_ (height)
	, midPoint_ (width / 2, height / 2)
	{}

BaseCamera::~BaseCamera ()
{
	emit("SYS_DESTROY");

	removeAllListeners();

	culledObjects_.clear();

	if (customViewport_)
		// We're turning off a custom viewport for this Camera
		sceneManager_->customViewports--;

	renderList_.clear();

	scene = nullptr;
	scaleManager_ = nullptr;
	sceneManager_ = nullptr;
	cameraManager_ = nullptr;
}

int BaseCamera::getComponentMask ()
{
	return COMPONENT_MASK;
}

void BaseCamera::addToRenderList (GameObjects::GameObject& child)
{
	renderList_.emplace_back(&child);
}

BaseCamera& BaseCamera::setOrigin (float x, float y)
{
	originX_ = x;
	originY_ = y;

	return *this;
}

BaseCamera& BaseCamera::setOrigin (float x = 0.5)
{
	return setOrigin(x, x);
}

Math::Vector2 BaseCamera::getScroll (int x, int y)
{
	Math::Vector2 out;

	auto originX = width_ * 0.5;
	auto originY = height_ * 0.5;

	out.x = x - originX;
	out.y = y - originY;

	if (useBounds_) {
		out.x = clampX(out.x);
		out.y = clampX(out.y);
	}

	return out;
}

BaseCamera& BaseCamera::centerOnX (int x)
{
	float originX = width_ * 0.5;

	midPoint_.x = x;

	setScrollX(x - originX);

	if (useBounds_)
		setScrollX(clampX(getScrollX()));

	return *this;
}

BaseCamera& BaseCamera::centerOnY (int y)
{
	float originY = height_ * 0.5;

	midPoint_.y = y;

	setScrollY(y - originY);

	if (useBounds_)
		setScrollY(clampY(getScrollY()));

	return *this;
}

BaseCamera& BaseCamera::centerOn (int x, int y)
{
	centerOnX(x);
	centerOnY(y);

	return *this;
}

BaseCamera& BaseCamera::centerToBounds ()
{
	if (useBounds_) {
		auto originX = width_ * 0.5;
		auto originY = height_ * 0.5;

		midPoint_.set(bounds_.centerX(), bounds_.centerY());

		setScrollX(bounds_.centerX() - originX);
		setScrollY(bounds_.centerY() - originY);
	}

	return *this;
}

BaseCamera& BaseCamera::centerToSize ()
{
	setScrollX(width_ * 0.5);
	setScrollY(height_ * 0.5);

	return *this;
}

std::vector<GameObjects::GameObject*> cull (
		std::vector<GameObjects::GameObject*> renderableObjects)
{
	if (disableCull_)
		return renderableObjects;

	auto& cameraMatrix = matrix_.matrix;

	auto mva = cameraMatrix[0];
	auto mvb = cameraMatrix[1];
	auto mvc = cameraMatrix[2];
	auto mvd = cameraMatrix[3];

	// First Invert Matrix
	auto determinant = (mva * mvd) - (mvb * mvc);

	if (!determinant)
		return renderableObjects;

	auto mve = cameraMatrix[4];
	auto mvf = cameraMatrix[5];

	auto cullTop = y_;
	auto cullBottom = y_ + height_;
	auto cullLeft = x_;
	auto cullRight = x_ + width_;

	determinant = 1 / determinant;

	culledObjects_.clear();

	for (auto object : renderableObjects) {
		if (~object->getComponentMask() & COMPONENT_MASK_SIZE
				|| object->parentContainer != nullptr)
		{
			culledObjects_.emplace_back(object);
			continue;
		}

		auto objectW = object->width;
		auto objectH = object->height;
		auto objectX = (object->x - (scrollX_ * object->scrollFactorX)) - (objectW * object->originX);
		auto objectY = (object->y - (scrollY_ * object->scrollFactorY)) - (objectH * object->originY);
		auto tx = (objectX * mva + objectY * mvc + mve);
		auto ty = (objectX * mvb + objectY * mvd + mvf);
		auto tw = ((objectX + objectW) * mva + (objectY + objectH) * mvc + mve);
		auto th = ((objectX + objectW) * mvb + (objectY + objectH) * mvd + mvf);

		if ((tw > cullLeft && tx < cullRight) && (th > cullTop && ty < cullBottom))
		{
			culledObjects_.emplace_back(object);
		}
	}

	return culledObjects_;
}

Math::Vector2 BaseCamera::getWorldPoint (int x, int y)
{
	Math::Vector2 output;

	auto& cameraMatrix = matrix_.matrix;

	auto mva = cameraMatrix[0];
	auto mvb = cameraMatrix[1];
	auto mvc = cameraMatrix[2];
	auto mvd = cameraMatrix[3];
	auto mve = cameraMatrix[4];
	auto mvf = cameraMatrix[5];

	// Invert Matrix
	float determinant = (mva * mvd) - (mvb * mvc);

	if (!determinant) {
		output.x = x;
		output.y = y;

		return output;
	}

	determinant = 1 / determinant;

	auto ima = mvd * determinant;
	auto imb = -mvb * determinant;
	auto imc = -mvc * determinant;
	auto imd = mva * determinant;
	auto ime = (mvc * mvf - mvd * mve) * determinant;
	auto imf = (mvb * mve - mva * mvf) * determinant;

	float c = std::cos(rotation_);
	float s = std::sin(rotation_);

	float sx = x + ((scrollX_ * c - scrollY_ * s) * zoomX_);
	float sy = y + ((scrollX_ * s - scrollY_ * c) * zoomY_);

	// Apply transform to point
	output.x = (sx * ima + sy * imc) + ime;
	output.y = (sx * imb + sy * imd) + imf;

	return output;
}

BaseCamera& BaseCamera::ignore (GameObjects::GameObject* entry)
{
	entry->cameraFilter |= id_;

	return *this;
}

BaseCamera& BaseCamera::ignore (
		std::vector<GameObjects::GameObject*>& entries)
{
	for (auto it : entries)
		ignore(it);

	return *this;
}

BaseCamera& BaseCamera::ignore (GameObjects::Group& entry)
{
	auto children = entry.getChildren().begin();
	auto end = entry.getChildren().end();

	for (; children != end; children++) {
		if (children->isParent)
			// A group in the group
			ignore(*children);
		else
			children->cameraFilter |= id_;
	}

	return *this;
}

void BaseCamera::preRender ()
{
	renderList_.clear();

	int halfWidth = width_ * 0.5;
	int halfHeight = height_ * 0.5;

	int originX = width_ * originX_;
	int originY = height_ * originY_;

	int sx = scrollX_;
	int sy = scrollY_;

	if (useBounds_) {
		sx = clampX(sx);
		sY = clampY(sy);
	}

	// Values are in pixels and not impacted by zooming the Camera
	setScrollX(sx);
	setScrollY(sy);

	int midX = sx + halfWidth;
	int midY = sy + halfHeight;

	// The center of the camera, in world space, so taking zoom into account
	// Basically the pixel value of what it's looking at in the middle of the cam
	midPoint_.set(midX, midY);

	int displayWidth = width_ / zoomX_;
	int displayHeight = height_ / zoomY_;

	worldView_.setTo(
			midX - (displayWidth / 2),
			midY - (displayHeight / 2),
			displayWidth,
			displayHeight
			);

	matrix_.applyITRS(x_ + originX, y_ + originY, rotation_, zoomX_, zoomY_);
	matrix_.translate(-originX, -originY);
}

int BaseCamera::clampX (int x)
{
	int bx = bounds_.x + ((displayWidth_ - width_) / 2);
	int bw = std::max(bx, bx + bounds_.width - displayWidth_);

	if (x < bx)
		x = bx;
	else if (x > bw)
		x = bw;

	return x;
}

int BaseCamera::clampY (int y)
{
	int by = bounds_.y + ((displayHeight_ - height_) / 2);
	int bh = std::max(by, by + bounds_.height - displayHeight_);

	if (y < by)
		y = by;
	else if (y > bh)
		y = bh;

	return y;
}

BaseCamera& BaseCamera::removeBounds ()
{
	useBounds_ = false;

	dirty_ = true;

	bounds_.setEmpty();

	return *this;
}

BaseCamera& BaseCamera::setAngle (float value)
{
	setRotation(Math::degToRad(value));

	return *this;
}

BaseCamera& BaseCamera::setBackgroundColor (std::string color)
{
	backgroundColor_ = Display::Color::StringToColor(color);

	transparent_ = (backgroundColor_.alpha == 0);

	return *this;
}

BaseCamera& BaseCamera::setBackgroundColor (int color)
{
	backgroundColor_ = Display::Color::IntegerToColor(color);

	transparent_ = (backgroundColor_.alpha == 0);

	return *this;
}

BaseCamera& BaseCamera::setBackgroundColor (int r, int g, int b, int a)
{
	backgroundColor_ = Display::Color::RGBAToColor(r, g, b, a);

	transparent_ = (backgroundColor_.alpha == 0);

	return *this;
}

BaseCamera& BaseCamera::setBounds (int x, int y, int width, int height, bool centerOn)
{
	bounds_.setTo(x, y, width, height);

	dirty_ = true;
	useBounds_ = true;

	if (centerOn) {
		centerToBounds();
	} else {
		setScrollX(clampX(scrollX_));
		setScrollY(clampX(scrollY_));
	}

	return *this;
}

Geom::Rectangle BaseCamera::getBounds ()
{
	return bounds_;
}

BaseCamera& BaseCamera::setName (std::string value)
{
	name_ = value;

	return *this;
}

BaseCamera& BaseCamera::setPosition (int x, int  y)
{
	setX(x);
	setY(y);

	return *this;
}

BaseCamera& BaseCamera::setPosition (int x)
{
	return setPosition(x, x);
}

BaseCamera& BaseCamera::setRotation (float value)
{
	rotation_ = value;
	dirty_ = true;

	return *this;
}

BaseCamera& BaseCamera::setScene (Scene* scene)
{
	if (scene_ != nullptr && customViewport_)
		sceneManager_->customViewport_--;

	scene_ = scene;

	sceneManager_ = &scene->game.scene;
	scaleManager_ = &scene->sys.scale;
	cameraManager_ = &scene->sys.cameras;

	updateSystem();

	return *this;
}

BaseCamera& BaseCamera::setScroll (int x, int y)
{
	setScrollX(x);
	setScrollY(y);

	return *this;
}

BaseCamera& BaseCamera::setScroll (int x)
{
	return setScroll(x, x);
}

BaseCamera& BaseCamera::setSize (int width, int height)
{
	setWidth(width);
	setHeight(height);

	return *this;
}

BaseCamera& BaseCamera::setSize (int size)
{
	return setSize(size, size);
}

BaseCamera& BaseCamera::setViewport (int x, int y, int width, int height)
{
	setX(x);
	setY(y);
	setWidth(width);
	setHeight((height < 0) ? width : height);

	return *this;
}

BaseCamera& BaseCamera::setZoom (float x, float y)
{
	if (x == 0)
		x = 0.001;

	if (y == 0)
		y = 0.001;

	setZoomX(x);
	setZoomY(y);

	return *this;
}

BaseCamera& BaseCamera::setMask (Display::Masks::Mask mask, bool fixedPosition)
{
	mask_ = std::make_unique<Display::Masks::Mask>();
	*mask_ = mask;

	maskCamera_ = (fixedPosition) ? cameraManager_->def : this;

	return *this;
}

BaseCamera& BaseCamera::clearMask ()
{
	mask_ = nullptr;

	return * this;
}

void BaseCamera::update (Uint32 time, Uint32 delta)
{}

void BaseCamera::updateSystem ()
{
	if (scaleManager_ == nullptr)
		return;

	bool custom = (x_ != 0 || y_ !=0 || scaleManager_->width != width_ || scaleManager_->height != height_);

	if (custom && !customViewport_)
		// We need a custom viewport for this camera
		sceneManager_->customViewports++;
	else if (!custom && customViewport_)
		// We're turning off a custom viewport for this Camera
		sceneManager_->customViewports--;

	dirty_ = true;
	customViewport_ = custom;
}

int BaseCamera::getX ()
{
	return x_;
}

BaseCamera& BaseCamera::setX (int x)
{
	x_ = x;
	updateSystem();

	return *this;
}

int BaseCamera::getY ()
{
	return y_;
}

BaseCamera& BaseCamera::setY (int y)
{
	y_ = y;
	updateSystem();

	return *this;
}

int BaseCamera::getWidth ()
{
	return width_;
}

BaseCamera& BaseCamera::setWidth (int width)
{
	width_ = width;
	updateSystem();

	return *this;
}

int BaseCamera::getHeight ()
{
	return height_;
}

BaseCamera& BaseCamera::setHeight (int height)
{
	height_ = height;
	updateSystem();

	return *this;
}

int BaseCamera::getScrollX ()
{
	return scrollX_;
}

BaseCamera& BaseCamera::setScrollX (int value)
{
	scrollX_ = value;
	dirty_ = true;

	return *this;
}

int BaseCamera::getScrollY ()
{
	return scrollY_;
}

BaseCamera& BaseCamera::setScrollY (int value)
{
	scrollY_ = value;
	dirty_ = true;

	return *this;
}

float BaseCamera::getZoom ()
{
	return (zoomX_ + zoomY_) / 2.0;
}

float BaseCamera::getZoomX ()
{
	return zoomX_;
}

BaseCamera& BaseCamera::setZoomX (float value)
{
	zoomX_ = value;
	dirty_ = true;

	return *this;
}

float BaseCamera::getZoomY ()
{
	return zoomY_;
}

BaseCamera& BaseCamera::setZoomY (float value)
{
	zoomY_ = value;
	dirty_ = true;

	return *this;
}

float BaseCamera::getRotation ()
{
	return rotation_;
}

float BaseCamera::getCenterX ()
{
	return getX() + (0.5 * getWidth());
}

float BaseCamera::getCenterY ()
{
	return getY() + (0.5 * getHeight());
}

int BaseCamera::getDisplayWidth ()
{
	return getWidth() / getZoomX();
}

int BaseCamera::getDisplayHeight ()
{
	return getHeight() / getZoomY();
}
}	// namespace Scene2D
}	// namespace Cameras
}	// namespace Zen
