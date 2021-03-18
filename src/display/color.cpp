/**
 * @file		color.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "color.h"
#include <algorithm>
#include <cmath>

namespace Zen {
namespace Display {

Color::Color (int red, int green, int blue, int alpha)
{
	setTo(red, green, blue, alpha);
}

Color& Color::transparent ()
{
	locked = true;

	red(0);
	green(0);
	blue(0);
	alpha(0);

	locked = false;

	return update(true);
}

Color& Color::setTo (int red, int green, int blue, int alpha, bool updateHSV)
{
	locked = true;

	red(red);
	green(green);
	blue(blue);
	alpha(alpha);

	locked = false;

	return update(updateHSV);
}

Color& Color::setGLTo (double red, double green, double blue, double alpha)
{
	locked = true;

	redGL(red);
	greenGL(green);
	blueGL(blue);
	alphaGL(alpha);

	locked = false;

	return update(true);
}

Color& Color::setFromRGB (InputColorObject color)
{
	locked = true;

	red(color.r);
	green(color.g);
	blue(color.b);

	if (color.a >= 0)
		alpha(color.a);

	locked = false;

	return update(true);
}

Color& Color::setFromHSV (double h, double s, double v)
{
	hsvToRgb(h, s, v, this);

	return *this;
}

Color& setFromHex (unsigned int hex)
{
	int red = (hex & 0xff0000) >> 16;
	int green = (hex & 0x00ff00) >> 8;
	int blue = (hex & 0x0000ff);

	return setTo(red, green, blue);
}

Color& Color::update (bool updateHSV)
{
	if (locked)
		return *this;

	color = getColor(red(), green(), blue());
	color32 = getColor32(red(), green(), blue(), alpha());

	if (updateHSV)
		rgbToHsv(red(), green(), blue(), this);

	return *this;
}

Color& Color::updateHSV ()
{
	rgbToHsv(red(), green(), blue(), this);

	return *this;
}

Color Color::clone ()
{
	return *this;
}

Color& Color::gray (int shade)
{
	return setTo(shade, shade, shade);
}

Color& Color::random (int min, int max)
{
	int red = Math::random.integer(min, max);
	int green = Math::random.integer(min, max);
	int blue = Math::random.integer(min, max);

	return setTo(red, green, blue);
}

Color& Color::randomGray (int min, int max)
{
	int shade = Math::random.integer(min, max);

	return setTo(shade, shade, shade);
}

Color& Color::saturate (int amount)
{
	s += amount / 100.0;

	return *this;
}

Color& Color::desaturate (int amount)
{
	s -= amount / 100.0;

	return *this;
}

Color& Color::lighten (int amount)
{
	v += amount / 100.0;

	return *this;
}

Color& Color::darken (int amount)
{
	v -= amount / 100.0;

	return *this;
}

Color& Color::brighten (int amount)
{
	int red = Math::clamp(r - std::round(255.0 * -(amount / 100.0)), 0, 255);
	int green = Math::clamp(g - std::round(255.0 * -(amount / 100.0)), 0, 255);
	int blue = Math::clamp(b - std::round(255.0 * -(amount / 100.0)), 0, 255);

	return setTo(red, green, blue);
}

double Color::redGL ()
{
	return gl[0];
}

double Color::greenGL ()
{
	return gl[1];
}

double Color::blueGL ()
{
	return gl[2];
}

double Color::alphaGL ()
{
	return gl[3];
}

Color& Color::redGL (double value)
{
	gl[0] = std::min(std::abs(value), 1);

	r = std::floor(gl[0] * 255);

	update(true);

	return *this;
}

Color& Color::greenGL (double value)
{
	gl[1] = std::min(std::abs(value), 1);

	g = std::floor(gl[1] * 255);

	update(true);

	return *this;
}

Color& Color::blueGL (double value)
{
	gl[2] = std::min(std::abs(value), 1);

	b = std::floor(gl[2] * 255);

	update(true);

	return *this;
}

Color& Color::alphaGL (double value)
{
	gl[3] = std::min(std::abs(value), 1);

	a = std::floor(gl[3] * 255);

	update(true);

	return *this;
}

int Color::red ()
{
	return r;
}

int Color::green ()
{
	return g;
}

int Color::blue ()
{
	return b;
}

int Color::alpha ()
{
	return a;
}

Color& Color::red (int value)
{
	value = std::abs(value);

	r = std::min(value, 255);

	gl[0] = value / 255.0;

	update(true);
}

Color& Color::green (int value)
{
	value = std::abs(value);

	g = std::min(value, 255);

	gl[1] = value / 255.0;

	update(true);
}

Color& Color::blue (int value)
{
	value = std::abs(value);

	b = std::min(value, 255);

	gl[2] = value / 255.0;

	update(true);
}

Color& Color::alpha (int value)
{
	value = std::abs(value);

	a = std::min(value, 255);

	gl[3] = value / 255.0;

	update(true);
}

double Color::h ()
{
	return h;
}

double Color::s ()
{
	return s;
}

double Color::v ()
{
	return v;
}

Color& Color::h (double value)
{
	h = value;

	hsvToRgb(value, s, v, this);
}

Color& Color::s (double value)
{
	s = value;

	hsvToRgb(h, value, v, this);
}

Color& Color::v (double value)
{
	v = value;

	hsvToRgb(h, s, value, this);
}

// Static methods
int getColor (int red, int green, int blue)
{
	return red << 16 | green << 8 | blue;
}

int getColor32 (int red, int green, int blue, int alpha)
{
	return alpha << 24 | red << 16 | green << 8 | blue;
}

int convertValue (int n, double h, double s, double v)
{
	double k = (n + h * 6.0) % 6.0;

	double min = std::min(k, 4 - k, 1);

	return std::round(255 * (v - v * s * std::max(0, min)));
}

void hsvToRgb (double h, double s, double v, Color *out)
{
	int red = convertValue(5, h, s, v);
	int green = convertValue(3, h, s, v);
	int blue = convertValue(1, h, s, v);

	out->setTo(red, green, blue, out->alpha(), false);
}

void rgbToHsv (int r, int g, int b, Color *out)
{
	double red = r / 255;
	double green = g / 255;
	double blue = b / 255;

	double min = std::min(red, std::min(green, blue));
	double max = std::max(red, std::max(green, blue));
	double d = max - min;

	// Achromatic by default
	double h = 0.0;
	double s = (max == 0.0) ? 0.0 : d / max;
	double v = max;

	if (max != min) {
		if (max == red)
            h = (green - blue) / d + ((green < blue) ? 6.0 : 0.0);
        else if (max == green)
            h = (blue - red) / d + 2.0;
        else if (max == blue)
            h = (red - green) / d + 4.0;

        h /= 6.0;
	}

	out->h(h);
	out->s(s);
	out->v(v);
}

}	// namespace Display
} 	// namespace Zen
