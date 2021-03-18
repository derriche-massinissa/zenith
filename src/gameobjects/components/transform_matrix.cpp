/**
 * @file		transform_matrix.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "transform_matrix.h"

namespace Zen {
namespace GameObjects {
namespace Components {

TransformMatrix::TransformMatrix (double a, double b, double c, double d, double tx, double ty)
	: matrix_ (a, b, c, d, tx, ty, 0, 0, 1)
{}

double getA ()
{
	return matrix_[0];
}

TransformMatrix& setA (double value)
{
	matrix_[0] = value;

	return *this;
}

double getB ()
{
	return matrix_[1];
}

TransformMatrix& setB (double value)
{
	matrix_[1] = value;

	return *this;
}

double getC ()
{
	return matrix_[2];
}

TransformMatrix& setC (double value)
{
	matrix_[2] = value;

	return *this;
}

double getD ()
{
	return matrix_[3];
}

TransformMatrix& setD (double value)
{
	matrix_[3] = value;

	return *this;
}

double getE ()
{
	return matrix_[4];
}

TransformMatrix& setE (double value)
{
	matrix_[4] = value;

	return *this;
}

double getF ()
{
	return matrix_[5];
}

TransformMatrix& setF (double value)
{
	matrix_[5] = value;

	return *this;
}

double getTx ()
{
	return matrix_[4];
}

TransformMatrix& setTx (double value)
{
	matrix_[4] = value;

	return *this;
}

double getTy ()
{
	return matrix_[5];
}

TransformMatrix& setTy (double value)
{
	matrix_[5] = value;

	return *this;
}

double getRotation ()
{
	return std::acos(getA() / getScaleX()) * ((std::atan(-getC() / getA()) < 0) ? -1 : 1);
}

double getRotationNormalized ()
{
	double a = matrix_[0];
	double b = matrix_[1];
	double c = matrix_[2];
	double d = matrix_[3];

	if (a || b) {
		return (b > 0) ? std::acos(a / getScaleX()) : -std::acos(a / getScaleX());
	} else if (c || d) {
		return Math::TAU - ((d > 0) ? std::acos(-c / getScaleY()) : -std::acos(c / getScaleY()));
	} else {
		return 0;
	}
}

double getScaleX ()
{
	return std::sqrt((getA() * getA()) + (getB() * getB()));
}

double getScaleY ()
{
	return std::sqrt((getC() * getC()) + (getD() * getD()));
}

TransformMatrix& loadIdentity ()
{
	matrix_[0] = 1;
	matrix_[1] = 0;
	matrix_[2] = 0;
	matrix_[3] = 1;
	matrix_[4] = 0;
	matrix_[5] = 0;

	return *this;
}

TransformMatrix& translate (double x, double y)
{
	matrix_[4] = matrix_[0] * x + matrix_[2] * y + matrix_[4];
	matrix_[5] = matrix_[1] * x + matrix_[3] * y + matrix_[5];

	return *this;
}

TransformMatrix& scale (double x, double y)
{
	matrix_[0] *= x;
	matrix_[1] *= x;
	matrix_[2] *= y;
	matrix_[3] *= y;

	return *this;
}

TransformMatrix& rotate (double angle)
{
	double sin = std::sin(angle);
	double cos = std::cos(angle);

	double a = matrix_[0];
	double b = matrix_[1];
	double c = matrix_[2];
	double d = matrix_[3];

	matrix_[0] = a * cos + c * sin;
	matrix_[1] = b * cos + d * sin;
	matrix_[2] = a * -sin + c * cos;
	matrix_[3] = b * -sin + d * cos;

	return *this;
}

TransformMatrix& multiply (TransformMatrix rhs)
{
	auto source = rhs.getVector();

	double localA = matrix_[0];
	double localB = matrix_[1];
	double localC = matrix_[2];
	double localD = matrix_[3];
	double localE = matrix_[4];
	double localF = matrix_[5];

	double sourceA = source[0];
	double sourceB = source[1];
	double sourceC = source[2];
	double sourceD = source[3];
	double sourceE = source[4];
	double sourceF = source[5];

	setA( (sourceA * localA) + (sourceB * localC) );
	setB( (sourceA * localB) + (sourceB * localD) );
	setC( (sourceC * localA) + (sourceD * localC) );
	setD( (sourceC * localB) + (sourceD * localD) );
	setE( (sourceE * localA) + (sourceF * localC) + localE );
	setF( (sourceE * localB) + (sourceF * localD) + localF );

	return *this;
}

TransformMatrix& multiplyWithOffset (TransformMatrix src, double offsetX, double offsetY)
{
	auto otherMatrix = src.getVector();

	double a0 = matrix_[0];
	double b0 = matrix_[1];
	double c0 = matrix_[2];
	double d0 = matrix_[3];
	double tx0 = matrix_[4];
	double ty0 = matrix_[5];

	double pse = offsetX * a0 + offsetY * c0 + tx0;
	double psf = offsetX * b0 + offsetY * d0 + ty0;

	double a1 = otherMatrix[0];
	double b1 = otherMatrix[1];
	double c1 = otherMatrix[2];
	double d1 = otherMatrix[3];
	double tx1 = otherMatrix[4];
	double ty1 = otherMatrix[5];

	setA( a1 * a0 + b1 * c0);
	setB( a1 * b0 + b1 * d0);
	setC( c1 * a0 + d1 * c0);
	setD( c1 * b0 + d1 * d0);
	setE( tx1 * a0 + ty1 * c0 + pse);
	setF( tx1 * b0 + ty1 * d0 + psf);

	return *this;
}

TransformMatrix& transform (double a, double b, double c, double d, double tx, double ty)
{
	double a0 = matrix_[0];
	double b0 = matrix_[1];
	double c0 = matrix_[2];
	double d0 = matrix_[3];
	double tx0 = matrix_[4];
	double ty0 = matrix_[5];

	setA( a * a0 + b * c0);
	setB( a * b0 + b * d0);
	setC( c * a0 + d * c0);
	setD( c * b0 + d * d0);
	setE( tx * a0 + ty * c0 + tx0);
	setF( tx * b0 + ty * d0 + ty0);

	return *this;
}

Math::Vector2 transformPoint (double x, double y)
{
	Math::Vector2 point (x, y);

	double a = matrix_[0];
	double b = matrix_[1];
	double c = matrix_[2];
	double d = matrix_[3];
	double tx = matrix_[4];
	double ty = matrix_[5];

	point.x = x * a + y * c + tx;
	point.y = x * b + y * d + ty;

	return point;
}

TransformMatrix& invert ()
{
	double a = matrix_[0];
	double b = matrix_[1];
	double c = matrix_[2];
	double d = matrix_[3];
	double tx = matrix_[4];
	double ty = matrix_[5];

	double n = a * d - b * c;

	setA( d / n);
	setB( -b / n);
	setC( -c / n);
	setD( a / n);
	setE( (c * ty - d * tx) / n);
	setF( -(a * ty - b * tx) / n);

	return *this;
}

TransformMatrix& copyFrom (TransformMatrix src)
{
	setA( src.getA() );
	setB( src.getB() );
	setC( src.getC() );
	setD( src.getD() );
	setE( src.getE() );
	setF( src.getF() );

	return *this;
}

TransformMatrix& copyFromArray (double src[])
{
	setA( src[0] );
	setB( src[1] );
	setC( src[2] );
	setD( src[3] );
	setE( src[4] );
	setF( src[5] );

	return *this;
}

TransformMatrix& copyFromVector (std::vector<double> src)
{
	setA( src.get(0) );
	setB( src.get(1) );
	setC( src.get(2) );
	setD( src.get(3) );
	setE( src.get(4) );
	setF( src.get(5) );

	return *this;
}

std::vector<double> getVector ()
{
	return matrix_;
}

TransformMatrix& setTransform (double a, double b, double c, double d, double tx, double ty)
{
	setA( a );
	setB( b );
	setC( c );
	setD( d );
	setTx( tx );
	setTy( ty );

	return *this;
}

DecomposedMatrix decomposeMatrix ()
{
	DecomposedMatrix decomposedMatrix;

	double a = getA();
	double b = getB();
	double c = getC();
	double d = getD();

	double determinant = a * d - b * c;

	decomposedMatrix.translateX = getE();
	decomposedMatrix.translateY = getF();

	if (a || b) {
		double r = std::sqrt(a * a + b * b);

		decomposedMatrix.rotation = (b > 0) ? std::acos(a / r) : -std::acos(a / r);
		decomposedMatrix.scaleX = r;
		decomposedMatrix.scaleY = determinant / r;
	} else if (c || d) {
		double s = std::sqrt(c * c + d * d);

		decomposedMatrix.rotation = M_PI * 0.5 - (d > 0 ? std::acos(-c / s) : -std::acos(c / s));
		decomposedMatrix.scaleX = determinant / s;
		decomposedMatrix.scaleY = s;
	} else {
		decomposedMatrix.rotation = 0;
		decomposedMatrix.scaleX = 0;
		decomposedMatrix.scaleY = 0;
	}

	return decomposeMatrix;
}

TransformMatrix& applyITRS (double x, double y, double rotation, double scaleX, double scaleY)
{
        double radianSin = std::sin(rotation);
        double radianCos = std::cos(rotation);

        // Translate
        setTx( x );
        setTy( y );

        // Rotate and Scale
        setA( radianCos * scaleX );
        setB( radianSin * scaleX );
        setC( -radianSin * scaleY );
        setD( radianCos * scaleY );

        return *this;
}

Math::Vector2 applyInverse (x, y)
{
	Math::Vector2 output;

	double a = matrix_[0];
	double b = matrix_[1];
	double c = matrix_[2];
	double d = matrix_[3];
	double tx = matrix_[4];
	double ty = matrix_[5];

	double id = 1 / ((a * d) + (c * -b));

	output.x = (d * id * x) + (-c * id * y) + (((ty * c) - (tx * d)) * id);
	output.y = (a * id * y) + (-b * id * x) + (((-ty * a) + (tx * b)) * id);

	return output;
}

double getX (double x, double y)
{
	return x * getA() + y * getC() + getE();
}

double getY (double x, double y)
{
	return x * getB() + y * getD() + getF();
}

int getXRound (double x, double y, bool round = false)
{
	double v = getX(x, y);

	if (round)
		v = std::round(v);

	return v;
}

int getYRound (double x, double y, bool round = false)
{
	double v = getY(x, y);

	if (round)
		v = std::round(v);

	return v;
}

}	// namespace Components
}	// namespace GameObjects
}	// namespace Zen
