/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_VECTOR_2_HPP
#define ZEN_MATH_VECTOR_2_HPP

#include "const.hpp"
#include "types/vector2.hpp"

namespace Zen {
namespace Math {

void SetToPolar (Vector2 *vector, double azimuth, double radius = 1.0);

bool Equals (Vector2 lhs, Vector2 rhs);

bool FuzzyEquals (Vector2 lhs, Vector2 rhs, double epsilon = EPSILON);

double Angle (Vector2 vector);

void SetAngle (Vector2 *vector, double angle);

void Add (Vector2 *vector, Vector2 source);

void Subtract (Vector2 *vector, Vector2 source);

void Multiply (Vector2 *vector, Vector2 source);

void Scale (Vector2 *vector, double value);

void Divide (Vector2 *vector, Vector2 source);

void Negate (Vector2 *vector);

double Distance (Vector2 vector, Vector2 source);

double DistanceSq (Vector2 vector, Vector2 source);

double Length (Vector2 vector);

void SetLength (Vector2 *vector, double length);

double LengthSq (Vector2 vector);

void Normalize (Vector2 *vector);

void NormalizeRightHand (Vector2 *vector);

void NormalizeLeftHand (Vector2 *vector);

double Dot (Vector2 vector, Vector2 other);

double Cross (Vector2 vector, Vector2 other);

void Lerp (Vector2 *vector, Vector2 other, double t = 0.);

/*
 * TODO
 Vector2& transformMat3 (Matrix3 mat);

 Vector2& transformMat4 (Matrix4 mat);
 */

void Limit (Vector2 *vector, double max);

void Reflect (Vector2 *vector, Vector2 normal);

void Mirror (Vector2 *vector, Vector2 axis);

void Rotate (Vector2 *vector, double delta);

}	// namespace Math
}	// namespace Zen

#endif
