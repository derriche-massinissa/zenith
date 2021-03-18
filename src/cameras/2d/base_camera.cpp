/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "base_camera.h"

namespace Zen {
namespace Cameras {
namespace Scene2D {

BaseCamera::BaseCamera (int x_, int y_, int width_, int height_)
	: x (x_)
	, y (y_)
	, width (width_)
	, height (height_)
	, midPoint (width_ / 2, height_ / 2)
	{}

BaseCamera::~BaseCamera ()
{
	emit("destroy");

	removeAllListeners();

	culledObjects.clear();

	if (customViewport)
		// We're turning off a custom viewport for this Camera
		sceneManager->customViewports--;

	renderList.clear();
}

int BaseCamera::getComponentMask ()
{
	return COMPONENT_MASK;
}

void BaseCamera::addToRenderList (GameObjects::GameObject& child_)
{
	renderList.emplace_back(&child_);
}

BaseCamera& BaseCamera::setOrigin (double x_, double y_)
{
	originX = x_;
	originY = y_;

	return *this;
}

BaseCamera& BaseCamera::setOrigin (double x_)
{
	return setOrigin(x_, x_);
}

Math::Vector2 BaseCamera::getScroll (int x_, int y_)
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

BaseCamera& BaseCamera::centerOnX (int x_)
{
	double originX_ = width * 0.5;

	midPoint.x = x_;

	setScrollX(x_ - originX_);

	if (useBounds)
		setScrollX( clampX( getScrollX() ) );

	return *this;
}

BaseCamera& BaseCamera::centerOnY (int y_)
{
	double originY_ = height * 0.5;

	midPoint.y = y_;

	setScrollY(y_ - originY_);

	if (useBounds)
		setScrollY( clampY( getScrollY() ) );

	return *this;
}

BaseCamera& BaseCamera::centerOn (int x_, int y_)
{
	centerOnX(x_);
	centerOnY(y_);

	return *this;
}

BaseCamera& BaseCamera::centerToBounds ()
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

BaseCamera& BaseCamera::centerToSize ()
{
	setScrollX(width * 0.5);
	setScrollY(height * 0.5);

	return *this;
}

std::vector<GameObjects::GameObject*> BaseCamera::cull (
		std::vector<GameObjects::GameObject*> renderableObjects_)
{
	if (disableCull)
		return renderableObjects_;

	auto& cameraMatrix_ = matrix.matrix;

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
		if (~object_->getComponentMask() & COMPONENT_MASK_SIZE
			|| object_->parentContainer != nullptr)
		{
			culledObjects.emplace_back(object_);
			continue;
		}

		auto objectW_ = object_->width;
		auto objectH_ = object_->height;
		auto objectX_ = (object_->x - (scrollX * object_->getScrollFactorX())) - (objectW_ * object_->getOriginX());
		auto objectY_ = (object_->y - (scrollY * object_->getScrollFactorY())) - (objectH_ * object_->getOriginY());
		auto tx_ = (objectX_ * mva_ + objectY_ * mvc_ + mve_);
		auto ty_ = (objectX_ * mvb_ + objectY_ * mvd_ + mvf_);
		auto tw_ = ((objectX_ + objectW_) * mva_ + (objectY_ + objectH_) * mvc_ + mve_);
		auto th_ = ((objectX_ + objectW_) * mvb_ + (objectY_ + objectH_) * mvd_ + mvf_);

		if ((tw_ > cullLeft_ && tx_ < cullRight_) && (th_ > cullTop_ && ty_ < cullBottom_))
		{
			culledObjects.emplace_back(object_);
		}
	}

	return culledObjects;
}

Math::Vector2 BaseCamera::getWorldPoint (int x_, int y_)
{
	Math::Vector2 output_;

	auto& cameraMatrix_ = matrix.matrix;

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

BaseCamera& BaseCamera::ignore (GameObjects::GameObject* entry_)
{
	entry_->cameraFilter |= id;

	return *this;
}

BaseCamera& BaseCamera::ignore (
		std::vector<GameObjects::GameObject*>& entries_)
{
	for (auto it_ : entries_)
		ignore(it_);

	return *this;
}

BaseCamera& BaseCamera::ignore (GameObjects::Group& entry_)
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

void BaseCamera::preRender ()
{
	renderList.clear();

	int halfWidth_ = width * 0.5;
	int halfHeight_ = height * 0.5;

	int originX_ = width * originX;
	int originY_ = height * originY;

	int sx_ = scrollX;
	int sy_ = scrollY;

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

	matrix.applyITRS(x + originX_, y + originY_, rotation, zoomX, zoomY);
	matrix.translate(-originX_, -originY_);
}

int BaseCamera::clampX (int x_)
{
	int bx_ = bounds.x + ((getDisplayWidth() - width) / 2);
	int bw_ = std::max(bx_, bx_ + bounds.width - getDisplayWidth());

	if (x_ < bx_)
		x_ = bx_;
	else if (x_ > bw_)
		x_ = bw_;

	return x_;
}

int BaseCamera::clampY (int y_)
{
	int by_ = bounds.y + ((getDisplayHeight() - height) / 2);
	int bh_ = std::max(by_, by_ + bounds.height - getDisplayHeight());

	if (y_ < by_)
		y_ = by_;
	else if (y_ > bh_)
		y_ = bh_;

	return y_;
}

BaseCamera& BaseCamera::removeBounds ()
{
	useBounds = false;

	dirty = true;

	bounds.setEmpty();

	return *this;
}

BaseCamera& BaseCamera::setAngle (double value_)
{
	setRotation(Math::degToRad(value_));

	return *this;
}

BaseCamera& BaseCamera::setBackgroundColor (int color_)
{
	backgroundColor.setFromHex(color_);

	transparent = (backgroundColor.alpha() == 0);

	return *this;
}

BaseCamera& BaseCamera::setBackgroundColor (int r_, int g_, int b_, int a_)
{
	backgroundColor.setTo(r_, g_, b_, a_);

	transparent = (backgroundColor.alpha() == 0);

	return *this;
}

BaseCamera& BaseCamera::setBounds (int x_, int y_, int width_, int height_, bool centerOn_)
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

Geom::Rectangle BaseCamera::getBounds ()
{
	return bounds;
}

BaseCamera& BaseCamera::setName (std::string value_)
{
	name = value_;

	return *this;
}

BaseCamera& BaseCamera::setPosition (int x_, int y_)
{
	setX(x_);
	setY(y_);

	return *this;
}

BaseCamera& BaseCamera::setPosition (int x_)
{
	return setPosition(x_, x_);
}

BaseCamera& BaseCamera::setRotation (double value_)
{
	rotation = value_;
	dirty = true;

	return *this;
}

BaseCamera& BaseCamera::setScene (Scene* scene_)
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

BaseCamera& BaseCamera::setScroll (int x_, int y_)
{
	setScrollX(x_);
	setScrollY(y_);

	return *this;
}

BaseCamera& BaseCamera::setScroll (int x_)
{
	return setScroll(x_, x_);
}

BaseCamera& BaseCamera::setSize (int width_, int height_)
{
	setWidth(width_);
	setHeight(height_);

	return *this;
}

BaseCamera& BaseCamera::setSize (int size_)
{
	return setSize(size_, size_);
}

BaseCamera& BaseCamera::setViewport (int x_, int y_, int width_, int height_)
{
	setX(x_);
	setY(y_);
	setWidth(width_);
	setHeight((height_ < 0) ? width_ : height_);

	return *this;
}

BaseCamera& BaseCamera::setZoom (double x_, double y_)
{
	if (x_ == 0)
		x_ = 0.001;

	if (y_ == 0)
		y_ = 0.001;

	setZoomX(x_);
	setZoomY(y_);

	return *this;
}

BaseCamera& BaseCamera::setMask (Display::Masks::Mask mask_, bool fixedPosition_)
{
	mask = std::make_unique<Display::Masks::Mask>();
	*mask = mask_;

	//maskCamera = (fixedPosition_) ? static_cast<BaseCamera*>(&cameraManager->def) : this;

	return *this;
}

BaseCamera& BaseCamera::clearMask ()
{
	mask = nullptr;
	maskCamera = nullptr;

	return *this;
}

void BaseCamera::update (Uint32 time_, Uint32 delta_)
{}

void BaseCamera::updateSystem ()
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

int BaseCamera::getX ()
{
	return x;
}

BaseCamera& BaseCamera::setX (int x_)
{
	x = x_;
	updateSystem();

	return *this;
}

int BaseCamera::getY ()
{
	return y;
}

BaseCamera& BaseCamera::setY (int y_)
{
	y = y_;
	updateSystem();

	return *this;
}

int BaseCamera::getWidth ()
{
	return width;
}

BaseCamera& BaseCamera::setWidth (int width_)
{
	width = width_;
	updateSystem();

	return *this;
}

int BaseCamera::getHeight ()
{
	return height;
}

BaseCamera& BaseCamera::setHeight (int height_)
{
	height = height_;
	updateSystem();

	return *this;
}

int BaseCamera::getScrollX ()
{
	return scrollX;
}

BaseCamera& BaseCamera::setScrollX (int value_)
{
	scrollX = value_;
	dirty = true;

	return *this;
}

int BaseCamera::getScrollY ()
{
	return scrollY;
}

BaseCamera& BaseCamera::setScrollY (int value_)
{
	scrollY = value_;
	dirty = true;

	return *this;
}

double BaseCamera::getZoom ()
{
	return (zoomX + zoomY) / 2.0;
}

double BaseCamera::getZoomX ()
{
	return zoomX;
}

BaseCamera& BaseCamera::setZoomX (double value_)
{
	zoomX = value_;
	dirty = true;

	return *this;
}

double BaseCamera::getZoomY ()
{
	return zoomY;
}

BaseCamera& BaseCamera::setZoomY (double value_)
{
	zoomY = value_;
	dirty = true;

	return *this;
}

double BaseCamera::getRotation ()
{
	return rotation;
}

double BaseCamera::getCenterX ()
{
	return getX() + (0.5 * getWidth());
}

double BaseCamera::getCenterY ()
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
