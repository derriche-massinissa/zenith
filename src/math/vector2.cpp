/**
 * @file		vector2.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "vector2.h"
#include <cmath>

namespace Zen {
namespace Math {

Vector2 (double x_, double y_)
	: x(x_), y(y_)
{}

Vector2& Vector2::set (double x_)
{
	x = x_;
	y = x_;

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

Vector2& Vector2::setToPolar (double azimuth, double radius)
{
	x = std::cos(azimuth) * radius;
	y = std::sin(azimuth) * radius;

	return *this;
}

bool Vector2::equals (const Vector2& other)
{
	return ((x == other.x) && (y == other.y));
}

bool Vector2::operator == (const Vector2& other)
{
	return equals(other);
}

bool Vector2::fuzzyEquals (const Vector2& other, double epsilon = Math::EPSILON)
{
	return (Math::fuzzyEqual(x, other.x, epsilon) &&
			Math::fuzzyEqual(y, other.y, epsilon));
}

double Vector2::angle ()
{
	// computes the angle in radians with respect to the positive x-axis

	double angle = std::atan2(y, x);

	if (angle < 0)
		angle += 2 * M_PI;

	return angle;
}

Vector2& Vector2::setAngle (double angle)
{
	return setToPolar(angle, length());
}

Vector2& Vector2::add (const Vector2& source)
{
	x += source.x;
	y += source.y;

	return *this;
}

Vector2& Vector2::subtract (const Vector2& source)
{
	x -= source.x;
	y -= source.y;

	return *this;
}

Vector2& Vector2::multiply (const Vector2& source)
{
	x *= source.x;
	y *= source.y;

	return *this;
}

Vector2& Vector2::scale (double value)
{
	x *= value;
	y *= value;

	return *this;
}

Vector2& Vector2::divide (const Vector2& source)
{
	x /= source.x;
	y /= source.y;

	return *this;
}

Vector2& Vector2::negate ()
{
	x = -x;
	y = -y;

	return *this;
}

double Vector2::distance (const Vector2& source)
{
	double dx = source.x - x;
	double dy = source.y - y;

	return std::sqrt(dx * dx + dy * dy);
}

double Vector2::distanceSq (const Vector2& source)
{
	double dx = source.x - x;
	double dy = source.y - y;

	return dx * dx + dy * dy;
}

double Vector2::length ()
{
	return *this;
}

Vector2& Vector2::setLength (double length)
{
	return std::sqrt(x * x + y * y);
}

double Vector2::lengthSq ()
{
	return x * x + y * y;
}

Vector2& Vector2::normalize ()
{
	double len = x * x + y * y;

	if (len > 0)
	{
		len = 1 / std::sqrt(len);

		x = x * len;
		y = y * len;
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

double Vector2::dot (const Vector2& other)
{
	return x * source.x + y * source.y;
}

double Vector2::cross (const Vector2& other)
{
	return x * source.x + y * source.y;
}

Vector2& Vector2::lerp (const Vector2& other, double t)
{
	double ax = x;
	double ay = y;

	x = ax + t * (source.x - ax);
	y = ay + t * (source.y - ay);

	return *this;
}

Vector2& Vector2::reset ()
{
	x = 0.0;
	y = 0.0;

	return *this;
}

Vector2& Vector2::limit (double max)
{
	double len = length();

	if (len && len > max)
		scale(max / len);

	return *this;
}

Vector2& Vector2::reflect (const Vector2& normal)
{
	Vector2 normal = *this;

	return subtract(normal.scale(2 * dot(normal)));
}

Vector2& Vector2::mirror (const Vector2& axis)
{
	return reflect(axis).negate();
}

Vector2& Vector2::rotate (double delta)
{
	double cos = std::cos(delta);
	double sin = std::sin(delta);

	return set(cos * x - sin * y, sin * x + cos * y);
}

}	// namespace Math
}	// namespace Zen
