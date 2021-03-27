/**
 * @file
 * @author		__AUTHOR_NAME_ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD_
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_SYSTEMS_TRANSFORMMATRIX_HPP
#define ZEN_SYSTEMS_TRANSFORMMATRIX_HPP

#include "../ecs/entity.hpp"
#include "../math/vector2.hpp"
#include "../components/transform_matrix.hpp"
#include "../structs/types/decomposed_matrix.hpp"

namespace Zen {

/**
 * Getter for the Translate X value.
 *
 * @since 0.0.0
 *
 * @return Translate X.
 */
double GetTx (Components::TransformMatrix matrix);

/**
 * Setter for the Translate X value.
 *
 * @since 0.0.0
 *
 * @param value Translate X.
 */
void SetTx (Components::TransformMatrix *matrix, double value);

/**
 * Getter for the Translate Y value.
 *
 * @since 0.0.0
 *
 * @return Translate Y.
 */
double GetTy (Components::TransformMatrix matrix);

/**
 * Setter for the Translate Y value.
 *
 * @since 0.0.0
 *
 * @param value Translate Y.
 */
void SetTy (Components::TransformMatrix *matrix, double value);

/**
 * Returns the rotation of the Matrix. Value is in radians.
 *
 * @since 0.0.0
 *
 * @return The rotation of the Matrix.
 */
double GetRotation (Components::TransformMatrix matrix);

/**
 * The rotation of the Matrix, normalized to be within the Zenith right-handed
 * clockwise rotation space. Value is in radians.
 *
 * @since 0.0.0
 *
 * @return The normalized rotation of the Matrix.
 */
double GetRotationNormalized (Components::TransformMatrix matrix);

/**
 * The decomposed horizontal scale of the Matrix. This value is always positive.
 *
 * @since 0.0.0
 *
 * @return The Scale X of the Matrix.
 */
double GetScaleX (Components::TransformMatrix matrix);

/**
 * The decomposed vertical scale of the Matrix. This value is always positive.
 *
 * @since 0.0.0
 *
 * @return The Scale Y of the Matrix.
 */
double GetScaleY (Components::TransformMatrix matrix);

/**
 * Reset the Matrix to an identity matrix.
 *
 * @since 0.0.0
 *
 * @return This Components::TransformMatrix.
 */
void LoadIdentity (Components::TransformMatrix *matrix);

/**
 * Translate the Matrix.
 *
 * @since 0.0.0
 *
 * @param x The horizontal translation value.
 * @param y The vertical translation value.
 *
 * @return This Components::TransformMatrix.
 */
void Translate (Components::TransformMatrix *matrix, double x, double y);

/**
 * Scale the Matrix.
 *
 * @since 0.0.0
 *
 * @param x The horizontal scale value.
 * @param y The vertical scale value.
 *
 * @return This Components::TransformMatrix.
 */
void Scale (Components::TransformMatrix *matrix, double x, double y);

/**
 * Rotate the Matrix.
 *
 * @since 0.0.0
 *
 * @param angle The angle of rotation in radians.
 *
 * @return This Components::TransformMatrix.
 */
void Rotate (Components::TransformMatrix *matrix, double angle);

/**
 * Multiply this Matrix by the given Matrix.
 *
 * @since 0.0.0
 *
 * @param rhs The Matrix to multiply by.
 *
 * @return This Components::TransformMatrix.
 */
void Multiply (Components::TransformMatrix *matrix, const Components::TransformMatrix& rhs);

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
 * @return This Components::TransformMatrix.
 */
void MultiplyWithOffset (Components::TransformMatrix *matrix, const Components::TransformMatrix& src, double offsetX, double offsetY);

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
 * @return This Components::TransformMatrix.
 */
void Transform (Components::TransformMatrix *matrix, double a, double b, double c, double d, double tx, double ty);

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
Math::Vector2 TransformPoint (Components::TransformMatrix matrix, double x, double y);

/**
 * Invert the Matrix.
 *
 * @since 0.0.0
 *
 * @return This Components::TransformMatrix.
 */
void Invert (Components::TransformMatrix *matrix);

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
DecomposedMatrix DecomposeMatrix (Components::TransformMatrix matrix);

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
 * @return This Components::TransformMatrix.
 */
void ApplyITRS (Components::TransformMatrix *matrix, double x, double y, double rotation, double scaleX, double scaleY);

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
Math::Vector2 ApplyInverse (Components::TransformMatrix matrix, double x, double y);

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
double GetX (Components::TransformMatrix matrix, double x, double y);

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
double GetY (Components::TransformMatrix matrix, double x, double y);

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
int GetXRound (Components::TransformMatrix matrix, double x, double y, bool round = false);

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
int GetYRound (Components::TransformMatrix matrix, double x, double y, bool round = false);

}	// namespace Zen

#endif
