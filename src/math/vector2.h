/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_VECTOR_2_H
#define ZEN_MATH_VECTOR_2_H

#include <cmath>
#include "const.h"

namespace Zen {
namespace Math {

class Vector2
{
public:
	Vector2 (double v_ = 0);

	Vector2 (double x_, double y_);

	double x = 0;

	double y = 0;

	Vector2& set (double x_);
	Vector2& set (double x_, double y_);

	Vector2& setTo (double x_);
	Vector2& setTo (double x_, double y_);

	Vector2& setToPolar (double azimuth_, double radius_ = 1.0);

	bool operator == (const Vector2& other_);

	bool equals (const Vector2& other_);

	bool fuzzyEquals (const Vector2& other_, double epsilon_ = EPSILON);

	double angle ();

	Vector2& setAngle (double angle_);

	Vector2& add (const Vector2& source_);

	Vector2& subtract (const Vector2& source_);

	Vector2& multiply (const Vector2& source_);

	Vector2& scale (double value_);

	Vector2& divide (const Vector2& source_);

	Vector2& negate ();

	double distance (const Vector2& source_);

	double distanceSq (const Vector2& source_);

	double length ();

	Vector2& setLength (double length_);

	double lengthSq ();

	Vector2& normalize ();

	Vector2& normalizeRightHand ();

	Vector2& normalizeLeftHand ();

	double dot (const Vector2& other_);

	double cross (const Vector2& other_);

	Vector2& lerp (const Vector2& other_, double t_ = 0.0);

	/*
	 * TODO
	Vector2& transformMat3 (Matrix3 mat);

	Vector2& transformMat4 (Matrix4 mat);
	*/

	Vector2& reset ();

	Vector2& limit (double max_);

	Vector2& reflect (const Vector2& normal_);

	Vector2& mirror (const Vector2& axis_);

	Vector2& rotate (double delta_);
};

}	// namespace Math
}	// namespace Zen

#endif
