/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "vector2.h"
#include <cmath>
#include "fuzzy/equal.h"

namespace Zen {
namespace Math {

Vector2::Vector2 (double v_)
	: x (v_), y (v_)
{}

Vector2::Vector2 (double x_, double y_)
	: x (x_), y (y_)
{}

Vector2::Vector2 (const Vector2& other_)
	: x (other_.x), y (other_.y)
{}

Vector2& Vector2::operator=(const Vector2& other_)
{
	x = other_.x;
	y = other_.y;

	return *this;
}

bool Vector2::operator == (const Vector2& other_)
{
	return equals(other_);
}

Vector2& Vector2::set (double x_)
{
	x = x_;
	y = x_;

	return *this;
}

Vector2& Vector2::setX (double x_)
{
	x = x_;

	return *this;
}

Vector2& Vector2::setY (double y_)
{
	y = y_;

	return *this;
}

Vector2& Vector2::set (double x_, double y_)
{
	x = x_;
	y = y_;

	return *this;
}

Vector2& Vector2::setTo (double x_)
{
	return set(x_);
}

Vector2& Vector2::setTo (double x_, double y_)
{
	return set(x_, y_);
}

Vector2& Vector2::setToPolar (double azimuth_, double radius_)
{
	x = std::cos(azimuth_) * radius_;
	y = std::sin(azimuth_) * radius_;

	return *this;
}

bool Vector2::equals (const Vector2& other_)
{
	return ((x == other_.x) && (y == other_.y));
}

bool Vector2::fuzzyEquals (const Vector2& other_, double epsilon_)
{
	return (Math::Fuzzy::equal(x, other_.x, epsilon_) &&
			Math::Fuzzy::equal(y, other_.y, epsilon_));
}

double Vector2::angle ()
{
	// computes the angle in radians with respect to the positive x-axis

	double angle_ = std::atan2(y, x);

	if (angle_ < 0)
		angle_ += 2 * M_PI;

	return angle_;
}

Vector2& Vector2::setAngle (double angle_)
{
	return setToPolar(angle_, length());
}

Vector2& Vector2::add (const Vector2& source_)
{
	x += source_.x;
	y += source_.y;

	return *this;
}

Vector2& Vector2::subtract (const Vector2& source_)
{
	x -= source_.x;
	y -= source_.y;

	return *this;
}

Vector2& Vector2::multiply (const Vector2& source_)
{
	x *= source_.x;
	y *= source_.y;

	return *this;
}

Vector2& Vector2::scale (double value_)
{
	x *= value_;
	y *= value_;

	return *this;
}

Vector2& Vector2::divide (const Vector2& source_)
{
	x /= source_.x;
	y /= source_.y;

	return *this;
}

Vector2& Vector2::negate ()
{
	x = -x;
	y = -y;

	return *this;
}

double Vector2::distance (const Vector2& source_)
{
	double dx_ = source_.x - x;
	double dy_ = source_.y - y;

	return std::sqrt(dx_ * dx_ + dy_ * dy_);
}

double Vector2::distanceSq (const Vector2& source_)
{
	double dx_ = source_.x - x;
	double dy_ = source_.y - y;

	return dx_ * dx_ + dy_ * dy_;
}

double Vector2::length ()
{
	return std::sqrt(x * x + y * y);
}

Vector2& Vector2::setLength (double length_)
{
	return normalize().scale(length_);
}

double Vector2::lengthSq ()
{
	return x * x + y * y;
}

Vector2& Vector2::normalize ()
{
	double len_ = x * x + y * y;

	if (len_ > 0)
	{
		len_ = 1 / std::sqrt(len_);

		x = x * len_;
		y = y * len_;
	}

	return *this;
}

Vector2& Vector2::normalizeRightHand ()
{
	double x_ = x;

	x = -y;
	y = x_;

	return *this;
}

Vector2& Vector2::normalizeLeftHand ()
{
	double x_ = x;

	x = y;
	y = -x_;

	return *this;
}

double Vector2::dot (const Vector2& source_)
{
	return x * source_.x + y * source_.y;
}

double Vector2::cross (const Vector2& source_)
{
	return x * source_.x + y * source_.y;
}

Vector2& Vector2::lerp (const Vector2& source_, double t_)
{
	double ax_ = x;
	double ay_ = y;

	x = ax_ + t_ * (source_.x - ax_);
	y = ay_ + t_ * (source_.y - ay_);

	return *this;
}

Vector2& Vector2::reset ()
{
	x = 0.0;
	y = 0.0;

	return *this;
}

Vector2& Vector2::limit (double max_)
{
	double len_ = length();

	if (len_ && len_ > max_)
		scale(max_ / len_);

	return *this;
}

Vector2& Vector2::reflect (const Vector2& normal_)
{
	Vector2 vec_ = normal_;

	vec_.normalize();

	return subtract(vec_.scale(2.0 * dot(vec_)));
}

Vector2& Vector2::mirror (const Vector2& axis_)
{
	return reflect(axis_).negate();
}

Vector2& Vector2::rotate (double delta_)
{
	double cos_ = std::cos(delta_);
	double sin_ = std::sin(delta_);

	return set(cos_ * x - sin_ * y, sin_ * x + cos_ * y);
}

}	// namespace Math
}	// namespace Zen
