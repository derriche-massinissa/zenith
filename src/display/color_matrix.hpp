/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_DISPLAY_COLORMATRIX_HPP
#define ZEN_DISPLAY_COLORMATRIX_HPP

#include "types/color_matrix.hpp"

namespace Zen {

/**
 * Sets this ColorMatrix from the given array of color values.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param value The ColorMatrix values to set.
 */
void Set (ColorMatrix* colorMatrix, std::array<double, 20> value);

/**
 * Resets a ColorMatrix.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 */
void Reset (ColorMatrix* colorMatrix);

/**
 * Gets the ColorMatrix data.
 *
 * Can be used directly as a 1fv shader uniform value.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 *
 * @return The ColorMatrix data.
 */
std::array<double, 20> GetData (ColorMatrix* colorMatrix);

/**
 * Changes the brightness of this ColorMatrix by the given amount.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param value The amount of brightness to apply to this ColorMatrix.
 * Between 0 (black) and 1.
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it
 * (`false`) ?
 */
void Brightness (ColorMatrix* colorMatrix, double value = 0, bool multiply = false);

/**
 * Changes the saturation of this ColorMatrix by the given amount.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param value The amount of saturation to apply to this ColorMatrix.
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it
 * (`false`) ?
 */
void Saturate (ColorMatrix* colorMatrix, double value = 0, bool multiply = false);

/**
 * Desaturates this ColorMatrix (removes color from it).
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it
 * (`false`) ?
 */
void Desaturate (ColorMatrix* colorMatrix, bool multiply = false);

/**
 * Rotates the hues of this ColorMatrix by the value given.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param rotation The amount of hue rotation to apply to this ColorMatrix, in
 * degrees.
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it
 * (`false`) ?
 */
void Hue (ColorMatrix* colorMatrix, double rotation = 0, bool multiply = false);

/**
 * Sets this ColorMatrix to be grayscale.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param value The grayscale scale (0 is black).
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it
 * (`false`) ?
 */
void Grayscale (ColorMatrix* colorMatrix, double value = 1, bool multiply = false);

/**
 * Sets this ColorMatrix to be black and white.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it
 * (`false`) ?
 */
void BlackWhite (ColorMatrix* colorMatrix, bool multiply = false);

/**
 * Change the contrast of this ColorMatrix by the amount given.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param value The amount of contrast to apply to this ColorMatrix.
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it
 * (`false`) ?
 */
void Contrast (ColorMatrix* colorMatrix, double value = 0, bool multiply = false);

/**
 * Converts this ColorMatrix to have negative values.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it
 * (`false`) ?
 */
void Negative (ColorMatrix* colorMatrix, bool multiply = false);

/**
 * Apply a desaturated luminance to this ColorMatrix.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it
 * (`false`) ?
 */
void DesaturateLuminance (ColorMatrix* colorMatrix, bool multiply = false);

/**
 * Applies a sepia tone to this ColorMatrix.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it
 * (`false`) ?
 */
void Sepia (ColorMatrix* colorMatrix, bool multiply = false);

/** Applies a night vision tone to this ColorMatrix.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param intensity The intensity of this effect.
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it (`false`) ?
 */
void Night (ColorMatrix* colorMatrix, double intensity = .1, bool multiply = false);

/** Applies a trippy color tone to this ColorMatrix.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it
 * (`false`) ?
 */
void Lsd (ColorMatrix* colorMatrix, bool multiply = false);

/** Applies a brown tone to this ColorMatrix.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it
 * (`false`) ?
 */
void Brown (ColorMatrix* colorMatrix, bool multiply = false);

/** Applies a vintage pinhole color effect to this ColorMatrix.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it
 * (`false`) ?
 */
void VintagePinhole (ColorMatrix* colorMatrix, bool multiply = false);

/** Applies a kodachrome color effect to this ColorMatrix.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it
 * (`false`) ?
 */
void Kodachrome (ColorMatrix* colorMatrix, bool multiply = false);

/** Applies a technicolor color effect to this ColorMatrix.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it
 * (`false`) ?
 */
void Technicolor (ColorMatrix* colorMatrix, bool multiply = false);

/** Applies a polaroid color effect to this ColorMatrix.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it
 * (`false`) ?
 */
void Polaroid (ColorMatrix* colorMatrix, bool multiply = false);

/** Shifts the values of this ColorMatrix into BGR order.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param multiply Multiply the resulting ColorMatrix (`true`), or set it
 * (`false`) ?
 */
void ShiftToBGR (ColorMatrix* colorMatrix, bool multiply = false);

/** Multiplies the two given matrices.
 *
 * @since 0.0.0
 *
 * @param colorMatrix The color matrix.
 * @param a The 5x4 array to multiply with the color matrix.
 */
void Multiply (ColorMatrix* colorMatrix, std::array<double, 20> other,
		bool multiply = true);

}	// namespace Zen

#endif
