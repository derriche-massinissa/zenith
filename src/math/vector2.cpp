/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "vector2.hpp"

#include <cmath>
#include "fuzzy/fuzzy_equal.hpp"

namespace Zen {
namespace Math {

void SetToPolar (Vector2 *vector, double azimuth, double radius)
{
	vector->x = std::cos(azimuth) * radius;
	vector->y = std::sin(azimuth) * radius;
}

bool Equals (Vector2 lhs, Vector2 rhs)
{
	return ((lhs.x == rhs.x) && (rhs.y == rhs.y));
}

bool FuzzyEquals (Vector2 lhs, Vector2 rhs, double epsilon)
{
	return (Math::FuzzyEqual(lhs.x, rhs.x, epsilon) &&
			Math::FuzzyEqual(lhs.y, rhs.y, epsilon));
}

double Angle (Vector2 vector)
{
	// Computes the angle in radians with respect to the positive x-axis

	double angle = std::atan2(vector.y, vector.x);

	if (angle < 0)
		angle += 2 * M_PI;

	return angle;
}

void SetAngle (Vector2 *vector, double angle)
{
	SetToPolar(vector, angle, Length(*vector));
}

void Add (Vector2 *vector, Vector2 source)
{
	vector->x += source.x;
	vector->y += source.y;
}

void Subtract (Vector2 *vector, Vector2 source)
{
	vector->x -= source.x;
	vector->y -= source.y;
}

void Multiply (Vector2 *vector, Vector2 source)
{
	vector->x *= source.x;
	vector->y *= source.y;
}

void Scale (Vector2 *vector, double value)
{
	vector->x *= value;
	vector->y *= value;
}

void Divide (Vector2 *vector, Vector2 source)
{
	vector->x /= source.x;
	vector->y /= source.y;
}

void Negate (Vector2 *vector)
{
	vector->x = -vector->x;
	vector->y = -vector->y;
}

double Distance (Vector2 vector, Vector2 source)
{
	double dx = source.x - vector.x;
	double dy = source.y - vector.y;

	return std::sqrt(dx * dx + dy * dy);
}

double DistanceSq (Vector2 vector, Vector2 source)
{
	double dx = source.x - vector.x;
	double dy = source.y - vector.y;

	return dx * dx + dy * dy;
}

double Length (Vector2 vector)
{
	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

void SetLength (Vector2 *vector, double length)
{
	Normalize(vector);

	Scale(vector, length);
}

double LengthSq (Vector2 vector)
{
	return vector.x * vector.x + vector.y * vector.y;
}

void Normalize (Vector2 *vector)
{
	double len = vector->x * vector->x + vector->y * vector->y;

	if (len > 0)
	{
		len = 1. / std::sqrt(len);

		vector->x = vector->x * len;
		vector->y = vector->y * len;
	}
}

void NormalizeRightHand (Vector2 *vector)
{
	double x = vector->x;

	vector->x = -vector->y;
	vector->y = x;
}

void NormalizeLeftHand (Vector2 *vector)
{
	double x = vector->x;

	vector->x = vector->y;
	vector->y = -x;
}

double Dot (Vector2 vector, Vector2 source)
{
	return vector.x * source.x + vector.y * source.y;
}

double Cross (Vector2 vector, Vector2 source)
{
	return vector.x * source.x + vector.y * source.y;
}

void Lerp (Vector2 *vector, Vector2 source, double t)
{
	double ax = vector->x;
	double ay = vector->y;

	vector->x = ax + t * (source.x - ax);
	vector->y = ay + t * (source.y - ay);
}

void Limit (Vector2 *vector, double max)
{
	double len = Length(*vector);

	if (len && len > max)
		Scale(vector, max / len);
}

void Reflect (Vector2 *vector, Vector2 normal)
{
	Normalize(&normal);

	Scale(&normal, 2. * Dot(*vector, normal));

	Subtract(vector, normal);
}

void Mirror (Vector2 *vector, Vector2 axis)
{
	Reflect(vector, axis);

	Negate(vector);
}

void Rotate (Vector2 *vector, double delta)
{
	double cos = std::cos(delta);
	double sin = std::sin(delta);

	vector->x = cos * vector->x - sin * vector->y;
	vector->y = sin * vector->x + cos * vector->y;
}

}	// namespace Math
}	// namespace Zen
