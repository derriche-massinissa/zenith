/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "color_matrix.hpp"
#include <cmath>

namespace Zen {

void Set (ColorMatrix* colorMatrix, std::array<double, 20> value)
{
	colorMatrix->matrix = value;
	colorMatrix->dirty = true;
}

void Reset (ColorMatrix* colorMatrix)
{
	colorMatrix->matrix = {
		1, 0, 0, 0, 0,
		0, 1, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 1, 0
	};

	colorMatrix->dirty = true;
}

std::array<double, 20> GetData (ColorMatrix* colorMatrix)
{
	if (colorMatrix->dirty) {
		colorMatrix->data = colorMatrix->matrix;

		colorMatrix->data[4] /= 255;
		colorMatrix->data[9] /= 255;
		colorMatrix->data[14] /= 255;
		colorMatrix->data[19] /= 255;

		colorMatrix->dirty = false;
	}

	return colorMatrix->data;
}

void Brightness (ColorMatrix* colorMatrix, double v, bool multiply)
{
	Multiply(colorMatrix, {
		v, 0, 0, 0, 0,
		0, v, 0, 0, 0,
		0, 0, v, 0, 0,
		0, 0, 0, 1, 0
	}, multiply);
}

void Saturate (ColorMatrix* colorMatrix, double value, bool multiply)
{
	double x = (value * 2 / 3) + 1;
	double y = ((x - 1) * -0.5);

	Multiply(colorMatrix, {
		x, y, y, 0, 0,
		y, x, y, 0, 0,
		y, y, x, 0, 0,
		0, 0, 0, 1, 0
	}, multiply);
}

void Desaturate (ColorMatrix* colorMatrix, bool multiply)
{
	Saturate(colorMatrix, -1, multiply);
}

void Hue (ColorMatrix* colorMatrix, double rotation, bool multiply)
{
	rotation = rotation / 180 * M_PI;

	double cos = std::cos(rotation);
	double sin = std::sin(rotation);
	double lumR = 0.213;
	double lumG = 0.715;
	double lumB = 0.072;

	Multiply(colorMatrix, {
		lumR + cos * (1 - lumR) + sin * (-lumR),lumG + cos * (-lumG) + sin * (-lumG),lumB + cos * (-lumB) + sin * (1 - lumB), 0, 0,
		lumR + cos * (-lumR) + sin * (0.143),lumG + cos * (1 - lumG) + sin * (0.140),lumB + cos * (-lumB) + sin * (-0.283), 0, 0,
		lumR + cos * (-lumR) + sin * (-(1 - lumR)),lumG + cos * (-lumG) + sin * (lumG),lumB + cos * (1 - lumB) + sin * (lumB), 0, 0,
		0, 0, 0, 1, 0
	}, multiply);
}

void Grayscale (ColorMatrix* colorMatrix, double value, bool multiply)
{
	Saturate(colorMatrix, -value, multiply);
}

void BlackWhite (ColorMatrix* colorMatrix, bool multiply)
{
	Multiply(colorMatrix, {
		0.2126, 0.7152, 0.0722, 0, 0,
		0.2126, 0.7152, 0.0722, 0, 0,
		0.2126, 0.7152, 0.0722, 0, 0,
		0, 0, 0, 1, 0
	}, multiply);
}

void Contrast (ColorMatrix* colorMatrix, double value, bool multiply)
{
	double v = value + 1;
	double o = -0.5 * (v - 1);

	Multiply(colorMatrix, {
		v, 0, 0, 0, o,
		0, v, 0, 0, o,
		0, 0, v, 0, o,
		0, 0, 0, 1, 0
	}, multiply);
}

void Negative (ColorMatrix* colorMatrix, bool multiply)
{
	Multiply(colorMatrix, {
		-1, 0, 0, 1, 0,
		0, -1, 0, 1, 0,
		0, 0, -1, 1, 0,
		0, 0, 0, 1, 0
	}, multiply);
}

void DesaturateLuminance (ColorMatrix* colorMatrix, bool multiply)
{
	Multiply(colorMatrix, {
		0.2764723, 0.9297080, 0.0938197, 0, -37.1,
		0.2764723, 0.9297080, 0.0938197, 0, -37.1,
		0.2764723, 0.9297080, 0.0938197, 0, -37.1,
		0, 0, 0, 1, 0
	}, multiply);
}

void Sepia (ColorMatrix* colorMatrix, bool multiply)
{
	Multiply(colorMatrix, {
		0.393, 0.7689999, 0.18899999, 0, 0,
		0.349, 0.6859999, 0.16799999, 0, 0,
		0.272, 0.5339999, 0.13099999, 0, 0,
		0, 0, 0, 1, 0
	}, multiply);
}

void Night (ColorMatrix* colorMatrix, double intensity, bool multiply)
{
	Multiply(colorMatrix, {
		intensity * (-2.0), -intensity, 0, 0, 0,
		-intensity, 0, intensity, 0, 0,
		0, intensity, intensity * 2.0, 0, 0,
		0, 0, 0, 1, 0
	}, multiply);
}

void Lsd (ColorMatrix* colorMatrix, bool multiply)
{
	Multiply(colorMatrix, {
		2, -0.4, 0.5, 0, 0,
		-0.5, 2, -0.4, 0, 0,
		-0.4, -0.5, 3, 0, 0,
		0, 0, 0, 1, 0
	}, multiply);
}

void Brown (ColorMatrix* colorMatrix, bool multiply)
{
	Multiply(colorMatrix, {
		0.5997023498159715, 0.34553243048391263, -0.2708298674538042, 0, 47.43192855600873,
		-0.037703249837783157, 0.8609577587992641, 0.15059552388459913, 0, -36.96841498319127,
		0.24113635128153335, -0.07441037908422492, 0.44972182064877153, 0, -7.562075277591283,
		0, 0, 0, 1, 0
	}, multiply);
}

void VintagePinhole (ColorMatrix* colorMatrix, bool multiply)
{
	Multiply(colorMatrix, {
		0.6279345635605994, 0.3202183420819367, -0.03965408211312453, 0, 9.651285835294123,
		0.02578397704808868, 0.6441188644374771, 0.03259127616149294, 0, 7.462829176470591,
		0.0466055556782719, -0.0851232987247891, 0.5241648018700465, 0, 5.159190588235296,
		0, 0, 0, 1, 0
	}, multiply);
}

void Kodachrome (ColorMatrix* colorMatrix, bool multiply)
{
	Multiply(colorMatrix, {
		1.1285582396593525, -0.3967382283601348, -0.03992559172921793, 0, 63.72958762196502,
		-0.16404339962244616, 1.0835251566291304, -0.05498805115633132, 0, 24.732407896706203,
		-0.16786010706155763, -0.5603416277695248, 1.6014850761964943, 0, 35.62982807460946,
		0, 0, 0, 1, 0
	}, multiply);
}

void Technicolor (ColorMatrix* colorMatrix, bool multiply)
{
	Multiply(colorMatrix, {
		1.9125277891456083, -0.8545344976951645, -0.09155508482755585, 0, 11.793603434377337,
		-0.3087833385928097, 1.7658908555458428, -0.10601743074722245, 0, -70.35205161461398,
		-0.231103377548616, -0.7501899197440212, 1.847597816108189, 0, 30.950940869491138,
		0, 0, 0, 1, 0
	}, multiply);
}

void Polaroid (ColorMatrix* colorMatrix, bool multiply)
{
	Multiply(colorMatrix, {
		1.438, -0.062, -0.062, 0, 0,
		-0.122, 1.378, -0.122, 0, 0,
		-0.016, -0.016, 1.483, 0, 0,
		0, 0, 0, 1, 0
	}, multiply);
}

void ShiftToBGR (ColorMatrix* colorMatrix, bool multiply)
{
	Multiply(colorMatrix, {
		0, 0, 1, 0, 0,
		0, 1, 0, 0, 0,
		1, 0, 0, 0, 0,
		0, 0, 0, 1, 0
	}, multiply);
}

void Multiply (ColorMatrix* colorMatrix, std::array<double, 20> a,
		bool multiply)
{
	if (!multiply)
		Reset(colorMatrix);

	std::array<double, 20> c = colorMatrix->matrix;
	auto &m = colorMatrix->matrix;

	// R
	m[0] = (c[0] * a[0]) + (c[1] * a[5]) + (c[2] * a[10]) + (c[3] * a[15]);
	m[1] = (c[0] * a[1]) + (c[1] * a[6]) + (c[2] * a[11]) + (c[3] * a[16]);
	m[2] = (c[0] * a[2]) + (c[1] * a[7]) + (c[2] * a[12]) + (c[3] * a[17]);
	m[3] = (c[0] * a[3]) + (c[1] * a[8]) + (c[2] * a[13]) + (c[3] * a[18]);
	m[4] = (c[0] * a[4]) + (c[1] * a[9]) + (c[2] * a[14]) + (c[3] * a[19]) + c[4];

	// G
	m[5] = (c[5] * a[0]) + (c[6] * a[5]) + (c[7] * a[10]) + (c[8] * a[15]);
	m[6] = (c[5] * a[1]) + (c[6] * a[6]) + (c[7] * a[11]) + (c[8] * a[16]);
	m[7] = (c[5] * a[2]) + (c[6] * a[7]) + (c[7] * a[12]) + (c[8] * a[17]);
	m[8] = (c[5] * a[3]) + (c[6] * a[8]) + (c[7] * a[13]) + (c[8] * a[18]);
	m[9] = (c[5] * a[4]) + (c[6] * a[9]) + (c[7] * a[14]) + (c[8] * a[19]) + c[9];

	// B
	m[10] = (c[10] * a[0]) + (c[11] * a[5]) + (c[12] * a[10]) + (c[13] * a[15]);
	m[11] = (c[10] * a[1]) + (c[11] * a[6]) + (c[12] * a[11]) + (c[13] * a[16]);
	m[12] = (c[10] * a[2]) + (c[11] * a[7]) + (c[12] * a[12]) + (c[13] * a[17]);
	m[13] = (c[10] * a[3]) + (c[11] * a[8]) + (c[12] * a[13]) + (c[13] * a[18]);
	m[14] = (c[10] * a[4]) + (c[11] * a[9]) + (c[12] * a[14]) + (c[13] * a[19]) + c[14];

	// A
	m[15] = (c[15] * a[0]) + (c[16] * a[5]) + (c[17] * a[10]) + (c[18] * a[15]);
	m[16] = (c[15] * a[1]) + (c[16] * a[6]) + (c[17] * a[11]) + (c[18] * a[16]);
	m[17] = (c[15] * a[2]) + (c[16] * a[7]) + (c[17] * a[12]) + (c[18] * a[17]);
	m[18] = (c[15] * a[3]) + (c[16] * a[8]) + (c[17] * a[13]) + (c[18] * a[18]);
	m[19] = (c[15] * a[4]) + (c[16] * a[9]) + (c[17] * a[14]) + (c[18] * a[19]) + c[19];

	colorMatrix->dirty = true;
}

}	// namespace Zen
