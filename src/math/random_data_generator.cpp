/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#include "random_data_generator.h"

namespace Zen {
namespace Math {

RandomDataGenerator::RandomDataGenerator ()
{
	// Use the random device to generate a seed for the random engine
	seed();
}

void RandomDataGenerator::seed ()
{
	seeds = randomDevice();

	engine.seed(seeds);
}

void RandomDataGenerator::seed (unsigned int seed_)
{
	seeds = seed_;

	engine.seed(seeds);
}

int RandomDataGenerator::integer (int min_, int max_)
{
	std::uniform_int_distribution<> dist (min_, max_);

	return dist(engine);
}

int RandomDataGenerator::between (int min_, int max_)
{
	return integer(min_, max_);
}

double RandomDataGenerator::real (double min_, double max_)
{
	std::uniform_real_distribution<> dist (min_, max_);

	return dist(engine);
}

double RandomDataGenerator::between (double min_, double max_)
{
	return real(min_, max_);
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

unsigned int RandomDataGenerator::getSeed ()
{
	return seeds;
}

// Instantiate a Random object in the Zen::Math namespace
RandomDataGenerator random;
// Add an alias
RandomDataGenerator& RND = random;

}	// namespace Math
}	// namespace Zen
