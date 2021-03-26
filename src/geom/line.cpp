/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "line.hpp"

#include <cmath>
#include "../math/random.hpp"

namespace Zen {

Math::Vector2 GetPoint (Line line, double position)
{
	return {
		line.x1 + (line.x2 - line.x1) * position,
		line.y1 + (line.y2 - line.y1) * position
	};
}

std::vector<Math::Vector2> GetPoints (Line line, int quantity, double stepRate)
{
	std::vector<Math::Vector2> out;

	// If quantity is 0, we calculate it based on the stepRate_ instead
	if (!quantity)
		quantity = GetLength(line) / stepRate;

	for (int i = 0; i < quantity; i++)
	{
		out.emplace_back(
				GetPoint(line, static_cast<double>(i) / static_cast<double>(quantity))
				);
	}

	return out;
}

double GetLength (Line line)
{
	return std::sqrt(
			std::pow(line.x2 - line.x1, 2) +
			std::pow(line.y2 - line.y1, 2)
			);
}

Math::Vector2 GetRandomPoint (Line line)
{
	double t = Math::Random.frac();

	return {
		line.x1 + t * (line.x2 - line.x1),
		line.y1 + t * (line.y2 - line.y1)
	};
}

void SetTo (Line *line, double x1, double y1, double x2, double y2)
{
	line->x1 = x1;
	line->y1 = y1;

	line->x2 = x2;
	line->y2 = y2;
}

int GetLeft (Line line)
{
	return std::min(line.x1, line.x2);
}

int GetRight (Line line)
{
	return std::max(line.x1, line.x2);
}

int GetTop (Line line)
{
	return std::min(line.y1, line.y2);
}

int GetBottom (Line line)
{
	return std::max(line.y1, line.y2);
}

void SetLeft (Line *line, int value)
{
	if (line->x1 <= line->x2)
		line->x1 = value;
	else
		line->x2 = value;
}

void SetRight (Line *line, int value)
{
	if (line->x1 > line->x2)
		line->x1 = value;
	else
		line->x2 = value;
}

void SetTop (Line *line, int value)
{
	if (line->y1 <= line->y2)
		line->y1 = value;
	else
		line->y2 = value;
}

void SetBottom (Line *line, int value)
{
	if (line->y1 > line->y2)
		line->y1 = value;
	else
		line->y2 = value;
}

}	// namespace Zen
