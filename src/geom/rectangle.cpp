/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "rectangle.hpp"

#include "../math/random.hpp"

namespace Zen {

bool Contains (Rectangle rectangle, double x, double y)
{
    if (rectangle.width <= 0 || rectangle.height <= 0)
        return false;

    return (rectangle.x <= x && rectangle.x + rectangle.width >= x &&
			rectangle.y <= y && rectangle.y + rectangle.height >= y);
}

bool RectangleContains (Shape shape, double x, double y)
{
	return Contains(shape.rectangle, x, y);
}

Math::Vector2 GetPoint (Rectangle rectangle, double position)
{
	Math::Vector2 out;

	if (position <= 0 || position >= 1)
	{
		out.x = rectangle.x;
		out.y = rectangle.y;

		return out;
	}

	double p = GetPerimeter(rectangle) * position;

	if (position > 0.5)
	{
		p -= (rectangle.width + rectangle.height);

		if (p <= rectangle.width)
		{
			// Face 3
			out.x = GetRight(rectangle) - p;
			out.y = GetBottom(rectangle);
		}
		else
		{
			// Face 4
			out.x = rectangle.x;
			out.y = GetBottom(rectangle) - (p - rectangle.width);
		}
	}
	else if (p <= rectangle.width)
	{
		// Face 1
		out.x = rectangle.x + p;
		out.y = rectangle.y;
	}
	else
	{
		// Face 2
		out.x = GetRight(rectangle);
		out.y = rectangle.y + (p - rectangle.width);
	}

	return out;
}

std::vector<Math::Vector2> GetPoints (Rectangle rectangle, int quantity, double stepRate)
{
	std::vector<Math::Vector2> out;

	// If quantity is 0, we calculate it based on stepRate
	if (!quantity)
		quantity = GetPerimeter(rectangle) / stepRate;

	for (int i = 0; i < quantity; i++)
	{
		double pos = static_cast<double>(i) / static_cast<double>(quantity);

		out.emplace_back(GetPoint(rectangle, pos));
	}

	return out;
}

Math::Vector2 GetRandomPoint (Rectangle rectangle)
{
	Math::Vector2 out;

	out.x = rectangle.x + Math::Random.between(0., rectangle.width);
	out.y = rectangle.y + Math::Random.between(0., rectangle.height);

	return out;
}

double GetPerimeter (Rectangle rectangle)
{
	return 2 * (rectangle.width + rectangle.height);
}

void SetTo (Rectangle *rectangle, double x, double y, double width, double height)
{
	rectangle->x = x;
	rectangle->y = y;
	rectangle->width = width;
	rectangle->height = height;
}

void SetEmpty (Rectangle *rectangle)
{
	SetTo(rectangle, 0, 0, 0, 0);
}

void SetPosition (Rectangle *rectangle, double x, double y)
{
	rectangle->x = x;
	rectangle->y = y;
}

void SetPosition (Rectangle *rectangle, double x)
{
	SetPosition(rectangle, x, x);
}

void SetSize (Rectangle *rectangle, double width, double height)
{
	rectangle->width = width;
	rectangle->height = height;
}

void SetSize (Rectangle *rectangle, double value)
{
	return SetSize(rectangle, value, value);
}

void CenterOn (Rectangle *rectangle, double x, double y)
{
	CenterX(rectangle, x);
	CenterY(rectangle, y);
}

void CenterX (Rectangle *rectangle, double x)
{
	rectangle->x = x - (rectangle->width / 2.);
}

void CenterY (Rectangle *rectangle, double y)
{
	rectangle->y = y - (rectangle->height / 2.);
}

bool IsEmpty (Rectangle *rectangle)
{
	return (rectangle->width <= 0 || rectangle->height <= 0);
}

Line GetLineA (Rectangle rectangle)
{
	return Line(rectangle.x, rectangle.y, GetRight(rectangle), rectangle.y);
}

Line GetLineB (Rectangle rectangle)
{
	return Line(GetRight(rectangle), rectangle.y, GetRight(rectangle), GetBottom(rectangle));
}

Line GetLineC (Rectangle rectangle)
{
	return Line(GetRight(rectangle), GetBottom(rectangle), rectangle.x, GetBottom(rectangle));
}

Line GetLineD (Rectangle rectangle)
{
	return Line(rectangle.x, GetBottom(rectangle), rectangle.x, rectangle.y);
}

double GetLeft (Rectangle rectangle)
{
	return rectangle.x;
}

void SetLeft (Rectangle *rectangle, double value)
{
	if (value >= GetRight(*rectangle))
		rectangle->width = 0;
	else
		rectangle->width = GetRight(*rectangle) - value;

	rectangle->x = value;
}

double GetRight (Rectangle rectangle)
{
	return rectangle.x + rectangle.width;
}
void SetRight (Rectangle *rectangle, double value)
{
	if (value <= rectangle->x)
		rectangle->width = 0;
	else
		rectangle->width = value - rectangle->x;
}

double GetTop (Rectangle rectangle)
{
	return rectangle.y;
}

void SetTop (Rectangle *rectangle, double value)
{
	if (value >= GetBottom(*rectangle))
		rectangle->height = 0;
	else
		rectangle->height = GetBottom(*rectangle) - value;

	rectangle->y = value;
}

double GetBottom (Rectangle rectangle)
{
	return rectangle.y + rectangle.height;
}

void SetBottom (Rectangle *rectangle, double value)
{
	if (value <= rectangle->y)
		rectangle->height = 0;
	else
		rectangle->height = value - rectangle->y;
}

double GetCenterX (Rectangle rectangle)
{
	return rectangle.x + (rectangle.width / 2);
}

void SetCenterX (Rectangle *rectangle, double value)
{
	rectangle->x = value - (rectangle->width / 2);
}

double GetCenterY (Rectangle rectangle)
{
	return rectangle.y + (rectangle.height / 2);
}
void SetCenterY (Rectangle *rectangle, double value)
{
	rectangle->y = value - (rectangle->height / 2);
}

}	// namespace Zen
