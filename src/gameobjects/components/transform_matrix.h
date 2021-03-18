/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_COMPONENTS_TRANSFORM_MATRIX_H
#define ZEN_GAMEOBJECTS_COMPONENTS_TRANSFORM_MATRIX_H

#include <vector>
#include <cmath>

#include "../../math/vector2.h"

namespace Zen {
namespace GameObjects {
namespace Components {

/**
 * @struct DecomposedMatrix
 * @since 0.0.0
 */
struct DecomposedMatrix
{
	double translateX = 0;
	double translateY = 0;
	double scaleX = 1;
	double scaleY = 1;
	double rotation = 0;
};

/**
 * A Matrix used for display transformations for rendering.
 *
 * It is represented like so:
 *
 * ```
 * | a | c | tx |
 * | b | d | ty |
 * | 0 | 0 | 1  |
 * ```
 *
 * @class TransformMatrix
 * @since 0.0.0
 */
class TransformMatrix
{
public:
	/**
	 * @since 0.0.0
	 *
	 * @param a_ The Scale X value.
	 * @param b_ The Skew Y value.
	 * @param c_ The Skew X value.
	 * @param d_ The Scale Y value.
	 * @param tx_ The Translate X value.
	 * @param ty_ The Translate Y value.
	 */
	TransformMatrix (double a_ = 1.0, double b_ = 0.0, double c_ = 0.0, double d_ = 1.0, double tx_ = 0.0, double ty_ = 0.0);

	/**
	 * The matrix values.
	 *
	 * @since 0.0.0
	 */
	std::vector<double> matrix {};

	/**
	 * Getter for the Scale X value.
	 *
	 * @since 0.0.0
	 *
	 * @return Scale X.
	 */
	double getA ();
	/**
	 * Setter for the Scale X value.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ Scale X.
	 */
	TransformMatrix& setA (double value_);

	/**
	 * Getter for the Skew Y value.
	 *
	 * @since 0.0.0
	 *
	 * @return Skew Y.
	 */
	double getB ();
	/**
	 * Setter for the Skew Y value.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ Skew Y.
	 */
	TransformMatrix& setB (double value_);

	/**
	 * Getter for the Skew X value.
	 *
	 * @since 0.0.0
	 *
	 * @return Skew X.
	 */
	double getC ();
	/**
	 * Setter for the Skew X value.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ Skew X.
	 */
	TransformMatrix& setC (double value_);

	/**
	 * Getter for the Scale Y value.
	 *
	 * @since 0.0.0
	 *
	 * @return Scale Y.
	 */
	double getD ();
	/**
	 * Setter for the Scale Y value.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ Scale Y.
	 */
	TransformMatrix& setD (double value_);

	/**
	 * Getter for the Translate X value.
	 *
	 * @since 0.0.0
	 *
	 * @return Translate X.
	 */
	double getE ();
	/**
	 * Setter for the Translate X value.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ Translate X.
	 */
	TransformMatrix& setE (double value_);

	/**
	 * Getter for the Translate Y value.
	 *
	 * @since 0.0.0
	 *
	 * @return Translate Y.
	 */
	double getF ();
	/**
	 * Setter for the Translate Y value.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ Translate Y.
	 */
	TransformMatrix& setF (double value_);

	/**
	 * Getter for the Translate X value.
	 *
	 * @since 0.0.0
	 *
	 * @return Translate X.
	 */
	double getTx ();
	/**
	 * Setter for the Translate X value.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ Translate X.
	 */
	TransformMatrix& setTx (double value_);

	/**
	 * Getter for the Translate Y value.
	 *
	 * @since 0.0.0
	 *
	 * @return Translate Y.
	 */
	double getTy ();
	/**
	 * Setter for the Translate Y value.
	 *
	 * @since 0.0.0
	 *
	 * @param value_ Translate Y.
	 */
	TransformMatrix& setTy (double value_);

	/**
	 * Returns the rotation of the Matrix. Value is in radians.
	 *
	 * @since 0.0.0
	 *
	 * @return The rotation of the Matrix.
	 */
	double getRotation ();

	/**
	 * The rotation of the Matrix, normalized to be within the Zenith right-handed
	 * clockwise rotation space. Value is in radians.
	 *
	 * @since 0.0.0
	 *
	 * @return The normalized rotation of the Matrix.
	 */
	double getRotationNormalized ();

	/**
	 * The decomposed horizontal scale of the Matrix. This value is always positive.
	 *
	 * @since 0.0.0
	 *
	 * @return The Scale X of the Matrix.
	 */
	double getScaleX ();

	/**
	 * The decomposed vertical scale of the Matrix. This value is always positive.
	 *
	 * @since 0.0.0
	 *
	 * @return The Scale Y of the Matrix.
	 */
	double getScaleY ();

	/**
	 * Reset the Matrix to an identity matrix.
	 *
	 * @since 0.0.0
	 *
	 * @return This TransformMatrix.
	 */
	TransformMatrix& loadIdentity ();

	/**
	 * Translate the Matrix.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The horizontal translation value.
	 * @param y_ The vertical translation value.
	 *
	 * @return This TransformMatrix.
	 */
	TransformMatrix& translate (double x_, double y_);

	/**
	 * Scale the Matrix.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The horizontal scale value.
	 * @param y_ The vertical scale value.
	 *
	 * @return This TransformMatrix.
	 */
	TransformMatrix& scale (double x_, double y_);

	/**
	 * Rotate the Matrix.
	 *
	 * @since 0.0.0
	 *
	 * @param angle_ The angle of rotation in radians.
	 *
	 * @return This TransformMatrix.
	 */
	TransformMatrix& rotate (double angle_);

	/**
	 * Multiply this Matrix by the given Matrix.
	 *
	 * @since 0.0.0
	 *
	 * @param rhs_ The Matrix to multiply by.
	 *
	 * @return This TransformMatrix.
	 */
	TransformMatrix& multiply (TransformMatrix rhs_);

	/**
	 * Multiply this Matrix by the matrix given, including the offset.
	 *
	 * The offsetX is added to the tx value: `offsetX * a + offsetY * c + tx`.
	 * The offsetY is added to the ty value: `offsetY * b + offsetY * d + ty`.
	 *
	 * @since 0.0.0
	 *
	 * @param src_ The source Matrix to copy from.
	 * @param offsetX_ Horizontal offset to factor in to the multiplication.
	 * @param offsetY_ Vertical offset to factor in to the multiplication.
	 *
	 * @return This TransformMatrix.
	 */
	TransformMatrix& multiplyWithOffset (TransformMatrix src_, double offsetX_, double offsetY_);

	/**
	 * Transform the Matrix.
	 *
	 * @since 0.0.0
	 *
	 * @param a_ The Scale X value.
	 * @param b_ The Shear Y value.
	 * @param c_ The Shear X value.
	 * @param d_ The Scale Y value.
	 * @param tx_ The Translate X value.
	 * @param ty_ The Translate Y value.
	 *
	 * @return This TransformMatrix.
	 */
	TransformMatrix& transform (double a_, double b_, double c_, double d_, double tx_, double ty_);

	/**
	 * Transform a point using this Matrix.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ - The x coordinate of the point to transform.
	 * @param y_ - The y coordinate of the point to transform.
	 *
	 * @return The Point containing the transformed coordinates.
	 */
	Math::Vector2 transformPoint (double x_, double y_);

	/**
	 * Invert the Matrix.
	 *
	 * @since 0.0.0
	 *
	 * @return This TransformMatrix.
	 */
	TransformMatrix& invert ();

	/**
	 * Set the values of this Matrix to copy those of the matrix given.
	 *
	 * @since 0.0.0
	 *
	 * @param src_ The source Matrix to copy from.
	 *
	 * @return This TransformMatrix.
	 */
	TransformMatrix& copyFrom (TransformMatrix src_);

	/**
	 * Set the values of this Matrix to copy those of the array given.
	 * Where array indexes 0, 1, 2, 3, 4 and 5 are mapped to a, b, c, d, e and f.
	 *
	 * @since 0.0.0
	 *
	 * @param src_ The array of values to set into this matrix.
	 *
	 * @return This TransformMatrix.
	 */
	TransformMatrix& copyFromArray (double src_[]);

	/**
	 * Set the values of this Matrix to copy those of the vector given.
	 * Where vector indexes 0, 1, 2, 3, 4 and 5 are mapped to a, b, c, d, e and f.
	 *
	 * @since 0.0.0
	 *
	 * @param src_ The vector to set into this matrix.
	 *
	 * @return This TransformMatrix.
	 */
	TransformMatrix& copyFromVector (std::vector<double> src_);

	/**
	 * Returns the values in this Matrix in the form of a vector.
	 *
	 * Where array indexes 0, 1, 2, 3, 4 and 5 are mapped to a, b, c, d, e and f.
	 *
	 * @since 0.0.0
	 *
	 * @return A vector where elements 0 to 5 contain the values from this matrix.
	 */
	std::vector<double> getVector ();

	/**
	 * Set the values of this Matrix.
	 *
	 * @since 0.0.0
	 *
	 * @param a_ The Scale X value.
	 * @param b_ The Shear Y value.
	 * @param c_ The Shear X value.
	 * @param d_ The Scale Y value.
	 * @param tx_ The Translate X value.
	 * @param ty_ The Translate Y value.
	 *
	 * @return This TransformMatrix.
	 */
	TransformMatrix& setTransform (double a_, double b_, double c_, double d_, double tx_, double ty_);

	/**
	 * Decompose this Matrix into its translation, scale and rotation values using QR decomposition.
	 *
	 * The result must be applied in the following order to reproduce the current matrix:
	 *
	 * translate -> rotate -> scale
	 *
	 * @since 0.0.0
	 *
	 * @return The decomposed Matrix.
	 */
	DecomposedMatrix decomposeMatrix ();

	/**
	 * Apply the identity, translate, rotate and scale operations on the Matrix.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The horizontal translation.
	 * @param y_ The vertical translation.
	 * @param rotation_ The angle of rotation in radians.
	 * @param scaleX_ The horizontal scale.
	 * @param scaleY_ The vertical scale.
	 *
	 * @return This TransformMatrix.
	 */
	TransformMatrix& applyITRS (double x_, double y_, double rotation_, double scaleX_, double scaleY_);

	/**
	 * Takes the `x` and `y` values and returns a new position in the `output` vector that is the inverse of
	 * the current matrix with its transformation applied.
	 *
	 * Can be used to translate points from world to local space.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The x position to translate.
	 * @param y_ The y position to translate.
	 *
	 * @return The coordinates, inverse-transformed through this matrix.
	 */
	Math::Vector2 applyInverse (double x_, double y_);

	/**
	 * Returns the X component of this matrix multiplied by the given values.
	 * This is the same as `x * a + y * c + e`.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The x value.
	 * @param y_ The y value.
	 *
	 * @return The calculated x value.
	 */
	double getX (double x_, double y_);

	/**
	 * Returns the Y component of this matrix multiplied by the given values.
	 * This is the same as `x * b + y * d + f`.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The x value.
	 * @param y_ The y value.
	 *
	 * @return The calculated y value.
	 */
	double getY (double x_, double y_);

	/**
	 * Returns the X component of this matrix multiplied by the given values.
	 *
	 * This is the same as `x * a + y * c + e`, optionally passing via `Math.round`.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The x value.
	 * @param y_ The y value.
	 * @param round_ Math.round the resulting value?
	 *
	 * @return The calculated x value.
	 */
	int getXRound (double x_, double y_, bool round_ = false);

	/**
	 * Returns the Y component of this matrix multiplied by the given values.
	 *
	 * This is the same as `x * b + y * d + f`, optionally passing via `Math.round`.
	 *
	 * @since 0.0.0
	 *
	 * @param x_ The x value.
	 * @param y_ The y value.
	 * @param round_ Math.round the resulting value?
	 *
	 * @return The calculated y value.
	 */
	int getYRound (double x_, double y_, bool round_ = false);
};

}	// namespace Components
}	// namespace GameObjects
}	// namespace Zen

#endif
