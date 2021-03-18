/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "rectangle.h"

namespace Zen {
namespace Geom {

Rectangle::Rectangle (int x_, int y_, int width_, int height_)
	: x(x_), y(y_), width(width_), height (height_)
{}

bool Rectangle::contains (int x_, int y_)
{
    if (width <= 0 || height <= 0)
        return false;

    return (x <= x_ && x + width >= x_ &&
			y <= y_ && y + height >= y_);
}

Point Rectangle::getPoint (double position_)
{
	Point out_;

	if (position_ <= 0 || position_ >= 1)
	{
		out_.x = x;
		out_.y = y;

		return out_;
	}

	double p_ = getPerimeter() * position_;

	if (position_ > 0.5)
	{
		p_ -= (width + height);

		if (p_ <= width)
		{
			// Face 3
			out_.x = getRight() - p_;
			out_.y = getBottom();
		}
		else
		{
			// Face 4
			out_.x = x;
			out_.y = getBottom() - (p_ - width);
		}
	}
	else if (p_ <= width)
	{
		// Face 1
		out_.x = x + p_;
		out_.y = y;
	}
	else
	{
		// Face 2
		out_.x = getRight();
		out_.y = y + (p_ - width);
	}

	return out_;
}

std::vector<Point> Rectangle::getPoints (int quantity_, double stepRate_)
{
	std::vector<Point> out_;

	// If quantity_ is 0, we calculate it based on stepRate_
	if (!quantity_)
		quantity_ = getPerimeter() / stepRate_;

	for (int i_ = 0; i_ < quantity_; i_++)
	{
		double pos_ = i_ / quantity_;

		out_.emplace_back(getPoint(pos_));
	}

	return out_;
}

Point Rectangle::getRandomPoint ()
{
	Point out_;

	out_.x = x + Math::random.between(0, width);
	out_.y = y + Math::random.between(0, height);

	return out_;
}

double Rectangle::getPerimeter ()
{
	return 2 * (width + height);
}

Rectangle& Rectangle::setTo (int x_, int y_, int width_, int height_)
{
	x = x_;
	y = y_;
	width = width_;
	height = height_;

	return *this;
}

Rectangle& Rectangle::setEmpty ()
{
	return setTo(0, 0, 0 ,0);
}

Rectangle& Rectangle::setPosition (int x_, int y_)
{
	x = x_;
	y = y_;

	return *this;
}

Rectangle& Rectangle::setPosition (int x_)
{
	return setPosition(x_);
}

Rectangle& Rectangle::setSize (int width_, int height_)
{
	width = width_;
	height = height_;

	return *this;
}

Rectangle& Rectangle::setSize (int width_)
{
	return setSize(width_);
}

Rectangle& Rectangle::centerOn (int x_, int y_)
{
	centerX(x_);
	centerY(y_);

	return *this;
}

bool Rectangle::isEmpty ()
{
	return (width <= 0 || height <= 0);
}

Line Rectangle::getLineA ()
{
	return {x, y, right(), y};
}

Line Rectangle::getLineB ()
{
	return {right(), y, right(), bottom()};
}

Line Rectangle::getLineC ()
{
	return {right(), bottom(), x, bottom()};
}

Line Rectangle::getLineD ()
{
	return {x, bottom(), x, y};
}

int Rectangle::getLeft ()
{
	return x;
}
int Rectangle::left ()
{
	return x;
}
Rectangle& Rectangle::setLeft (int value_)
{
	if (value_ >= right())
		width = 0;
	else
		width = right() - value_;

	x = value_;

	return *this;
}
Rectangle& Rectangle::left (int value_)
{
	return setLeft(value_);
}

int Rectangle::getRight ()
{
	return x + width;
}
int Rectangle::right ()
{
	return x + width;
}
Rectangle& Rectangle::setRight (int value_)
{
	if (value_ <= x)
		width = 0;
	else
		width = value_ - x;

	return *this;
}
Rectangle& Rectangle::right (int value_)
{
	return setRight(value_);
}

int Rectangle::getTop ()
{
	return y;
}
int Rectangle::top ()
{
	return y;
}
Rectangle& Rectangle::setTop (int value_)
{
	if (value_ >= bottom())
		height = 0;
	else
		height = bottom() - value_;

	y = value_;

	return *this;
}
Rectangle& Rectangle::top (int value_)
{
	return setTop(value_);
}

int Rectangle::getBottom ()
{
	return y + height;
}
int Rectangle::bottom ()
{
	return y + height;
}
Rectangle& Rectangle::setBottom (int value_)
{
	if (value_ <= y)
		height = 0;
	else
		height = value_ - y;

	return *this;
}
Rectangle& Rectangle::bottom (int value_)
{
	return setBottom(value_);
}

int Rectangle::getCenterX ()
{
	return x + (width / 2);
}
int Rectangle::centerX ()
{
	return x + (width / 2);
}
Rectangle& Rectangle::setCenterX (int value_)
{
	x = value_ - (width / 2);

	return *this;
}
Rectangle& Rectangle::centerX (int value_)
{
	return setCenterX(value_);
}

int Rectangle::getCenterY ()
{
	return y + (height / 2);
}
int Rectangle::centerY ()
{
	return y + (height / 2);
}
Rectangle& Rectangle::setCenterY (int value_)
{
	y = value_ - (height / 2);

	return *this;
}
Rectangle& Rectangle::centerY (int value_)
{
	return setCenterY(value_);
}

}	// namespace Geom
}	// namespace Zen
