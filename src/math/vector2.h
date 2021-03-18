/**
 * @file		vector2.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef VECTOR_2_H
#define VECTOR_2_H

#include <math.h>

namespace Zen {
namespace Math {

class Vector2
{
public:
	Vector2 (double x_, double y_);

	double x = 0;

	double y = 0;

	Vector2& set (double x_);
	Vector2& set (double x_, double y_);

	Vector2& setTo (double x_);
	Vector2& setTo (double x_, double y_);

	Vector2& setToPolar (double azimuth, double radius = 1.0);

	bool operator == (const Vector2& other);

	bool equals (const Vector2& other);

	bool fuzzyEquals (const Vector2& other, double epsilon = Math::EPSILON);

	double angle ();

	Vector2& setAngle (double angle);

	Vector2& add (const Vector2& source);

	Vector2& subtract (const Vector2& source);

	Vector2& multiply (const Vector2& source);

	Vector2& scale (double value);

	Vector2& divide (const Vector2& source);

	Vector2& negate ();

	double distance (const Vector2& source);

	double distanceSq (const Vector2& source);

	double length ();

	Vector2& setLength (double length);

	double lengthSq ();

	Vector2& normalize ();

	Vector2& normalizeRightHand ();

	Vector2& normalizeLeftHand ();

	double dot (const Vector2& other);

	double cross (const Vector2& other);

	Vector2& lerp (const Vector2& other, double t = 0.0);

	/*
	 * TODO
	Vector2& transformMat3 (Matrix3 mat);

	Vector2& transformMat4 (Matrix4 mat);
	*/

	Vector2& reset ();

	Vector2& limit (double max);

	Vector2& reflect (const Vector2& normal);

	Vector2& mirror (const Vector2& axis);

	Vector2& rotate (double delta);
};

}	// namespace Math
}	// namespace Zen

#endif
