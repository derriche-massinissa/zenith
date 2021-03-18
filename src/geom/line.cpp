/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "line.h"
#include <cmath>

namespace Zen {
namespace Geom {

Line::Line (int x1_, int y1_, int x2_, int y2_)
	: x1(x1_), y1(y1_), x2(x2_), y2(y2_)
{}

Point Line::getPoint (double position_)
{
	return {
		x1 + (x2 - x1) * position_,
		y1 + (y2 - y1) * position_
	};
}

std::vector<Point> Line::getPoints (int quantity_, double stepRate_)
{
	std::vector<Point> out_;

	// If quantity_ is 0, we calculate it based on the stepRate_ instead
	if (!quantity_)
		quantity_ = getLength() / stepRate_;

	for (int i_ = 0; i_ < quantity_; i_++)
	{
		out_.emplace_back(
				getPoint(i_ / quantity_)
				);
	}

	return out_;
}

double Line::getLength ()
{
	return std::sqrt(
			std::pow(x2 - x1, 2) +
			std::pow(y2 - y1, 2)
			);
}

Point Line::getRandomPoint ()
{
	double t_ = Math::random.frac();

	return {
		x1 + t_ * (x2 - x1),
		y1 + t_ * (y2 - y1)
	};
}

Line& Line::setTo (int x1_, int y1_, int x2_, int y2_)
{
	x1 = x1_;
	y1 = y1_;

	x2 = x2_;
	y2 = y2_;

	return *this;
}

Math::Vector2 Line::getPointA ()
{
	return {x1, y1};
}

Math::Vector2 Line::getPointB ()
{
	return {x2, y2};
}

int Line::left ()
{
	return getLeft();
}

int Line::getLeft ()
{
	return std::min(x1, x2);
}

int Line::right ()
{
	return getRight();
}

int Line::getRight ()
{
	return std::max(x1, x2);
}

int Line::top ()
{
	return getTop();
}

int Line::getTop ()
{
	return std::min(y1, y2);
}

int Line::bottom ()
{
	return getBottom();
}

int Line::getBottom ()
{
	return std::max(y1, y2);
}

Line& Line::left (int value_)
{
	return setLeft(value_);
}

Line& Line::setLeft (int value_)
{
	if (x1 <= x2)
		x1 = value_;
	else
		x2 = value_;

	return *this;
}

Line& Line::right (int value_)
{
	return setRight(value_);
}

Line& Line::setRight (int value_)
{
	if (x1 > x2)
		x1 = value_;
	else
		x2 = value_;

	return *this;
}

Line& Line::top (int value_)
{
	return setTop(value_);
}

Line& Line::setTop (int value_)
{
	if (y1 <= y2)
		y1 = value_;
	else
		y2 = value_;

	return *this;
}

Line& Line::bottom (int value_)
{
	return setBottom(value_);
}

Line& Line::setBottom (int value_)
{
	if (y1 > y2)
		y1 = value_;
	else
		y2 = value_;

	return *this;
}

}	// namespace Geom
}	// namespace Zen
