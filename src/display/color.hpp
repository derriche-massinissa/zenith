/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_DISPLAY_COLOR_HPP
#define ZEN_DISPLAY_COLOR_HPP

#include "types/color.hpp"

namespace Zen {

/**
 * Sets this color to be transparent. Sets all values to zero.
 *
 * @since 0.0.0
 */
void Transparent (Color *color);

/**
 * Sets the color of this Color component.
 *
 * @since 0.0.0
 *
 * @param red The red color value. A number between 0 and 255.
 * @param green The green color value. A number between 0 and 255.
 * @param blue The blue color value. A number between 0 and 255.
 * @param alpha The alpha value. A number between 0 and 255.
 * @param updateHSV Update the HSV values after setting the RGB values?
 */
void SetTo (Color *color, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);

/**
 * Sets the red, green, blue and alpha GL values of this Color component.
 *
 * @since 0.0.0
 *
 * @param red - The red color value. A number between 0 and 1.
 * @param green - The green color value. A number between 0 and 1.
 * @param blue - The blue color value. A number between 0 and 1.
 * @param alpha The alpha value. A number between 0 and 1.
 */
void SetGLTo (Color *color, double red, double green, double blue, double alpha = 1.0);

/**
 * Sets the color based on the hue, saturation and lightness values given.
 *
 * @since 0.0.0
 *
 * @param h The hue, in the range 0 - 1. This is the base color.
 * @param s The saturation, in the range 0 - 1. This controls how much of the 
 * hue will be in the final color, where 1 is fully saturated and 0 will 
 * give you white.
 * @param v The value, in the range 0 - 1. This controls how dark the color is. 
 * Where 1 is as bright as possible and 0 is black.
 *
 * @return This Color object.
 */
void SetHSV (Color *color, double h, double s, double l);

/**
 * Sets the color from the hex value given.
 *
 * You cannot set alpha using this method!
 *
 * @since 0.0.0
 *
 * @param hex Hex value to set this color to.
 *
 * @return This Color object.
 */
void SetHex (Color *color, uint32_t hex);

/**
 * Updates the internal cache values, using the RGB propetries.
 *
 * @since 0.0.0
 *
 * @param color The color to update.
 */
void UpdateOnRgb (Color *color);

/**
 * Updates the internal cache values, using the glRGB propetries.
 *
 * @since 0.0.0
 *
 * @param color The color to update.
 */
void UpdateOnGL (Color *color);

/**
 * Updates the internal cache values, using the HSV propetries.
 *
 * @since 0.0.0
 *
 * @param color The color to update.
 */
void UpdateOnHsv (Color *color);

/**
 * Sets this Color object to be grayscaled based on the shade value given.
 *
 * @since 0.0.0
 * 
 * @param shade A value between 0 and 255.
 *
 * @return This Color object.
 */
void Gray (Color *color, uint8_t shade);

/**
 * Sets this Color object to be a random color between the `min` and `max` 
 * values given.
 *
 * @since 0.0.0
 * 
 * @param min The minimum random color value. Between 0 and 255.
 * @param max The maximum random color value. Between 0 and 255.
 *
 * @return This Color object.
 */
void Random (Color *color, uint8_t min = 0, uint8_t max = 255);

/**
 * Sets this Color object to be a random grayscale color between the `min` and 
 * `max` values given.
 *
 * @since 0.0.0
 *
 * @param min The minimum random color value. Between 0 and 255.
 * @param max The maximum random color value. Between 0 and 255.
 *
 * @return This Color object.
 */
void RandomGray (Color *color, uint8_t min = 0, uint8_t max = 255);

/**
 * Increase the saturation of this Color by the percentage amount given.
 * The saturation is the amount of the base color in the hue.
 *
 * @since 0.0.0
 * 
 * @param amount The percentage amount to change this color by. A value
 * between 0 and 100.
 *
 * @return This Color object.
 */
void Saturate (Color *color, uint8_t amount);

/**
 * Decrease the saturation of this Color by the percentage amount given.
 * The saturation is the amount of the base color in the hue.
 *
 * @since 0.0.0
 * 
 * @param amount The percentage amount to change this color by. A value
 * between 0 and 100.
 *
 * @return This Color object.
 */
void Desaturate (Color *color, uint8_t amount);

/**
 * Increase the lightness of this Color by the percentage amount given.
 *
 * @since 0.0.0
 * 
 * @param amount The percentage amount to change this color by. A value
 * between 0 and 100.
 *
 * @return This Color object.
 */
void Lighten (Color *color, uint8_t amount);

/**
 * Decrease the lightness of this Color by the percentage amount given.
 *
 * @since 0.0.0
 * 
 * @param amount The percentage amount to change this color by. A value
 * between 0 and 100.
 *
 * @return This Color object.
 */
void Darken (Color *color, uint8_t amount);

/**
 * Brighten this Color by the percentage amount given.
 *
 * @since 0.0.0
 * 
 * @param amount The percentage amount to change this color by. A value
 * between 0 and 100.
 *
 * @return This Color object.
 */
void Brighten (Color *color, uint8_t amount);

/**
 * Sets the color using a red color value in the range of 0-1.
 *
 * @param color The color to set.
 * @param value The value to use.
 * @param update Should all cached values be updated?
 */
void SetRedGL (Color *color, double value, bool update = true);

/**
 * Sets the color using a green color value in the range of 0-1.
 *
 * @param color The color to set.
 * @param value The value to use.
 * @param update Should all cached values be updated?
 */
void SetGreenGL (Color *color, double value, bool update = true);

/**
 * Sets the color using a blue color value in the range of 0-1.
 *
 * @param color The color to set.
 * @param value The value to use.
 * @param update Should all cached values be updated?
 */
void SetBlueGL (Color *color, double value, bool update = true);

/**
 * Sets the color using an alpha color value in the range of 0-1.
 *
 * @param color The color to set.
 * @param value The value to use.
 * @param update Should all cached values be updated?
 */
void SetAlphaGL (Color *color, double value, bool update = true);

/**
 * Sets the color using a red color value in the range of 0-255.
 *
 * @param color The color to set.
 * @param value The value to use.
 * @param update Should all cached values be updated?
 */
void SetRed (Color *color, uint8_t value, bool update = true);

/**
 * Sets the color using a green color value in the range of 0-255.
 *
 * @param color The color to set.
 * @param value The value to use.
 * @param update Should all cached values be updated?
 */
void SetGreen (Color *color, uint8_t value, bool update = true);

/**
 * Sets the color using a blue color value in the range of 0-255.
 *
 * @param color The color to set.
 * @param value The value to use.
 * @param update Should all cached values be updated?
 */
void SetBlue (Color *color, uint8_t value, bool update = true);

/**
 * Sets the color using an alpha color value in the range of 0-255.
 *
 * @param color The color to set.
 * @param value The value to use.
 * @param update Should all cached values be updated?
 */
void SetAlpha (Color *color, uint8_t value, bool update = true);

/**
 * Sets the color using a hue color value in the range of 0-1.
 *
 * @param color The color to set.
 * @param value The value to use.
 * @param update Should all cached values be updated?
 */
void SetHue (Color *color, double value, bool update = true);

/**
 * Sets the color using a saturation color value in the range of 0-1.
 *
 * @param color The color to set.
 * @param value The value to use.
 * @param update Should all cached values be updated?
 */
void SetSaturation (Color *color, double value, bool update = true);

/**
 * Sets the color using a lightness color value in the range of 0-1.
 *
 * @param color The color to set.
 * @param value The value to use.
 * @param update Should all cached values be updated?
 */
void SetLightness (Color *color, double value, bool update = true);

/**
 * Given 3 separate color values this will return an hexadecimal representation
 * of it.
 *
 * @since 0.0.0
 *
 * @param red The red color value. A number between 0 and 255.
 * @param green The green color value. A number between 0 and 255.
 * @param blue The blue color value. A number between 0 and 255.
 *
 * @return The combined color value.
 */
uint32_t GetHex (uint8_t red, uint8_t green, uint8_t blue);

/**
 * Given an alpha and 3 separate color values this will return an integer
 * representation of it.
 *
 * @since 0.0.0
 *
 * @param red The red color value. A number between 0 and 255.
 * @param green The green color value. A number between 0 and 255.
 * @param blue The blue color value. A number between 0 and 255.
 * @param alpha The alpha color value. A number between 0 and 255.
 *
 * @return The combined color value.
 */
uint32_t GetHex32 (uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

/**
 * Given 3 separate color values, this will return an integer representation of it.
 *
 * @since 0.0.0
 * @param r The red component in a range from 0.0 to 1.0.
 * @param g The green component in a range from 0.0 to 1.0.
 * @param b The blue component in a range from 0.0 to 1.0.
 * @return The packed RGBA values as a Uint32.
 */
uint32_t GetHexF (double r, double g, double b);

/**
 * Given an alpha and 3 separate color values, this will return an integer
 * representation of it.
 *
 * @since 0.0.0
 * @param r The red component in a range from 0.0 to 1.0.
 * @param g The green component in a range from 0.0 to 1.0.
 * @param b The blue component in a range from 0.0 to 1.0.
 * @param a The alpha component in a range from 0.0 to 1.0.
 * @return The packed RGBA values as a Uint32.
 */
uint32_t GetHexF (double r, double g, double b, double a);

/**
 * RGB space conversion.
 *
 * @param n The value to convert.
 * @param h The h value.
 * @param s The s value.
 * @param v The v value.
 *
 * @return The converted value.
 */
uint8_t ConvertValue (Color *color, int n, double h, double s, double v);

/**
 * Converts a HSV (hue, saturation and value) color set to RGB.
 *
 * Conversion formula from https://en.wikipedia.org/wiki/HSL_and_HSV
 *
 * Assumes HSV values are contained in the set [0, 1].
 *
 * @since 0.0.0
 *
 * @param h The hue, in the range 0 - 1. This is the base color.
 * @param s The saturation, in the range 0 - 1. This controls how much of the 
 * hue will be in the final color, where 1 is fully saturated and 0 will give 
 * you white.
 * @param v The value, in the range 0 - 1. This controls how dark the color is. 
 * Where 1 is as bright as possible and 0 is black.
 * @param out A Color object to store the results in.
 */
void HsvToRgb (Color *color, double h, double s, double v);

/**
 * Converts an RGB color value to HSV (hue, saturation and value).
 * Conversion formula from http://en.wikipedia.org/wiki/HSL_color_space.
 * Assumes RGB values are contained in the set [0, 255] and returns h, s and 
 * v in the set [0, 1].
 * Based on code by Michael Jackson (https://github.com/mjijackson)
 *
 * @since 0.0.0
 *
 * @param r The red color value. A number between 0 and 255.
 * @param g The green color value. A number between 0 and 255.
 * @param b The blue color value. A number between 0 and 255.
 * @param out An object to store the color values in.
 */
void RgbToHsv (Color *color, uint8_t r, uint8_t g, uint8_t b);

}	// namespace Zen

#endif
