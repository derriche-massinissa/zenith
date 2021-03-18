/**
 * @file		transform_matrix.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef TRANSFORM_MATRIX_H
#define TRANSFORM_MATRIX_H

#include <vector>
#include <cmath>

namespace Zen {
namespace GameObjects {
namespace Components {

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
 *
 * @param a The Scale X value.
 * @param b The Skew Y value.
 * @param c The Skew X value.
 * @param d The Scale Y value.
 * @param tx The Translate X value.
 * @param ty The Translate Y value.
 */
class TransformMatrix
{
public:
	TransformMatrix (double a = 1.0, double b = 0.0, double c = 0.0, double d = 1.0, double tx = 0.0, double ty = 0.0);

	/**
	 * The matrix values.
	 *
	 * @since 0.0.0
	 */
	std::vector<double> matrix_ {};

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
	 * @param value Scale X.
	 */
	TransformMatrix& setA (double value);

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
	 * @param value Skew Y.
	 */
	TransformMatrix& setB (double value);

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
	 * @param value Skew X.
	 */
	TransformMatrix& setC (double value);

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
	 * @param value Scale Y.
	 */
	TransformMatrix& setD (double value);

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
	 * @param value Translate X.
	 */
	TransformMatrix& setE (double value);

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
	 * @param value Translate Y.
	 */
	TransformMatrix& setF (double value);

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
	 * @param value Translate X.
	 */
	TransformMatrix& setTx (double value);

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
	 * @param value Translate Y.
	 */
	TransformMatrix& setTy (double value);

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
     * @param x The horizontal translation value.
     * @param y The vertical translation value.
     *
     * @return This TransformMatrix.
     */
    TransformMatrix& translate (double x, double y);

    /**
     * Scale the Matrix.
     *
     * @since 0.0.0
     *
     * @param x The horizontal scale value.
     * @param y The vertical scale value.
     *
     * @return This TransformMatrix.
     */
    TransformMatrix& scale (double x, double y);

    /**
     * Rotate the Matrix.
     *
     * @since 0.0.0
     *
     * @param angle The angle of rotation in radians.
     *
     * @return This TransformMatrix.
     */
    TransformMatrix& rotate (double angle);

    /**
     * Multiply this Matrix by the given Matrix.
     *
     * @since 0.0.0
     *
     * @param rhs The Matrix to multiply by.
     *
     * @return This TransformMatrix.
     */
    TransformMatrix& multiply (TransformMatrix rhs);

    /**
     * Multiply this Matrix by the matrix given, including the offset.
     *
     * The offsetX is added to the tx value: `offsetX * a + offsetY * c + tx`.
     * The offsetY is added to the ty value: `offsetY * b + offsetY * d + ty`.
     *
     * @since 0.0.0
     *
     * @param src The source Matrix to copy from.
     * @param offsetX Horizontal offset to factor in to the multiplication.
     * @param offsetY Vertical offset to factor in to the multiplication.
     *
     * @return This TransformMatrix.
     */
    TransformMatrix& multiplyWithOffset (TransformMatrix src, double offsetX, double offsetY);

    /**
     * Transform the Matrix.
     *
     * @since 0.0.0
     *
     * @param a The Scale X value.
     * @param b The Shear Y value.
     * @param c The Shear X value.
     * @param d The Scale Y value.
     * @param tx The Translate X value.
     * @param ty The Translate Y value.
     *
     * @return This TransformMatrix.
     */
    TransformMatrix& transform (double a, double b, double c, double d, double tx, double ty);

    /**
     * Transform a point using this Matrix.
     *
     * @since 0.0.0
     *
     * @param x - The x coordinate of the point to transform.
     * @param y - The y coordinate of the point to transform.
     *
     * @return The Point containing the transformed coordinates.
     */
	Math::Vector2 transformPoint (double x, double y);

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
     * @param src The source Matrix to copy from.
     *
     * @return This TransformMatrix.
     */
    TransformMatrix& copyFrom (TransformMatrix src);

    /**
     * Set the values of this Matrix to copy those of the array given.
     * Where array indexes 0, 1, 2, 3, 4 and 5 are mapped to a, b, c, d, e and f.
     *
     * @since 0.0.0
     *
     * @param src The array of values to set into this matrix.
     *
     * @return This TransformMatrix.
     */
    TransformMatrix& copyFromArray (double src[]);

    /**
     * Set the values of this Matrix to copy those of the vector given.
     * Where vector indexes 0, 1, 2, 3, 4 and 5 are mapped to a, b, c, d, e and f.
     *
     * @since 0.0.0
     *
     * @param src The vector to set into this matrix.
     *
     * @return This TransformMatrix.
     */
    TransformMatrix& copyFromVector (std::vector<double> src);

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
     * @param a The Scale X value.
     * @param b The Shear Y value.
     * @param c The Shear X value.
     * @param d The Scale Y value.
     * @param tx The Translate X value.
     * @param ty The Translate Y value.
     *
     * @return This TransformMatrix.
     */
    TransformMatrix& setTransform (double a, double b, double c, double d, double tx, double ty);

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
     * @param x The horizontal translation.
     * @param y The vertical translation.
     * @param rotation The angle of rotation in radians.
     * @param scaleX The horizontal scale.
     * @param scaleY The vertical scale.
     *
     * @return This TransformMatrix.
     */
    TransformMatrix& applyITRS (double x, double y, double rotation, double scaleX, double scaleY);

    /**
     * Takes the `x` and `y` values and returns a new position in the `output` vector that is the inverse of
     * the current matrix with its transformation applied.
     *
     * Can be used to translate points from world to local space.
     *
     * @since 0.0.0
     *
     * @param x The x position to translate.
     * @param y The y position to translate.
     *
     * @return The coordinates, inverse-transformed through this matrix.
     */
	Math::Vector2 applyInverse (double x, double y);

    /**
     * Returns the X component of this matrix multiplied by the given values.
     * This is the same as `x * a + y * c + e`.
     *
     * @since 0.0.0
     *
     * @param x The x value.
     * @param y The y value.
     *
     * @return The calculated x value.
     */
    double getX (double x, double y);

    /**
     * Returns the Y component of this matrix multiplied by the given values.
     * This is the same as `x * b + y * d + f`.
     *
     * @since 0.0.0
     *
     * @param x The x value.
     * @param y The y value.
     *
     * @return The calculated y value.
     */
    double getY (double x, double y);

    /**
     * Returns the X component of this matrix multiplied by the given values.
     *
     * This is the same as `x * a + y * c + e`, optionally passing via `Math.round`.
     *
     * @since 0.0.0
     *
     * @param x The x value.
     * @param y The y value.
     * @param round Math.round the resulting value?
     *
     * @return The calculated x value.
     */
    int getXRound (double x, double y, bool round = false);

    /**
     * Returns the Y component of this matrix multiplied by the given values.
     *
     * This is the same as `x * b + y * d + f`, optionally passing via `Math.round`.
     *
     * @since 0.0.0
     *
     * @param x The x value.
     * @param y The y value.
     * @param round Math.round the resulting value?
     *
     * @return The calculated y value.
     */
    int getYRound (double x, double y, bool round = false);
};

}	// namespace Components
}	// namespace GameObjects
}	// namespace Zen

#endif
