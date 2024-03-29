/**
 * @file
 * @author		__AUTHOR_NAME__ <mail@host.com>
 * @copyright	2021 __COMPANY_LTD__
 * @license		<a href="https://opensource.org/licenses/MIT">MIT License</a>
 */

#ifndef ZEN_MATH_RANDOM_DATA_GENERATOR_HPP
#define ZEN_MATH_RANDOM_DATA_GENERATOR_HPP

#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

namespace Zen {
namespace Math {

class RandomDataGenerator
{
public:
	RandomDataGenerator ();

	/**
	 * Sets the seed of this Random Data Generator using the internal random device.
	 *
	 * @since 0.0.0
	 */
	void seed ();

	/**
	 * Sets the seed of this Random Data Generator to the given seed.
	 *
	 * @param seeds_ The new seed value.
	 *
	 * @since 0.0.0
	 */
	void seed (unsigned int seed_);

	/**
	 * Returns a random integer between min and max, both inclusive.
	 *
	 * @since 0.0.0
	 *
	 * @param min_ The minimum value in the range.
	 * @param max_ The maximum value in the range.
	 *
	 * @return A random number between `min` and `max`.
	 */
	int integer (int min_, int max_);

	/**
	 * Returns a random integer between min and max, both inclusive.
	 * This is an alias for RandomDataGenerator::integer.
	 *
	 * @since 0.0.0
	 *
	 * @param min_ The minimum value in the range.
	 * @param max_ The maximum value in the range.
	 *
	 * @return A random number between `min` and `max`.
	 */
	int between (int min_, int max_);

	/**
	 * Returns a random real number between min and max, both inclusive.
	 *
	 * @since 0.0.0
	 *
	 * @param min_ The minimum value in the range.
	 * @param max_ The maximum value in the range.
	 *
	 * @return A random number between `min` and `max`.
	 */
	double real (double min_, double max_);

	/**
	 * Returns a random real number between min and max, both inclusive.
	 * This is an alias for RandomDataGenerator::real.
	 *
	 * @since 0.0.0
	 *
	 * @param min_ The minimum value in the range.
	 * @param max_ The maximum value in the range.
	 *
	 * @return A random number between `min` and `max`.
	 */
	double between (double min_, double max_);

	/**
	 * Returns a random real number between 0 and 1.
	 *
	 * @since 0.0.0
	 *
	 * @return A random number between 0 and 1.
	 */
	double frac ();

	/**
	 * Returns a random real number between -1 and 1.
	 *
	 * @since 0.0.0
	 *
	 * @return A random real number between -1 and 1.
	 */
	double normal ();

	/**
     * Returns a reference to a random element from within the given vector.
	 *
	 * @since 0.0.0
	 *
	 * @param vector_ The vector to pick a random element from.
	 * @param start_ The start of the range to select from.
	 * @param length_ The number of elements to select from.
	 *
	 * @return A random member of the vector.
	 */
	template <typename T>
	T& pick (std::vector<T>& vector_, std::size_t start_ = 0, std::size_t length_ = 0)
	{
		if (length_)
			return vector_.at( integer(start_, start_ + length_) );
		else
			return vector_.at( integer(start_, vector_.size() - 1) );
	}

    /**
     * Returns a sign to be used with multiplication operator.
     *
     * @since 0.0.0
     *
     * @return -1 or +1.
     */
	int sign ();

    /**
     * Returns a reference to a random element from within the given vector,
	 * favoring the earlier entries.
     *
     * @since 0.0.0
     *
     * @param vector_ The vector to pick a random element from.
     *
     * @return A reference to a random member of the array.
     */
	template <typename T>
	T& weightedPick (const std::vector<T>& vector_)
	{
		return vector_.at(
				std::trunc(
					std::pow(frac(), 2.0) * (vector_.size() - 1) + 0.5
					)
				);
	}

    /**
     * Returns a random angle between -180 and 180.
     *
     * @since 0.0.0
     *
     * @return A random number between -180 and 180.
     */
	int angle ();

    /**
     * Returns a random rotation in radians, between `-pi` (-3.141) and `pi` (3.141)
     *
     * @since 0.0.0
     *
     * @return A random number between `-pi` and `pi`.
     */
	double rotation ();

    /**
     * Shuffles the given vector, using the current seed.
     *
     * @since 0.0.0
     *
     * @param vector_ A pointer to the vector to be shuffled.
     */
	template <typename T>
	void shuffle (std::vector<T> *vector_)
	{
		std::shuffle(vector_->begin(), vector_->end(), engine);
	}

    /**
     * Shuffles the given range of vector, using the current seed.
     *
     * @since 0.0.0
     *
     * @param vector_ A pointer to the vector to be shuffled.
     * @param begin_ The start index of the range to shuffle.
     * @param length_ A end index of the range to shuffle.
     */
	template <typename T>
	void shuffle (std::vector<T> *vector_, std::size_t begin_, std::size_t length_)
	{
		std::shuffle(vector_->begin() + begin_, vector_->begin() + begin_ + length_, engine);
	}

	/**
	 * Getter for the generator's seed.
	 *
	 * @since 0.0.0
	 *
	 * @return The current seed of this random data generator.
	 */
	unsigned int getSeed ();

private:
	/**
	 * A random device.
	 *
	 * Using it is really expensive, so it must only be done as little as possible!
	 *
	 * @since 0.0.0
	 */
	std::random_device randomDevice;

	/**
	 * A random engine.
	 *
	 * It takes a seed. We use the random device to generate it.
	 *
	 * @since 0.0.0
	 */
	std::mt19937 engine;

	/**
	 * The seed of this Random Data Generator.
	 *
	 * @since 0.0.0
	 */
	unsigned int seeds = 0;
};

}	// namespace Math
}	// namespace Zen

#endif
