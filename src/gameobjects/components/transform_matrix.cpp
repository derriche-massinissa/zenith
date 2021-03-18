/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "transform_matrix.h"

namespace Zen {
namespace GameObjects {
namespace Components {

TransformMatrix::TransformMatrix (double a_, double b_, double c_, double d_, double tx_, double ty_)
{
	matrix.reserve(6);

	setTransform (a_, b_, c_, d_, tx_, ty_);
}

double TransformMatrix::getA ()
{
	return matrix[0];
}

TransformMatrix& TransformMatrix::setA (double value_)
{
	matrix[0] = value_;

	return *this;
}

double TransformMatrix::getB ()
{
	return matrix[1];
}

TransformMatrix& TransformMatrix::setB (double value_)
{
	matrix[1] = value_;

	return *this;
}

double TransformMatrix::getC ()
{
	return matrix[2];
}

TransformMatrix& TransformMatrix::setC (double value_)
{
	matrix[2] = value_;

	return *this;
}

double TransformMatrix::getD ()
{
	return matrix[3];
}

TransformMatrix& TransformMatrix::setD (double value_)
{
	matrix[3] = value_;

	return *this;
}

double TransformMatrix::getE ()
{
	return matrix[4];
}

TransformMatrix& TransformMatrix::setE (double value_)
{
	matrix[4] = value_;

	return *this;
}

double TransformMatrix::getF ()
{
	return matrix[5];
}

TransformMatrix& TransformMatrix::setF (double value_)
{
	matrix[5] = value_;

	return *this;
}

double TransformMatrix::getTx ()
{
	return matrix[4];
}

TransformMatrix& TransformMatrix::setTx (double value_)
{
	matrix[4] = value_;

	return *this;
}

double TransformMatrix::getTy ()
{
	return matrix[5];
}

TransformMatrix& TransformMatrix::setTy (double value_)
{
	matrix[5] = value_;

	return *this;
}

double TransformMatrix::getRotation ()
{
	return std::acos(getA() / getScaleX()) * ((std::atan(-getC() / getA()) < 0) ? -1 : 1);
}

double TransformMatrix::getRotationNormalized ()
{
	double a_ = matrix[0];
	double b_ = matrix[1];
	double c_ = matrix[2];
	double d_ = matrix[3];

	if (a_ || b_) {
		return (b_ > 0) ? std::acos(a_ / getScaleX()) : -std::acos(a_ / getScaleX());
	} else if (c_ || d_) {
		return Math::TAU - ((d_ > 0) ? std::acos(-c_ / getScaleY()) : -std::acos(c_ / getScaleY()));
	} else {
		return 0;
	}
}

double TransformMatrix::getScaleX ()
{
	return std::sqrt((getA() * getA()) + (getB() * getB()));
}

double TransformMatrix::getScaleY ()
{
	return std::sqrt((getC() * getC()) + (getD() * getD()));
}

TransformMatrix& TransformMatrix::loadIdentity ()
{
	matrix[0] = 1;
	matrix[1] = 0;
	matrix[2] = 0;
	matrix[3] = 1;
	matrix[4] = 0;
	matrix[5] = 0;

	return *this;
}

TransformMatrix& TransformMatrix::translate (double x_, double y_)
{
	matrix[4] = matrix[0] * x_ + matrix[2] * y_ + matrix[4];
	matrix[5] = matrix[1] * x_ + matrix[3] * y_ + matrix[5];

	return *this;
}

TransformMatrix& TransformMatrix::scale (double x_, double y_)
{
	matrix[0] *= x_;
	matrix[1] *= x_;
	matrix[2] *= y_;
	matrix[3] *= y_;

	return *this;
}

TransformMatrix& TransformMatrix::rotate (double angle_)
{
	double sin_ = std::sin(angle_);
	double cos_ = std::cos(angle_);

	double a_ = matrix[0];
	double b_ = matrix[1];
	double c_ = matrix[2];
	double d_ = matrix[3];

	matrix[0] = a_ * cos_ + c_ * sin_;
	matrix[1] = b_ * cos_ + d_ * sin_;
	matrix[2] = a_ * -sin_ + c_ * cos_;
	matrix[3] = b_ * -sin_ + d_ * cos_;

	return *this;
}

TransformMatrix& TransformMatrix::multiply (TransformMatrix rhs_)
{
	auto source_ = rhs_.getVector();

	double localA_ = matrix[0];
	double localB_ = matrix[1];
	double localC_ = matrix[2];
	double localD_ = matrix[3];
	double localE_ = matrix[4];
	double localF_ = matrix[5];

	double sourceA_ = source_[0];
	double sourceB_ = source_[1];
	double sourceC_ = source_[2];
	double sourceD_ = source_[3];
	double sourceE_ = source_[4];
	double sourceF_ = source_[5];

	setA( (sourceA_ * localA_) + (sourceB_ * localC_) );
	setB( (sourceA_ * localB_) + (sourceB_ * localD_) );
	setC( (sourceC_ * localA_) + (sourceD_ * localC_) );
	setD( (sourceC_ * localB_) + (sourceD_ * localD_) );
	setE( (sourceE_ * localA_) + (sourceF_ * localC_) + localE_ );
	setF( (sourceE_ * localB_) + (sourceF_ * localD_) + localF_ );

	return *this;
}

TransformMatrix& TransformMatrix::multiplyWithOffset (TransformMatrix src_, double offsetX_, double offsetY_)
{
	auto otherMatrix_ = src_.getVector();

	double a0_ = matrix[0];
	double b0_ = matrix[1];
	double c0_ = matrix[2];
	double d0_ = matrix[3];
	double tx0_ = matrix[4];
	double ty0_ = matrix[5];

	double pse_ = offsetX_ * a0_ + offsetY_ * c0_ + tx0_;
	double psf_ = offsetX_ * b0_ + offsetY_ * d0_ + ty0_;

	double a1_ = otherMatrix_[0];
	double b1_ = otherMatrix_[1];
	double c1_ = otherMatrix_[2];
	double d1_ = otherMatrix_[3];
	double tx1_ = otherMatrix_[4];
	double ty1_ = otherMatrix_[5];

	setA( a1_ * a0_ + b1_ * c0_);
	setB( a1_ * b0_ + b1_ * d0_);
	setC( c1_ * a0_ + d1_ * c0_);
	setD( c1_ * b0_ + d1_ * d0_);
	setE( tx1_ * a0_ + ty1_ * c0_ + pse_);
	setF( tx1_ * b0_ + ty1_ * d0_ + psf_);

	return *this;
}

TransformMatrix& TransformMatrix::transform (double a_, double b_, double c_, double d_, double tx_, double ty_)
{
	double a0_ = matrix[0];
	double b0_ = matrix[1];
	double c0_ = matrix[2];
	double d0_ = matrix[3];
	double tx0_ = matrix[4];
	double ty0_ = matrix[5];

	setA( a_ * a0_ + b_ * c0_);
	setB( a_ * b0_ + b_ * d0_);
	setC( c_ * a0_ + d_ * c0_);
	setD( c_ * b0_ + d_ * d0_);
	setE( tx_ * a0_ + ty_ * c0_ + tx0_);
	setF( tx_ * b0_ + ty_ * d0_ + ty0_);

	return *this;
}

Math::Vector2 TransformMatrix::transformPoint (double x_, double y_)
{
	Math::Vector2 point_ (x_, y_);

	double a_ = matrix[0];
	double b_ = matrix[1];
	double c_ = matrix[2];
	double d_ = matrix[3];
	double tx_ = matrix[4];
	double ty_ = matrix[5];

	point_.x = x_ * a_ + y_ * c_ + tx_;
	point_.y = x_ * b_ + y_ * d_ + ty_;

	return point_;
}

TransformMatrix& TransformMatrix::invert ()
{
	double a_ = matrix[0];
	double b_ = matrix[1];
	double c_ = matrix[2];
	double d_ = matrix[3];
	double tx_ = matrix[4];
	double ty_ = matrix[5];

	double n_ = a_ * d_ - b_ * c_;

	setA( d_ / n_);
	setB( -b_ / n_);
	setC( -c_ / n_);
	setD( a_ / n_);
	setE( (c_ * ty_ - d_ * tx_) / n_);
	setF( -(a_ * ty_ - b_ * tx_) / n_);

	return *this;
}

TransformMatrix& TransformMatrix::copyFrom (TransformMatrix src_)
{
	setA( src_.getA() );
	setB( src_.getB() );
	setC( src_.getC() );
	setD( src_.getD() );
	setE( src_.getE() );
	setF( src_.getF() );

	return *this;
}

TransformMatrix& TransformMatrix::copyFromArray (double src_[])
{
	setA( src_[0] );
	setB( src_[1] );
	setC( src_[2] );
	setD( src_[3] );
	setE( src_[4] );
	setF( src_[5] );

	return *this;
}

TransformMatrix& TransformMatrix::copyFromVector (std::vector<double> src_)
{
	setA( src_.at(0) );
	setB( src_.at(1) );
	setC( src_.at(2) );
	setD( src_.at(3) );
	setE( src_.at(4) );
	setF( src_.at(5) );

	return *this;
}

std::vector<double> TransformMatrix::getVector ()
{
	return matrix;
}

TransformMatrix& TransformMatrix::setTransform (double a_, double b_, double c_, double d_, double tx_, double ty_)
{
	setA( a_ );
	setB( b_ );
	setC( c_ );
	setD( d_ );
	setTx( tx_ );
	setTy( ty_ );

	return *this;
}

DecomposedMatrix TransformMatrix::decomposeMatrix ()
{
	DecomposedMatrix decomposedMatrix_;

	double a_ = getA();
	double b_ = getB();
	double c_ = getC();
	double d_ = getD();

	double determinant_ = a_ * d_ - b_ * c_;

	decomposedMatrix_.translateX = getE();
	decomposedMatrix_.translateY = getF();

	if (a_ || b_) {
		double r_ = std::sqrt(a_ * a_ + b_ * b_);

		decomposedMatrix_.rotation = (b_ > 0) ? std::acos(a_ / r_) : -std::acos(a_ / r_);
		decomposedMatrix_.scaleX = r_;
		decomposedMatrix_.scaleY = determinant_ / r_;
	} else if (c_ || d_) {
		double s_ = std::sqrt(c_ * c_ + d_ * d_);

		decomposedMatrix_.rotation = M_PI * 0.5 - (d_ > 0 ? std::acos(-c_ / s_) : -std::acos(c_ / s_));
		decomposedMatrix_.scaleX = determinant_ / s_;
		decomposedMatrix_.scaleY = s_;
	} else {
		decomposedMatrix_.rotation = 0;
		decomposedMatrix_.scaleX = 0;
		decomposedMatrix_.scaleY = 0;
	}

	return decomposedMatrix_;
}

TransformMatrix& TransformMatrix::applyITRS (double x_, double y_, double rotation_, double scaleX_, double scaleY_)
{
        double radianSin_ = std::sin(rotation_);
        double radianCos_ = std::cos(rotation_);

        // Translate
        setTx( x_ );
        setTy( y_ );

        // Rotate and Scale
        setA( radianCos_ * scaleX_ );
        setB( radianSin_ * scaleX_ );
        setC( -radianSin_ * scaleY_ );
        setD( radianCos_ * scaleY_ );

        return *this;
}

Math::Vector2 TransformMatrix::applyInverse (double x_, double y_)
{
	Math::Vector2 output_;

	double a_ = matrix[0];
	double b_ = matrix[1];
	double c_ = matrix[2];
	double d_ = matrix[3];
	double tx_ = matrix[4];
	double ty_ = matrix[5];

	double id_ = 1 / ((a_ * d_) + (c_ * -b_));

	output_.x = (d_ * id_ * x_) + (-c_ * id_ * y_) + (((ty_ * c_) - (tx_ * d_)) * id_);
	output_.y = (a_ * id_ * y_) + (-b_ * id_ * x_) + (((-ty_ * a_) + (tx_ * b_)) * id_);

	return output_;
}

double TransformMatrix::getX (double x_, double y_)
{
	return x_ * getA() + y_ * getC() + getE();
}

double TransformMatrix::getY (double x_, double y_)
{
	return x_ * getB() + y_ * getD() + getF();
}

int TransformMatrix::getXRound (double x_, double y_, bool round_)
{
	double v_ = getX(x_, y_);

	if (round_)
		v_ = std::round(v_);

	return v_;
}

int TransformMatrix::getYRound (double x_, double y_, bool round_)
{
	double v_ = getY(x_, y_);

	if (round_)
		v_ = std::round(v_);

	return v_;
}

}	// namespace Components
}	// namespace GameObjects
}	// namespace Zen
