/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "math.h"

#include <cmath>

#include "vector2.h"
#include "../geom/point.h"

namespace Zen {
namespace Math {

int clamp (int value, int min, int max)
{
	return std::max(min, std::min(max, value));
}

double clamp (double value, double min, double max)
{
	return std::max(min, std::min(max, value));
}

int linear (int p0, int p1, double t)
{
	return (p1 - p0) * t + p0;
}

double linear (double p0, double p1, double t)
{
	return (p1 - p0) * t + p0;
}

double snapFloor (double value, double gap, double start, bool divide)
{
	if (!gap)
		return value;

	value -= start;
	value = gap * std::floor(value / gap);

	return (divide) ? (start + value) / gap : start + value;
}

double snapCeil (double value, double gap, double start, bool divide)
{
	if (!gap)
		return value;

	value -= start;
	value = gap * std::ceil(value / gap);

	return (divide) ? (start + value) / gap : start + value;
}

double snapTo (double value, double gap, double start, bool divide)
{
	if (!gap)
		return value;

	value -= start;
	value = gap * std::round(value / gap);

	return (divide) ? (start + value) / gap : start + value;
}

bool fuzzyEqual (double a, double b, double epsilon)
{
	return std::abs(a - b) < epsilon;
}

bool fuzzyCeil (double value, double epsilon)
{
	return std::ceil(value - epsilon);
}

bool fuzzyFloor (double value, double epsilon)
{
	return std::floor(value + epsilon);
}

bool fuzzyGreaterThan (double a, double b, double epsilon)
{
	return a > b - epsilon;
}

bool fuzzyLessThan (double a, double b, double epsilon)
{
	return a < b + epsilon;
}

double degToRad (double degrees)
{
    return degrees * DEG_TO_RAD;
};

Math::Vector2 rotateAround (Math::Vector2 point, int x, int y, double angle)
{
	double c = std::cos(angle);
	double s = std::sin(angle);

	int tx = point.x - x;
	int ty = point.y - y;

	point.x = tx * c - ty * s + x;
	point.y = tx * s - ty * c + y;

	return point;
}

}	// namespace Math
}	// namespace Zen
