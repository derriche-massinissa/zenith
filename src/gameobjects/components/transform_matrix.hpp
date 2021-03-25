/**
 * @file
 * @author		__AUTHOR_NAME_ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD_
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_GAMEOBJECTS_COMPONENTS_TRANSFORMMATRIX_HPP
#define ZEN_GAMEOBJECTS_COMPONENTS_TRANSFORMMATRIX_HPP

#include <vector>

#include "../../ecs/entity.hpp"
#include "../../math/vector2.h"

namespace Zen {

/**
 * A structure containing the transformations of a composed transform matrix.
 *
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
 * Its components are as follow:
 * - a: Scale X
 * - b: Skew Y
 * - c: Skew X
 * - d: Scale Y
 * - e: Translate X
 * - f: Translate Y
 *
 * It is represented like so:
 * ```
 * | a | c | e |
 * | b | d | f |
 * | 0 | 0 | 1 |
 * ```
 * ... or:
 * ```
 * | a | c | tx |
 * | b | d | ty |
 * | 0 | 0 | 1  |
 * ```
 *
 * @struct TransformMatrix
 * @since 0.0.0
 */
struct TransformMatrix
{
	double a, b, c, d, e, f;
}

/**
 * The matrix values.
 *
 * @since 0.0.0
 */
std::vector<double> matrix {};

/**
 * @since 0.0.0
 *
 * @param a The Scale X value.
 * @param b The Skew Y value.
 * @param c The Skew X value.
 * @param d The Scale Y value.
 * @param tx The Translate X value.
 * @param ty The Translate Y value.
 */
Entity CreateTransformMatrix (double a = 1.0, double b = 0.0, double c = 0.0, double d = 1.0, double tx = 0.0, double ty = 0.0);

/**
 * Getter for the Translate X value.
 *
 * @since 0.0.0
 *
 * @return Translate X.
 */
double GetTx (TransformMatrix matrix);
/**
 * Setter for the Translate X value.
 *
 * @since 0.0.0
 *
 * @param value Translate X.
 */
void SetTx (TransformMatrix matrix, double value);

/**
 * Getter for the Translate Y value.
 *
 * @since 0.0.0
 *
 * @return Translate Y.
 */
double GetTy (TransformMatrix matrix);
/**
 * Setter for the Translate Y value.
 *
 * @since 0.0.0
 *
 * @param value Translate Y.
 */
void SetTy (TransformMatrix matrix, double value);

/**
 * Returns the rotation of the Matrix. Value is in radians.
 *
 * @since 0.0.0
 *
 * @return The rotation of the Matrix.
 */
double GetRotation (TransformMatrix matrix);

/**
 * The rotation of the Matrix, normalized to be within the Zenith right-handed
 * clockwise rotation space. Value is in radians.
 *
 * @since 0.0.0
 *
 * @return The normalized rotation of the Matrix.
 */
double GetRotationNormalized (TransformMatrix matrix);

/**
 * The decomposed horizontal scale of the Matrix. This value is always positive.
 *
 * @since 0.0.0
 *
 * @return The Scale X of the Matrix.
 */
double GetScaleX (TransformMatrix matrix);

/**
 * The decomposed vertical scale of the Matrix. This value is always positive.
 *
 * @since 0.0.0
 *
 * @return The Scale Y of the Matrix.
 */
double GetScaleY (TransformMatrix matrix);

/**
 * Reset the Matrix to an identity matrix.
 *
 * @since 0.0.0
 *
 * @return This TransformMatrix.
 */
void LoadIdentity (TransformMatrix matrix);

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
void Translate (TransformMatrix matrix, double x, double y);

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
void Scale (TransformMatrix matrix, double x, double y);

/**
 * Rotate the Matrix.
 *
 * @since 0.0.0
 *
 * @param angle The angle of rotation in radians.
 *
 * @return This TransformMatrix.
 */
void Rotate (TransformMatrix matrix, double angle);

/**
 * Multiply this Matrix by the given Matrix.
 *
 * @since 0.0.0
 *
 * @param rhs The Matrix to multiply by.
 *
 * @return This TransformMatrix.
 */
void Multiply (TransformMatrix matrix, const TransformMatrix& rhs);

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
void MultiplyWithOffset (TransformMatrix matrix, const TransformMatrix& src, double offsetX, double offsetY);

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
void Transform (TransformMatrix matrix, double a, double b, double c, double d, double tx, double ty);

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
Math::Vector2 TransformPoint (TransformMatrix matrix, double x, double y);

/**
 * Invert the Matrix.
 *
 * @since 0.0.0
 *
 * @return This TransformMatrix.
 */
void Invert (TransformMatrix matrix);

/**
 * Set the values of this Matrix to copy those of the matrix given.
 *
 * @since 0.0.0
 *
 * @param src The source Matrix to copy from.
 *
 * @return This TransformMatrix.
 */
void CopyFrom (TransformMatrix matrix, const TransformMatrix& src);

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
void CopyFromArray (TransformMatrix matrix, double src[]);

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
void CopyFromVector (TransformMatrix matrix, std::vector<double> src);

/**
 * Returns the values in this Matrix in the form of a vector.
 *
 * Where array indexes 0, 1, 2, 3, 4 and 5 are mapped to a, b, c, d, e and f.
 *
 * @since 0.0.0
 *
 * @return A vector where elements 0 to 5 contain the values from this matrix.
 */
std::vector<double> GetVector (TransformMatrix matrix);

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
void SetTransform (TransformMatrix matrix, double a, double b, double c, double d, double tx, double ty);

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
DecomposedMatrix DecomposeMatrix (TransformMatrix matrix);

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
void ApplyITRS (TransformMatrix matrix, double x, double y, double rotation, double scaleX, double scaleY);

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
Math::Vector2 ApplyInverse (TransformMatrix matrix, double x, double y);

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
double GetX (TransformMatrix matrix, double x, double y);

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
double GetY (TransformMatrix matrix, double x, double y);

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
int GetXRound (TransformMatrix matrix, double x, double y, bool round = false);

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
int GetYRound (TransformMatrix matrix, double x, double y, bool round = false);

}	// namespace Zen

#endif
