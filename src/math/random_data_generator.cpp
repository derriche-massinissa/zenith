/**
 * @file		random_data_generator.cpp
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "random_data_generator.h"

namespace Zen {
namespace Math {

RandomDataGenerator::Random ()
{
	// Use the random device to generate a seed for the random engine
	seed();
}

void RandomDataGenerator::seed ()
{
	seed = randomDevice();

	engine.seed(seed);
}

void RandomDataGenerator::seed (unsigned int seeds)
{
	seed = seeds;

	engine.seed(seed);
}

int RandomDataGenerator::integer (int min, int max)
{
	std::uniform_int_distribution<> dist (min, max);

	return dist(engine);
}

int RandomDataGenerator::between (int min, int max)
{
	return integer(min, max);
}

double RandomDataGenerator::real (double min, double max)
{
	std::uniform_real_distribution<> dist (min, max);

	return dist(engine);
}

double RandomDataGenerator::between (double min, double max)
{
	return real(min, max);
}

double RandomDataGenerator::frac ()
{
	return real(0.0, 1.0);
}

double RandomDataGenerator::normal ()
{
	return real(-1.0, 1.0);
}

int RandomDataGenerator::sign ()
{
	return 1 - (integer(0, 1) * 2);
}

int RandomDataGenerator::angle ()
{
	return integer(-180, 180);
}

double RandomDataGenerator::rotation ()
{
	return real(-M_PI, M_PI);
}

unsigned int getSeed ()
{
	return seed;
}

// Instantiate a Random object in the Zen::Math namespace
RandomDataGenerator random;
// Add an alias
RandomDataGenerator& RND = random;

}	// namespace Math
}	// namespace Zen
