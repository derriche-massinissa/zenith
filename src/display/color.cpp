/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "color.hpp"

#include "../math/clamp.hpp"
#include "../math/random.hpp"

#include <algorithm>

namespace Zen {

void Transparent (Color *color)
{
	SetTo(color, 0, 0, 0, 0);
}

void SetTo (Color *color, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	SetRed(color, red, false);
	SetGreen(color, green, false);
	SetBlue(color, blue, false);
	SetAlpha(color, alpha, false);

	UpdateOnRgb(color);
}

void SetGLTo (Color *color, double red, double green, double blue, double alpha)
{
	SetRedGL(color, red, false);
	SetGreenGL(color, green, false);
	SetBlueGL(color, blue, false);
	SetAlphaGL(color, alpha, false);

	UpdateOnGL(color);
}

void SetHSV (Color *color, double h, double s, double l)
{
	SetHue(color, h, false);
	SetSaturation(color, s, false);
	SetLightness(color, l, false);

	UpdateOnHsv(color);
}

void SetHex (Color *color, uint32_t hex)
{
	uint8_t red = (hex & 0xff0000) >> 16;
	uint8_t green = (hex & 0x00ff00) >> 8;
	uint8_t blue = (hex & 0x0000ff);

	SetTo(color, red, green, blue);
}

void UpdateOnRgb (Color *color)
{
	// OpenGL Colors
	color->gl[0] = color->red / 255.;
	color->gl[1] = color->green / 255.;
	color->gl[2] = color->blue / 255.;
	color->gl[3] = color->alpha / 255.;

	// Hex
	color->hex = GetHex(color->red, color->green, color->blue);
	color->hex32 = GetHex32(color->red, color->green, color->blue, color->alpha);

	// HSV
	RgbToHsv(color, color->red, color->green, color->blue);
}

void UpdateOnGL (Color *color)
{
	// RGB
	color->red = color->gl[0] * 255;
	color->green = color->gl[1] * 255;
	color->blue = color->gl[2] * 255;
	color->alpha = color->gl[3] * 255;

	// Hex
	color->hex = GetHex(color->red, color->green, color->blue);
	color->hex32 = GetHex32(color->red, color->green, color->blue, color->alpha);

	// HSV
	RgbToHsv(color, color->red, color->green, color->blue);
}

void UpdateOnHsv (Color *color)
{
	// RGB
	HsvToRgb(color, color->hue, color->saturation, color->lightness);

	// OpenGL Colors
	color->gl[0] = color->red / 255.;
	color->gl[1] = color->green / 255.;
	color->gl[2] = color->blue / 255.;

	// Hex
	color->hex = GetHex(color->red, color->green, color->blue);
	color->hex32 = GetHex32(color->red, color->green, color->blue, color->alpha);
}

void Gray (Color *color, uint8_t shade)
{
	SetTo(color, shade, shade, shade);
}

void Random (Color *color, uint8_t min, uint8_t max)
{
	uint8_t red = Math::Random.integer(min, max);
	uint8_t green = Math::Random.integer(min, max);
	uint8_t blue = Math::Random.integer(min, max);

	SetTo(color, red, green, blue);
}

void RandomGray (Color *color, uint8_t min, uint8_t max)
{
	int shade = Math::Random.integer(min, max);

	SetTo(color, shade, shade, shade);
}

void Saturate (Color *color, uint8_t amount)
{
	amount = std::min(amount, static_cast<uint8_t>(100));
	color->saturation += amount / 100.;
}

void Desaturate (Color *color, uint8_t amount)
{
	amount = std::min(amount, static_cast<uint8_t>(100));
	color->saturation -= amount / 100.;
}

void Lighten (Color *color, uint8_t amount)
{
	amount = std::min(amount, static_cast<uint8_t>(100));
	color->lightness += amount / 100.;
}

void Darken (Color *color, uint8_t amount)
{
	amount = std::min(amount, static_cast<uint8_t>(100));
	color->lightness -= amount / 100.;
}

void Brighten (Color *color, uint8_t amount)
{
	amount = std::min(amount, static_cast<uint8_t>(100));

	double n = amount / 100.;
	n *= 255.;

	uint8_t red = Math::Clamp(color->red + n, 0, 255);
	uint8_t green = Math::Clamp(color->green + n, 0, 255);
	uint8_t blue = Math::Clamp(color->blue + n, 0, 255);

	SetTo(color, red, green, blue);
}

void SetRedGL (Color *color, double value, bool update)
{
	color->gl[0] = std::min(std::abs(value), 1.);

	if (update)
		UpdateOnGL(color);
}

void SetGreenGL (Color *color, double value, bool update)
{
	color->gl[1] = std::min(std::abs(value), 1.);

	if (update)
		UpdateOnGL(color);
}

void SetBlueGL (Color *color, double value, bool update)
{
	color->gl[2] = std::min(std::abs(value), 1.);

	if (update)
		UpdateOnGL(color);
}

void SetAlphaGL (Color *color, double value, bool update)
{
	color->gl[3] = std::min(std::abs(value), 1.);

	if (update)
		UpdateOnGL(color);
}

void SetRed (Color *color, uint8_t value, bool update)
{
	color->red = value;

	if (update)
		UpdateOnRgb(color);
}

void SetGreen (Color *color, uint8_t value, bool update)
{
	color->green = value;

	if (update)
		UpdateOnRgb(color);
}

void SetBlue (Color *color, uint8_t value, bool update)
{
	color->blue = value;

	if (update)
		UpdateOnRgb(color);
}

void SetAlpha (Color *color, uint8_t value, bool update)
{
	color->alpha = value;

	if (update)
		UpdateOnRgb(color);
}

void SetHue (Color *color, double value, bool update)
{
	color->hue = value;

	if (update)
		UpdateOnHsv(color);
}

void SetSaturation (Color *color, double value, bool update)
{
	color->saturation = value;

	if (update)
		UpdateOnHsv(color);
}

void SetLightness (Color *color, double value, bool update)
{
	color->lightness = value;

	if (update)
		UpdateOnHsv(color);
}

uint32_t GetHex (uint8_t red, uint8_t green, uint8_t blue)
{
	return red << 16 | green << 8 | blue;
}

uint32_t GetHex32 (uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	return alpha << 24 | red << 16 | green << 8 | blue;
}

int8_t ConvertValue (int n, double h, double s, double v)
{
	int k = (n + int(h) * 6) % 6;

	double min = std::min(k, std::min(4 - k, 1));

	return std::round(255 * (v - v * s * std::max(0.0, min)));
}

void HsvToRgb (Color *color, double h, double s, double v)
{
	uint8_t red = ConvertValue(5, h, s, v);
	uint8_t green = ConvertValue(3, h, s, v);
	uint8_t blue = ConvertValue(1, h, s, v);

	color->red = red;
	color->green = green;
	color->blue = blue;
}

void RgbToHsv (Color *color, uint8_t r, uint8_t g, uint8_t b)
{
	double red = r / 255.;
	double green = g / 255.;
	double blue = b / 255.;

	double min = std::min(red, std::min(green, blue));
	double max = std::max(red, std::max(green, blue));
	double d = max - min;

	// Achromatic by default
	double hue = 0.0;
	double saturation = (max == 0.0) ? 0.0 : d / max;
	double lightness = max;

	if (max != min) {
		if (max == red)
			hue = (green - blue) / d + ((green < blue) ? 6.0 : 0.0);
		else if (max == green)
			hue = (blue - red) / d + 2.0;
		else if (max == blue)
			hue = (red - green) / d + 4.0;

		hue /= 6.0;
	}

	color->hue = hue;
	color->saturation = saturation;
	color->lightness = lightness;
}

} 	// namespace Zen
