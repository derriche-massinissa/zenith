/**
 * @file		rectangle.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "rectangle.h"

Zen::Geom::Rectangle (int _x, int _y, int _width, int _height)
	: x(_x), y(_y), width(_width), height (_height)
{}

Zen::Geom::~Rectangle ()
{}

bool Zen::Geom::Rectangle::contains (
	int rectX, int rectY, int rectW, int rectH, int x, int y)
{
	if (rectW <= 0 || rectH <= 0)
		return false;

	return (rectX <= x && rectX+rectW >= x && rectY <= y && rectY+rectH >= y);
}
