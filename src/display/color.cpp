/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "color.h"

namespace Zen {
namespace Display {

Color::Color (int red_, int green_, int blue_, int alpha_, bool validity_)
{
	setTo(red_, green_, blue_, alpha_);
	valid = validity_;
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

Color& Color::setTo (int red_, int green_, int blue_, int alpha_, bool updateHSV_)
{
	locked = true;

	red(red_);
	green(green_);
	blue(blue_);
	alpha(alpha_);

	locked = false;

	return update(updateHSV_);
}

Color& Color::setGLTo (double red_, double green_, double blue_, double alpha_)
{
	locked = true;

	redGL(red_);
	greenGL(green_);
	blueGL(blue_);
	alphaGL(alpha_);

	locked = false;

	return update(true);
}

Color& Color::setFromHSV (double h_, double s_, double v_)
{
	hsvToRgb(h_, s_, v_, this);

	return *this;
}

Color& setFromHex (unsigned int hex_)
{
	int red_ = (hex_ & 0xff0000) >> 16;
	int green_ = (hex_ & 0x00ff00) >> 8;
	int blue_ = (hex_ & 0x0000ff);

	return setTo(red_, green_, blue_);
}

Color& Color::update (bool updateHSV_)
{
	if (locked)
		return *this;

	color = getColor(red(), green(), blue());
	color32 = getColor32(red(), green(), blue(), alpha());

	if (updateHSV_)
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

Color& Color::gray (int shade_)
{
	return setTo(shade_, shade_, shade_);
}

Color& Color::random (int min_, int max_)
{
	int red_ = Math::random.integer(min_, max_);
	int green_ = Math::random.integer(min_, max_);
	int blue_ = Math::random.integer(min_, max_);

	return setTo(red_, green_, blue_);
}

Color& Color::randomGray (int min_, int max_)
{
	int shade_ = Math::random.integer(min_, max_);

	return setTo(shade_, shade_, shade_);
}

Color& Color::saturate (int amount_)
{
	s += amount_ / 100.0;

	return *this;
}

Color& Color::desaturate (int amount_)
{
	s -= amount_ / 100.0;

	return *this;
}

Color& Color::lighten (int amount_)
{
	v += amount_ / 100.0;

	return *this;
}

Color& Color::darken (int amount_)
{
	v -= amount_ / 100.0;

	return *this;
}

Color& Color::brighten (int amount_)
{
	int red_ = Math::clamp(r - std::round(255.0 * -(amount_ / 100.0)), 0, 255);
	int green_ = Math::clamp(g - std::round(255.0 * -(amount_ / 100.0)), 0, 255);
	int blue_ = Math::clamp(b - std::round(255.0 * -(amount_ / 100.0)), 0, 255);

	return setTo(red_, green_, blue_);
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

Color& Color::redGL (double value_)
{
	gl[0] = std::min(std::abs(value_), 1);

	r = std::floor(gl[0] * 255);

	update(true);

	return *this;
}

Color& Color::greenGL (double value_)
{
	gl[1] = std::min(std::abs(value_), 1);

	g = std::floor(gl[1] * 255);

	update(true);

	return *this;
}

Color& Color::blueGL (double value_)
{
	gl[2] = std::min(std::abs(value_), 1);

	b = std::floor(gl[2] * 255);

	update(true);

	return *this;
}

Color& Color::alphaGL (double value_)
{
	gl[3] = std::min(std::abs(value_), 1);

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

Color& Color::red (int value_)
{
	value_ = std::abs(value_);

	r = std::min(value_, 255);

	gl[0] = value_ / 255.0;

	update(true);
}

Color& Color::green (int value_)
{
	value_ = std::abs(value_);

	g = std::min(value_, 255);

	gl[1] = value_ / 255.0;

	update(true);
}

Color& Color::blue (int value_)
{
	value_ = std::abs(value_);

	b = std::min(value_, 255);

	gl[2] = value_ / 255.0;

	update(true);
}

Color& Color::alpha (int value_)
{
	value_ = std::abs(value_);

	a = std::min(value_, 255);

	gl[3] = value_ / 255.0;

	update(true);
}

double Color::hue ()
{
	return h;
}

double Color::saturation ()
{
	return s;
}

double Color::value ()
{
	return v;
}

Color& Color::h (double value_)
{
	h = value_;

	hsvToRgb(value_, s, v, this);
}

Color& Color::s (double value_)
{
	s = value_;

	hsvToRgb(h, value_, v, this);
}

Color& Color::v (double value_)
{
	v = value_;

	hsvToRgb(h, s, value_, this);
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
