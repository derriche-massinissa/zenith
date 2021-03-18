/**
 * @file		color.h
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef COLOR_H
#define COLOR_H

namespace Zen {
namespace Display {

/**
 * The Color class holds a single color value and allows for easy modification
 * and reading of it.
 *
 * @class Color
 * @since 0.0.0
 */
class Color
{
public:
	/**
	 * @since 0.0.0
	 *
	 * @param red The red color value. A number between 0 and 255.
	 * @param green The green color value. A number between 0 and 255.
	 * @param blue The blue color value. A number between 0 and 255.
	 * @param alpha The red value. A number between 0 and 255.
	 */
	Color (int red = 0, int green = 0, int blue = 0, int alpha = 255);

	/**
	 * Is this a valid color?
	 *
	 * @since 0.0.0
	 */
	bool valid = true;

	/**
	 * The internal red color value.
	 *
	 * @since 0.0.0
	 */
	int r = 0;

	/**
	 * The internal green color value.
	 *
	 * @since 0.0.0
	 */
	int g = 0;

	/**
	 * The internal blue color value.
	 *
	 * @since 0.0.0
	 */
	int b = 0;

	/**
	 * The internal alpha color value.
	 *
	 * @since 0.0.0
	 */
	int a = 255;

	/**
	 * The hue color value. A number between 0 and 1.
	 *
	 * @since 0.0.0
	 */
	double h = 0;

	/**
	 * The saturation color value. A number between 0 and 1.
	 *
	 * @since 0.0.0
	 */
	double s = 0;

	/**
	 * The value of the color. A number between 0 and 1.
	 *
	 * @since 0.0.0
	 */
	double v = 0;

	/**
	 * Is this color update locked?
	 *
	 * @since 0.0.0
	 */
	bool locked = false;

	/**
	 * An array containing the calculated color values for WebGL use.
	 *
	 * @since 0.0.0
	 */
	std::vector<double> gl = { 0, 0, 0, 1 };

	/**
     * The color of this Color component, not including the alpha channel.
	 *
	 * @since 0.0.0
	 */
	int color = 0;

	/**
     * The color of this Color component, including the alpha channel.
	 *
	 * @since 0.0.0
	 */
	Uint32 color32 = 0;

    /**
     * Sets this color to be transparent. Sets all values to zero.
     *
     * @since 0.0.0
     *
     * @return This Color object.
     */
    Color& transparent ();

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
     *
     * @return This Color object.
     */
    Color& setTo (int red, int green, int blue, int alpha = 255, bool updateHSV = true);

    /**
     * Sets the red, green, blue and alpha GL values of this Color component.
     *
     * @since 0.0.0
     *
     * @param red - The red color value. A number between 0 and 1.
     * @param green - The green color value. A number between 0 and 1.
     * @param blue - The blue color value. A number between 0 and 1.
     * @param alpha The alpha value. A number between 0 and 1.
     *
     * @return This Color object.
     */
    Color& setGLTo (double red, double green, double blue, double alpha = 1.0);

    /**
     * Sets the color based on the color object given.
     *
     * @since 0.0.0
     *
     * @param color An object containing `r`, `g`, `b` and optionally `a` values in 
	 * the range 0 to 255.
     *
     * @return This Color object.
     */
    Color& setFromRGB (InputColorObject color);

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
    Color& setFromHSV (double h, double s, double v);

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
	Color& setFromHex (unsigned int hex);

    /**
     * Updates the internal cache values.
     *
     * @since 0.0.0
	 *
	 * @param updateHSV Should the HSV values be updated as well?
     *
     * @return This Color object.
     */
    Color& update (bool updateHSV = false);

    /**
     * Updates the internal hsv cache values.
     *
     * @since 0.0.0
     *
     * @return This Color object.
     */
    Color& updateHSV ();

    /**
     * Returns a new Color component using the values from this one.
     *
     * @since 0.0.0
     *
     * @return A new Color object.
     */
    Color clone ();

    /**
     * Sets this Color object to be grayscaled based on the shade value given.
     *
     * @since 0.0.0
     * 
     * @param shade A value between 0 and 255.
     *
     * @return This Color object.
     */
    Color& gray (int shade);

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
    Color& random (int min = 0, int max = 255);

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
    Color& randomGray (int min = 0, int max = 255);

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
    Color& saturate (int amount);

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
    Color& desaturate (int amount);

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
    Color& lighten (int amount);

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
    Color& darken (int amount);

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
    Color& brighten (int amount);

    /**
     * The red color value, normalized to the range 0 to 1.
     *
     * @since 0.0.0
	 *
	 * @return Normalized red color value.
     */
    double redGL ();

    /**
     * The green color value, normalized to the range 0 to 1.
     *
     * @since 0.0.0
	 *
	 * @return Normalized green color value.
     */
    double greenGL ();

    /**
     * The blue color value, normalized to the range 0 to 1.
     *
     * @since 0.0.0
	 *
	 * @return Normalized blue color value.
     */
    double blueGL ();

    /**
     * The alpha color value, normalized to the range 0 to 1.
     *
     * @since 0.0.0
	 *
	 * @return Normalized alpha color value.
     */
    double alphaGL ();

    /**
     * The red color value, normalized to the range 0 to 1.
     *
     * @since 0.0.0
	 *
	 * @param value The new red value.
	 *
	 * @return This Color object.
     */
    Color& redGL (double value);

    /**
     * The green color value, normalized to the range 0 to 1.
     *
     * @since 0.0.0
	 *
	 * @param value The new green value.
	 *
	 * @return This Color object.
     */
    Color& greenGL (double value);

    /**
     * The blue color value, normalized to the range 0 to 1.
     *
     * @since 0.0.0
	 *
	 * @param value The new blue value.
	 *
	 * @return This Color object.
     */
    Color& blueGL (double value);

    /**
     * The alpha color value, normalized to the range 0 to 1.
     *
     * @since 0.0.0
	 *
	 * @param value The new alpha value.
	 *
	 * @return This Color object.
     */
    Color& alphaGL (double value);

    /**
     * The red color value, normalized to the range 0 to 255.
     *
     * @since 0.0.0
	 *
	 * @return The red color value.
     */
	int red ();

    /**
     * The green color value, normalized to the range 0 to 255.
     *
     * @since 0.0.0
	 *
	 * @return The green color value.
     */
	int green ();

    /**
     * The blue color value, normalized to the range 0 to 255.
     *
     * @since 0.0.0
	 *
	 * @return The blue color value.
     */
	int blue ();

    /**
     * The alpha color value, normalized to the range 0 to 255.
     *
     * @since 0.0.0
	 *
	 * @return The alpha color value.
     */
	int alpha ();

    /**
     * The red color value, normalized to the range 0 to 255.
     *
     * @since 0.0.0
	 *
	 * @param value The new red color value.
	 *
	 * @return This Color object.
     */
	Color& red (int value_);

    /**
     * The green color value, normalized to the range 0 to 255.
     *
     * @since 0.0.0
	 *
	 * @param value The new green color value.
	 *
	 * @return This Color object.
     */
	Color& green (int value_);

    /**
     * The blue color value, normalized to the range 0 to 255.
     *
     * @since 0.0.0
	 *
	 * @param value The new blue color value.
	 *
	 * @return This Color object.
     */
	Color& blue (int value_);

    /**
     * The alpha color value, normalized to the range 0 to 255.
     *
     * @since 0.0.0
	 *
	 * @param value The new alpha color value.
	 *
	 * @return This Color object.
     */
	Color& alpha (int value_);

    /**
     * The hue color value. A number between 0 and 1.
     * This is the base color.
     *
     * @since 0.0.0
	 *
	 * @return The hue.
     */
    double hue ();

    /**
     * The saturation color value. A number between 0 and 1.
     * This controls how much of the hue will be in the final color, where 1 is
	 * fully saturated and 0 will give you white.
     *
     * @since 0.0.0
	 *
	 * @return The saturation.
     */
	double saturation ();

    /**
     * The lightness color value. A number between 0 and 1.
     * This controls how dark the color is. Where 1 is as bright as possible and
	 * 0 is black.
     *
     * @since 0.0.0
	 *
	 * @return The value.
     */
	double value ();

    /**
     * The hue color value. A number between 0 and 1.
     * This is the base color.
     *
     * @since 0.0.0
	 *
	 * @param value The new hue.
	 *
	 * @return This Color object.
     */
	Color& hue (double value_);

    /**
     * The saturation color value. A number between 0 and 1.
     * This controls how much of the hue will be in the final color, where 1 is
	 * fully saturated and 0 will give you white.
     *
     * @since 0.0.0
	 *
	 * @param value The new saturation.
	 *
	 * @return This Color object.
     */
	Color& saturation (double value_);

    /**
     * The lightness color value. A number between 0 and 1.
     * This controls how dark the color is. Where 1 is as bright as possible and
	 * 0 is black.
     *
     * @since 0.0.0
	 *
	 * @param value The new value.
	 *
	 * @return This Color object.
     */
	Color& value (double value_);

	/**
	 * Given 3 separate color values this will return an integer representation
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
	static int getColor (int red, int green, int blue);

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
	static int getColor32 (int red, int green, int blue, int alpha);

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
	static int convertValue (int n, float h, float s, float v);

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
	static void hsvToRgb (double h, double s, double v, Color *out);

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
	static void rgbToHsv (int r, int g, int b, Color *out);
};

}	// namespace Display
}	// namespace Zen

#endif
