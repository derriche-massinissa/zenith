/**
 * @file
 * @author		__AUTHOR_NAME_ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD_
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "../transform_matrix.hpp"

#include <cmath>
#include "../../math/const.hpp"
#include "../../utils/assert.hpp"
#include "../../components/container_item.hpp"

namespace Zen {

extern entt::registry g_registry;

double GetTx (Components::TransformMatrix matrix)
{
	return matrix.e;
}

void SetTx (Components::TransformMatrix *matrix, double value)
{
	matrix->e = value;
}

double GetTy (Components::TransformMatrix matrix)
{
	return matrix.f;
}

void SetTy (Components::TransformMatrix *matrix, double value)
{
	matrix->f = value;
}

double GetRotation (Components::TransformMatrix matrix)
{
	int direction = 0;

	if (std::atan(-matrix.c / matrix.a) < 0)
		direction = -1;
	else
		direction = 1;

	return std::acos(matrix.a / GetScaleX(matrix)) * direction;
}

double GetRotationNormalized (Components::TransformMatrix matrix)
{
	double a = matrix.a;
	double b = matrix.b;
	double c = matrix.c;
	double d = matrix.d;

	if (a || b)
	{
		return (b > 0) ? std::acos(a / GetScaleX(matrix)) : -std::acos(a / GetScaleX(matrix));
	}
	else if (c || d)
	{
		return Math::TAU - ((d > 0) ? std::acos(-c / GetScaleY(matrix)) : -std::acos(c / GetScaleY(matrix)));
	}
	else
	{
		return 0;
	}
}

double GetScaleX (Components::TransformMatrix matrix)
{
	return std::sqrt((matrix.a * matrix.a) + (matrix.b * matrix.b));
}

double GetScaleY (Components::TransformMatrix matrix)
{
	return std::sqrt((matrix.c * matrix.c) + (matrix.d * matrix.d));
}

void LoadIdentity (Components::TransformMatrix *matrix)
{
	matrix->a = 1;
	matrix->b = 0;
	matrix->c = 0;
	matrix->d = 1;
	matrix->e = 0;
	matrix->f = 0;
}

void Translate (Components::TransformMatrix *matrix, double x, double y)
{
	matrix->e = matrix->a * x + matrix->c * y + matrix->e;
	matrix->f = matrix->b * x + matrix->d * y + matrix->f;
}

void Scale (Components::TransformMatrix *matrix, double x, double y)
{
	matrix->a *= x;
	matrix->b *= x;
	matrix->c *= y;
	matrix->d *= y;
}

void Rotate (Components::TransformMatrix *matrix, double angle)
{
	double sin = std::sin(angle);
	double cos = std::cos(angle);

	double a = matrix->a;
	double b = matrix->b;
	double c = matrix->c;
	double d = matrix->d;

	matrix->a = a * cos + c * sin;
	matrix->b = b * cos + d * sin;
	matrix->c = a * -sin + c * cos;
	matrix->d = b * -sin + d * cos;
}

void Multiply (Components::TransformMatrix *matrix, Components::TransformMatrix source)
{
	double localA = matrix->a;
	double localB = matrix->b;
	double localC = matrix->c;
	double localD = matrix->d;
	double localE = matrix->e;
	double localF = matrix->f;

	matrix->a = (source.a * localA) + (source.b * localC);
	matrix->b = (source.a * localB) + (source.b * localD);
	matrix->c = (source.c * localA) + (source.d * localC);
	matrix->d = (source.c * localB) + (source.d * localD);
	matrix->e = (source.e * localA) + (source.f * localC) + localE;
	matrix->f = (source.e * localB) + (source.f * localD) + localF;
}

void MultiplyWithOffset (Components::TransformMatrix *matrix, Components::TransformMatrix source, double offsetX, double offsetY)
{
	double a0 = matrix->a;
	double b0 = matrix->b;
	double c0 = matrix->c;
	double d0 = matrix->d;
	double tx0 = matrix->e;
	double ty0 = matrix->f;

	double pse = offsetX * a0 + offsetY * c0 + tx0;
	double psf = offsetX * b0 + offsetY * d0 + ty0;

	double a1 = source.a;
	double b1 = source.b;
	double c1 = source.c;
	double d1 = source.d;
	double tx1 = source.e;
	double ty1 = source.f;

	matrix->a = a1 * a0 + b1 * c0;
	matrix->b = a1 * b0 + b1 * d0;
	matrix->c = c1 * a0 + d1 * c0;
	matrix->d = c1 * b0 + d1 * d0;
	matrix->e = tx1 * a0 + ty1 * c0 + pse;
	matrix->f = tx1 * b0 + ty1 * d0 + psf;
}

void Transform (Components::TransformMatrix *matrix, double a, double b, double c, double d, double tx, double ty)
{
	double a0 = matrix->a;
	double b0 = matrix->b;
	double c0 = matrix->c;
	double d0 = matrix->d;
	double tx0 = matrix->e;
	double ty0 = matrix->f;

	matrix->a = a * a0 + b * c0;
	matrix->b = a * b0 + b * d0;
	matrix->c = c * a0 + d * c0;
	matrix->d = c * b0 + d * d0;
	matrix->e = tx * a0 + ty * c0 + tx0;
	matrix->f = tx * b0 + ty * d0 + ty0;
}

Math::Vector2 TransformPoint (Components::TransformMatrix matrix, double x, double y)
{
	double a = matrix.a;
	double b = matrix.b;
	double c = matrix.c;
	double d = matrix.d;
	double tx = matrix.e;
	double ty = matrix.f;

	return {
		x * a + y * c + tx,
		x * b + y * d + ty
	};
}

void Invert (Components::TransformMatrix *matrix)
{
	double a = matrix->a;
	double b = matrix->b;
	double c = matrix->c;
	double d = matrix->d;
	double tx = matrix->e;
	double ty = matrix->f;

	double n = a * d - b * c;

	matrix->a = d / n;
	matrix->b = -b / n;
	matrix->c = -c / n;
	matrix->d = a / n;
	matrix->e = (c * ty - d * tx) / n;
	matrix->f = -(a * ty - b * tx) / n;
}

DecomposedMatrix DecomposeMatrix (Components::TransformMatrix matrix)
{
	DecomposedMatrix decomposedMatrix;

	double a = matrix.a;
	double b = matrix.b;
	double c = matrix.c;
	double d = matrix.d;

	double determinant = a * d - b * c;

	decomposedMatrix.translateX = matrix.e;
	decomposedMatrix.translateY = matrix.f;

	if (a || b)
	{
		double r = std::sqrt(a * a + b * b);

		decomposedMatrix.rotation = (b > 0) ? std::acos(a / r) : -std::acos(a / r);
		decomposedMatrix.scaleX = r;
		decomposedMatrix.scaleY = determinant / r;
	}
	else if (c || d)
	{
		double s = std::sqrt(c * c + d * d);

		decomposedMatrix.rotation = M_PI * 0.5 - (d > 0 ? std::acos(-c / s) : -std::acos(c / s));
		decomposedMatrix.scaleX = determinant / s;
		decomposedMatrix.scaleY = s;
	}
	else
	{
		decomposedMatrix.rotation = 0;
		decomposedMatrix.scaleX = 0;
		decomposedMatrix.scaleY = 0;
	}

	return decomposedMatrix;
}

void ApplyITRS (Components::TransformMatrix *matrix, double x, double y, double rotation, double scaleX, double scaleY)
{
	double radianSin = std::sin(rotation);
	double radianCos = std::cos(rotation);

	// Translate
	matrix->e = x;
	matrix->f = y;

	// Rotate and Scale
	matrix->a = radianCos * scaleX;
	matrix->b = radianSin * scaleX;
	matrix->c = -radianSin * scaleY;
	matrix->d = radianCos * scaleY;
}

Math::Vector2 ApplyInverse (Components::TransformMatrix matrix, double x, double y)
{
	Math::Vector2 output;

	double a = matrix.a;
	double b = matrix.b;
	double c = matrix.c;
	double d = matrix.d;
	double tx = matrix.e;
	double ty = matrix.f;

	double id = 1. / ((a * d) + (c * -b));

	output.x = (d * id * x) + (-c * id * y) + (((ty * c) - (tx * d)) * id);
	output.y = (a * id * y) + (-b * id * x) + (((-ty * a) + (tx * b)) * id);

	return output;
}

double GetX (Components::TransformMatrix matrix, double x, double y)
{
	return x * matrix.a + y * matrix.c + matrix.e;
}

double GetY (Components::TransformMatrix matrix, double x, double y)
{
	return x * matrix.b + y * matrix.d + matrix.f;
}

int GetXRound (Components::TransformMatrix matrix, double x, double y, bool round)
{
	double v = GetX(matrix, x, y);

	if (round)
		v = std::round(v);

	return v;
}

int GetYRound (Components::TransformMatrix matrix, double x, double y, bool round)
{
	double v = GetY(matrix, x, y);

	if (round)
		v = std::round(v);

	return v;
}

Components::TransformMatrix GetTransformMatrix (Entity entity)
{
	auto matrix = g_registry.try_get<Components::TransformMatrix>(entity);
	ZEN_ASSERT(matrix, "The entity has no 'TransformMatrix' component.");

	return *matrix;
}

Components::TransformMatrix* GetParentTransformMatrix (Entity entity)
{
	auto item = g_registry.try_get<Components::ContainerItem>(entity);
	//ZEN_ASSERT(item, "The entity has no 'Item' component.");

	if (!item)
		return nullptr;

	auto matrix = g_registry.try_get<Components::TransformMatrix>(item->parent);

	return matrix;
}

}	// namespace Zen
