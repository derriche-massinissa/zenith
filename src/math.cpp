/**
 * @file		math.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "math.h"

int Zen::Math::clamp (int value, int min, int max)
{
	return std::max(min, std::min(max, value));
}

double Zen::Math::clamp (double value, double min, double max)
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
